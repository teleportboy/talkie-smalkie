#ifndef ONLINE_USERS_H_DEFINED
#define ONLINE_USERS_H_DEFINED


typedef struct online_user {
	char* nickname;
	int	  socket;
} online_user;

typedef struct table_element {
	online_user  user;
	int			 is_empty;
	unsigned int hash;
} table_element;

typedef struct hash_table {
	table_element* table;
	int			   size;
} hash_table;

unsigned int karp_rabin(char* s);
hash_table   create_hash_table(int size);
unsigned int quadratic_probing_hash (
				unsigned int hashed_nick,
				int table_size,
				int quadratic
);

table_element ht_get_item(hash_table* hash_table, char* nickname);
int ht_insert(hash_table* hash_table, char* key, int value);

#endif