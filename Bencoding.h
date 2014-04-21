
#define BUFSIZE 65535

typedef enum BType {
	Nothing = 0,
	BString,
	BInt,
	BList,
	BDict
} BType;

struct Bencoding;

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

Bencoding* parse_start(char* input);
Bencoding* parse_bencoding();
void print_bencoding(Bencoding *b, int indent);

