
#define BUFSIZE 65535

typedef enum BType {
	Nothing = 0,
	BString,
	BInt,
	BList,
	BDict
} BType;

struct Bencoding;

typedef struct Torrent {
	char *announce;
	char *name;
	int piece_length;
	int length;
	char *pieces;
	char *path;
	char *url_list;
	char *info_hash;
} Torrent;

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
		char *str;      // used when type == BString
		DictNode *dict;
	} cargo; // data
} Bencoding;

Bencoding* parse_start(char* input, long limit);
Bencoding* parse_bencoding();
void print_bencoding(Bencoding *b, int indent);
Torrent* parse_torrent(char *input, long limit);
