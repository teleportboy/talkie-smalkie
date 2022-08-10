#include <stdio.h>
#include <stdlib.h>

#include "online_users_hashtable.h"

hash_table create_hash_table(int size) {
	hash_table hash_table;
	hash_table.table = malloc(sizeof(table_element) * size);
	hash_table.size  = size;

	for (int i = 0; i < size; i++) {
		hash_table.table[i].is_empty	  = 1;
		hash_table.table[i].user.nickname = "-1-empty-1-";
		hash_table.table[i].user.socket   = -1;
		hash_table.table[i].hash          = -1;
	}

	return hash_table;
}

unsigned int linear_probing_hash(unsigned int hashed_nick, const int table_size, int linear) {
	unsigned int hash = (hashed_nick % table_size + linear) % table_size;
	return hash;
}

int ht_insert(hash_table* hash_table, char* key, int value) {
	unsigned int hashed_nick = karp_rabin(key);
	unsigned int hash		 = hashed_nick % hash_table->size;

	if (hash_table->table[hash].is_empty) {
		hash_table->table[hash].user.nickname = key;
		hash_table->table[hash].user.socket   = value;

		hash_table->table[hash].hash	 = hashed_nick;
		hash_table->table[hash].is_empty = 0;
		return 1;
	}

	for (int i = 0; i < hash_table->size; i++) {
		hash = linear_probing_hash(hashed_nick, hash_table->size, i);
		if (hash_table->table[hash].is_empty) {
			hash_table->table[hash].user.nickname = key;
			hash_table->table[hash].user.socket   = value;

			hash_table->table[hash].hash	 = hashed_nick;
			hash_table->table[hash].is_empty = 0;
			return 1;
		}
	}

	return 0;
}

table_element ht_get_item(hash_table* hash_table, char* nickname) {
	unsigned int hashed_nick = karp_rabin(nickname);
	unsigned int hash        = hashed_nick % hash_table->size;

	int result = hash_table->table[hash].hash == hashed_nick 
		&& !hash_table->table[hash].is_empty ?
		hash_table->table[hash].user.socket : -1;

	if (result != -1) {
		return hash_table->table[hash];
	}

	for (int i = 0; i < hash_table->size; i++) {
		hash = linear_probing_hash(hashed_nick, hash_table->size, i);

		if (hash_table->table[hash].hash == hash) {
			return hash_table->table[hash];
		}

		if (hash_table->table[hash].is_empty) {
			return hash_table->table[hash];
		}
	}

	return hash_table->table[hash];
}

unsigned int karp_rabin(char* s) {
	static const unsigned int p = 54351;
	static const unsigned int x = 43451;
	unsigned int h = 1;
	for (int i = 0; s[i] != 0; i++) {
		h = ((unsigned long long) h * x + s[i]) % p;
	}
	return h;
}