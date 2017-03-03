#ifndef CC_TABLE_H
#define CC_TABLE_H

typedef struct Entry{
	void *key;
	void *val;
	struct Entry *next;
}Entry;

typedef unsigned int(*hash_fun)(void *key);
typedef int(*keyCompare)(void *key1, void *key2);
typedef void(*Freefun)(void *data);

typedef struct{
	hash_fun    hash;
	keyCompare  keycom;
	Freefun     keyFree;
	Freefun     valFree;
}table_type;

typedef struct{
	table_type        *type;
	Entry         **table;
	unsigned  int nums;
	unsigned  int nalloc;
}cc_table;

cc_table *create_table(table_type *type);
void* get_value(cc_table *ta,void *key);
int insert_entry(cc_table *ta, void *key, void *val);
int update_entry(cc_table *ta, void *key, void *val);
int delete_entry(cc_table *ta, void *key);
void destory_table(cc_table *ta);

#endif