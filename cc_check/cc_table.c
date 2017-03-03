#include "cc_table.h"

#include <malloc.h>
#include <string.h>

cc_table *create_table(table_type *type){
	cc_table *table = (cc_table*)malloc(sizeof(cc_table));
	table->nalloc = 16;
	table->type = type;
	table->nums = 0;
	table->table = malloc(table->nalloc *sizeof(void*));
	memset(table->table,0,table->nalloc *sizeof(void*));

	return table;
}

Entry *find_entry(cc_table *ta,void *key){
	Entry *p;
	int index = (ta->type->hash)(key)&(ta->nalloc - 1);
	p = ta->table[index];
	while (p){
		if (ta->type->keycom(key, p->key) == 0)
			return p;
		p = p->next;
	}
	return NULL;
}

void *get_value(cc_table *ta,void *key){
	Entry *p = find_entry(ta, key);
	if (p != NULL){
		return p->val;
	}
	return NULL;
}

void  expand(cc_table *ta,int nalloc){
	Entry **new_table = malloc(nalloc*sizeof(void*));
	for (int i = 0; i < ta->nalloc;i++){
		Entry *p = ta->table[i];
		while (p){
			ta->table[i] = p->next;
			int index = (ta->type->hash(p->key))&(nalloc - 1);
			Entry *temp = new_table[index];
			new_table[index] = p;
			p->next = temp;

			p = ta->table[i];
		}
	}

	Entry **old = ta->table;
	ta->table = new_table;
	free(old);
	ta->nalloc = nalloc;
}

int insert_entry(cc_table *ta,void *key, void *val){
	if (ta->nalloc * 10 < ta->nums){
		expand(ta,2*ta->nalloc);
	}
	Entry *p = (Entry*)malloc(sizeof(Entry));
	p->key = key;
	p->val = val;
	p->next = NULL;

	int index = (ta->type->hash(key))%(ta->nalloc - 1);
	Entry *temp = ta->table[index];
	ta->table[index] = p;
	p->next = temp;
	ta->nums++;
}

int update_entry(cc_table *ta,void *key, void *val){
	Entry *p = find_entry(ta,key);
	if (p == NULL){
		return -1;//¸üÐÂÊ§°Ü
	}
	ta->type->valFree(p->val);
	p->val = val;

	return 0;
}

int delete_entry(cc_table *ta,void *key){
	Entry **p;
	int index = (ta->type->hash)(key)&(ta->nalloc - 1);
	p = &(ta->table[index]);
	while (*p){
		if (ta->type->keycom(key, (*p)->key) == 0){
			Entry *free_entry = *p;
			*p = (*p)->next;

			ta->type->keyFree(free_entry->key);
			ta->type->valFree(free_entry->val);
			free(free_entry);
		
			return 0;
		}

		p = &((*p)->next);
	}

	return -1;
}

void destory_table(cc_table *ta){
	for (int i = 0; i < ta->nalloc; i++){
		Entry *p = ta->table[i];
		while (p){
			ta->table[i] = p->next;
			
			ta->type->keyFree(p->key);
			ta->type->valFree(p->val);
			free(p);

			p = ta->table[i];
		}
	}

	free(ta->table);
	free(ta);
}



