#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <string.h>
#include "Bencoding.h"


char buf[BUFSIZE];
int pos = 0;
int buf_lim = 0;

Bencoding* new_bencoding()
{
	return malloc(sizeof(Bencoding));
}

ListNode* new_listnode()
{
	return malloc(sizeof(ListNode));
}

DictNode* new_dictnode()
{
	return malloc(sizeof(DictNode));
}

Bencoding* new_bint(int val)
{
	Bencoding *b = new_bencoding();
	b->type = BInt;
	b->cargo.val = val;
	return b;
}

Bencoding* new_blist(ListNode *l)
{
	Bencoding *b = new_bencoding();
	b->type = BList;
	b->cargo.list = l;
	return b;
}

Bencoding* new_bstring(char *s, int len)
{
	Bencoding *b = new_bencoding();
	b->type = BString;
	b->cargo.str.string = s;
	b->cargo.str.length = len;
	return b;
}

Bencoding* new_bdict(DictNode *d)
{
	Bencoding *b = new_bencoding();
	b->type = BDict;
	b->cargo.dict = d;
	return b;
}

void failed()
{
	printf("Parse FAILED\n");
	exit(-1);
}

/*
char reloadBuffer() 
{ 
	pos = 0;
	printf("BUFFER POS RESET\n");
	return ' ';
}
*/

void checkBufReady()
{
	if (pos >= buf_lim){
	//	reloadBuffer();
	  printf("BUFFER OVERFLOW!\n");
	  pos=0;
	}

}

char peekChar()
{
	checkBufReady();
	return buf[pos];
}

char getChar()
{
	checkBufReady();
	return buf[pos++];
}

void matchChar(char c)
{
	if (getChar() != c)
		failed();
}

Bencoding* parse_int() 
{
	matchChar('i');
	long long val = 0;
	while (isdigit(peekChar()))
		val = val*10 + (getChar() - '0');
	matchChar('e');
//	printf("%d\n",(int)val);
	return new_bint(val);
}

Bencoding* parse_list()
{
	matchChar('l');
	ListNode l;
	ListNode *c = &l;
//	printf("List: \n");
	while (peekChar() != 'e') {
		c->next = new_listnode();
		c = c->next;
		c->cargo = parse_bencoding();
		c->next = 0;
	}
	matchChar('e');
	return new_blist(l.next);
}

Bencoding* parse_string()
{
	int len = 0;
	while (isdigit(peekChar()))
		len = len*10 + (getChar() - '0');
	matchChar(':');
	char *s = malloc(sizeof(char)*(len+1));
	int i;
	for (i = 0; i < len; ++i){
		s[i] = getChar();
	}
	s[len] = 0;
	return new_bstring(s, len+1);
}

Bencoding* parse_dict()
{
	matchChar('d');
	DictNode d;
	DictNode *c = &d;
	while (peekChar() != 'e') {
		c->next = new_dictnode();
		c = c->next;
//		printf("Key: ");
		Bencoding *s = parse_string();
		c->key = s->cargo.str.string;
		free(s);
//		printf("Value: ");
		c->value = parse_bencoding();
		c->next = 0;
	}
	matchChar('e');
	return new_bdict(d.next);
}

Bencoding* parse_start(char* input,long limit)
{
	buf_lim= (int)limit;
	int i =0;
	for(;i<buf_lim; i++){
		buf[i] = input[i];
	}
	printf("Parsing...\n");
	Bencoding *b =  parse_bencoding();
	return b;
}

Bencoding* parse_bencoding()
{
	char c = peekChar();
	switch (c) {
		case 'd':
//		  printf("New Dictionary\n");
		  return parse_dict();
		case 'l':
//		  printf("List\n");
		  return parse_list();
		case 'i': 
//		  printf("Int\n");
		  return parse_int();
		default: 
//		  printf("String\n");
		  return parse_string();
	}
}

void print_indent(int indent)
{
	int i;
	for (i = 0; i < indent; ++i)
		printf("  ");
}


char* get_info_dict(char *input) {
	int index = 0;
	int info_found = 0;
	int info_ends_remaining = 1;
	while (!info_found) {
		if(input[index] == '4') {
			if(input[index+1] == ':') {
				if(input[index+2] == 'i') {
					if(input[index+3] == 'n') {
						if(input[index+4] == 'f') {
							if(input[index+5] == 'o') {
								if(input[index+6] == 'd') {;
									index += 6;
									info_found = index;
								}
							}
						}
					}
				}
			}
		}
		index++;
	}
	while(info_ends_remaining) {
		if(input[index] != 'e'){
			if(isdigit(input[index])){
				int total = input[index] -'0';
				while(input[index] != ':') {
					total = (total * 10) + input[index] - '0';
					index++;
				}
				index += total;
			} else if (input[index] == 'i') {
				index++;
				while(input[index] != 'e') {
					index++;
				}
				index++;
			} else if (input[index] == 'd'|| input[index] == 'l') {
				info_ends_remaining++;
				index++;
			} else {
				index++;
			}
		} else {
			info_ends_remaining--;
			index++;
		} 
	}
	char *ret = malloc((size_t) (index - info_found));
	snprintf(ret, (size_t) (index - info_found), "%s", &input[info_found]);
       	return ret;
}

