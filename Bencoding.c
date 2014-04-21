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
	b->cargo.str = s;
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
	for (i = 0; i < len; ++i)
		s[i] = getChar();
	s[len] = 0;
//	printf("%s\n",s);
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
		c->key = s->cargo.str;
		free(s);
//		printf("Value: ");
		c->value = parse_bencoding();
		c->next = 0;
	}
	matchChar('e');
	return new_bdict(d.next);
}

Torrent* parse_start(char* input,long limit)
{
	buf_lim= (int)limit;
	int i =0;
	for(;i<buf_lim; i++){
		buf[i] = input[i];
	}
	printf("Parsing...\n");
	Bencoding *b =  parse_bencoding();
	print_bencoding(b,0);
	return parse_torrent(b);
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

Torrent* parse_torrent(Bencoding *b){
	Torrent *t = malloc(sizeof(Torrent));
	if(b->type != BDict){
		printf("Not a valid torrent bencode!\n");
		return NULL; 
	}
	DictNode *d = b->cargo.dict;
	DictNode *p = NULL;
	while(d != NULL){
		if(!strcmp(d->key,"announce")){
		  t->announce = d->value->cargo.str;
		  d = d->next;
		}
		else if(!strcmp(d->key,"name")){
		  t->name = d->value->cargo.str;
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
		else if(!strcmp(d->key,"pieces")){
		  t->pieces = d->value->cargo.str;
		  d = d->next;
		}
		else if(!strcmp(d->key,"path")){
		  t->path = d->value->cargo.str;
		  d = d->next;
		}
		else if(!strcmp(d->key,"url-list")){
		  t->url_list = d->value->cargo.str;
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
			printf("%s\n", b->cargo.str);
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
/*
int main(int argc, char *argv[])
{
	printf("Making a new Bencoding\n");
	Bencoding* b = parse_bencoding("d8:announce14:www.google.com4:info5:iteste");
	print_bencoding(b, 0);
	return 0;
}
*/
