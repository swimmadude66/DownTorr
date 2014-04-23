
#define BUFSIZE 65535

typedef enum BType {
	Nothing = 0,
	BString,
	BInt,
	BList,
	BDict
} BType;

struct Bencoding;

typedef struct str_t{
	char *string;
	int length;
} str_t;

typedef struct Torrent {
	char *announce;
	char *name;
	int piece_length;
	int length;
	int pieces_size;
	char *pieces;
	char *path;
	char *url_list;
	unsigned char info_hash[20];
} Torrent;

/*
typedef struct Torr_File {
	int length;
	char* path;
} Torr_File;
*/

typedef struct Peer {
    char *peer_id;
    char *ip;
    int port;
} Peer;

typedef struct Response {
    char *failure_reason;
    char *warning;
    int interval;
    int min_interval;
    char *tracker_id;
    int seeders;
    int leechers;
    Peer* peers;
} Response;




typedef struct ListNode {
	struct Bencoding *cargo;
	struct ListNode *next;
} ListNode;

typedef struct DictNode {
	char *key;
	struct Bencoding *value;
	struct DictNode *next;
} DictNode;

typedef struct Bencoding {
	BType type;
	union {
		long long val;  // used when type == BInt
		ListNode *list; // used when type == BList
		str_t str;      // used when type == BString
		DictNode *dict;
	} cargo; // data
} Bencoding;

Bencoding* parse_start(char* input, long limit);
Bencoding* parse_bencoding();
str_t *get_info_dict(char *input);
void print_bencoding(Bencoding *b, int indent);
Torrent* parse_torrent(char *input, long limit);
Response* parse_response(char *input, long limit);
void parse_peers(Response *r, ListNode *l);
char *url_encode(char *str);