Torrent* parse_torrent(char *input, long limit){
	Bencoding *b = parse_start(input, limit);
	Torrent *t = malloc(sizeof(Torrent));
	if(b->type != BDict){
		printf("Not a valid torrent bencode!\n");
		return NULL; 
	}
	DictNode *d = b->cargo.dict;
	DictNode *p = NULL;
	while(d != NULL){
		if(!strcmp(d->key,"announce")){
		  t->announce = d->value->cargo.str.string;
		  d = d->next;
		}
		else if(!strcmp(d->key,"name")){
		  t->name = d->value->cargo.str.string;
		  d = d->next;
		}
		else if(!strcmp(d->key,"piece length")){
		  t->piece_length = d->value->cargo.val;
		  d = d->next;
		}
		else if(!strcmp(d->key,"length")){
		  t->length = d->value->cargo.val;
		  d = d->next;
		}
/*
		else if(!strcmp(d->key,"files")){
		  parse_files(t,d->value->cargo.list);
		  d = d->next;
		}
*/
		else if(!strcmp(d->key,"pieces")){
		  t->pieces = d->value->cargo.str.string;
  		  t->pieces_size = d->value->cargo.str.length;
		  d = d->next;
		}
		else if(!strcmp(d->key,"url-list")){
		  t->url_list = d->value->cargo.str.string;
		  d = d->next;
		}
		else if(!strcmp(d->key,"info")){
		  p=d;
		  d = d->value->cargo.dict;
		}
		else{
		   d=d->next;
		}
		if(d==NULL){
		  if (p!=NULL){
		     d=p->next;
		     p=NULL;
		   }
		}
	}
	return t;
}

Response* parse_response(char *input, long limit)
{
	Bencoding *b = parse_start(input, limit);
	Response *r = malloc(sizeof(Response));
	if(b->type!=BDict){
	  printf("NOT A VALID RESPONSE!");
	  return NULL;
	}
	DictNode *d = b->cargo.dict;
	while(d != NULL){
		if(!strcmp(d->key,"failure reason")){
		  r->failure_reason = d->value->cargo.str.string;
		  d = d->next;
		}
		else if(!strcmp(d->key,"warning message")){
		  r->warning = d->value->cargo.str.string;
		  d = d->next;
		}
		else if(!strcmp(d->key,"interval")){
		  r->interval = d->value->cargo.val;
		  d = d->next;
		}
		else if(!strcmp(d->key,"min interval")){
		  r->min_interval = d->value->cargo.val;
		  d = d->next;
		}
		else if(!strcmp(d->key,"tracker id")){
		  r->tracker_id = d->value->cargo.str.string;
		  d = d->next;
		}
		else if(!strcmp(d->key,"complete")){
		  r->seeders = d->value->cargo.val;
		  d = d->next;
		}
		else if(!strcmp(d->key,"incomplete")){
		  r->leechers = d->value->cargo.val;
		  d = d->next;
		}
		else if(!strcmp(d->key,"peers")){
		  parse_peers(r,d->value->cargo.list);
		  d = d->next;
		}
		else{
		   d=d->next;
		}
	}
	return r;
}

void parse_peers(Response *r, ListNode *l)
{
  int index =0;
  while(l!=NULL){
    DictNode *d = l->cargo->cargo.dict;
    while(d!=NULL){
	if(!strcmp(d->key,"peer id")){
	  r->peers[index].peer_id = d->value->cargo.str.string;
	  d = d->next;
	}
	else if(!strcmp(d->key,"ip")){
	  r->peers[index].ip = d->value->cargo.str.string;
	  d = d->next;
	}
	else if(!strcmp(d->key,"port")){
	  r->peers[index].port = d->value->cargo.val;
	  d = d->next;
	}
	else{
	  d=d->next;
	}
    }
    index++;
    l=l->next;
  }
}

void print_bencoding(Bencoding *b, int indent)
{
	print_indent(indent);
	switch (b->type) {
		case BInt: 
			printf("%lld\n", b->cargo.val);
			break;
		case BList:
			printf("[\n");
			ListNode *c = b->cargo.list;
			while (c) {
				print_bencoding(c->cargo, indent+1);
				c = c->next;
			}
			print_indent(indent);
			printf("]\n");
			break;
		case BString:
			printf("%s\n", b->cargo.str.string);
			break;
		case BDict:
			printf("{\n");
			DictNode *d = b->cargo.dict;
			while (d) {
				print_indent(indent+1);
				printf("%s ==>\n", d->key);
				print_bencoding(d->value, indent+2);
				d = d->next;
			}
			print_indent(indent);
			printf("}\n");
			break;
		case Nothing:
			break;
	}
}


/*URL Encoder courtesy of GeekHideout*/

/* Converts a hex character to its integer value */
char from_hex(char ch) {
  return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

/* Converts an integer value to its hex character*/
char to_hex(char code) {
  static char hex[] = "0123456789abcdef";
  return hex[code & 15];
}

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_encode(char *str) {
  char *pstr = str, *buf = malloc(strlen(str) * 3 + 1), *pbuf = buf;
  while (*pstr) {
    if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') 
      *pbuf++ = *pstr;
    else if (*pstr == ' ') 
      *pbuf++ = '+';
    else 
      *pbuf++ = '%', *pbuf++ = to_hex(*pstr >> 4), *pbuf++ = to_hex(*pstr & 15);
    pstr++;
  }
  *pbuf = '\0';
  return buf;
}

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_decode(char *str) {
  char *pstr = str, *buf = malloc(strlen(str) + 1), *pbuf = buf;
  while (*pstr) {
    if (*pstr == '%') {
      if (pstr[1] && pstr[2]) {
        *pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
        pstr += 2;
      }
    } else if (*pstr == '+') { 
      *pbuf++ = ' ';
    } else {
      *pbuf++ = *pstr;
    }
    pstr++;
  }
  *pbuf = '\0';
  return buf;
}


/*
int main(int argc, char *argv[])
{
	printf("Making a new Bencoding\n");
	Bencoding* b = parse_bencoding("d8:announce14:www.google.com4:info5:iteste");
	print_bencoding(b, 0);
	return 0;
}
*/
