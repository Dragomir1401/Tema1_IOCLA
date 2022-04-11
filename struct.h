#define P_TYPE_1 "%" PRId8 "\n%" PRId8 "\n\n"
#define P_TYPE_2 "%" PRId16 "\n%" PRId32 "\n\n"
#define P_TYPE_3 "%" PRId32 "\n%" PRId32 "\n\n"

typedef struct head
{
	unsigned char type;
	unsigned int len;
} head;

typedef struct data_structure
{
	head *header;
	void *data;
} data_structure;
