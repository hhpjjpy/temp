#include "cc_table.h"
#include "hash.h"

#include <stdio.h>

int count=0;

unsigned int hash_test(void *data){
	return Hash((char*)data,sizeof(int));
}

int keycom(void *key1,void *key2){
	if (*((int*)key1) == *((int*)key2))
		return 0;
	return -1;
}

void keyfree(void *p){
	count++;
	printf("key free %d \n",*((int*)p));
}

void valfree(void *p){
	printf("val free %d \n", *((int*)p));
}

int key[10000];
int val[10000];

int main(){
	for (int i = 0; i < 10000;i++){
		key[i] = i;
		val[i] = i * 10;
	}

	table_type type;
	type.hash = hash_test;
	type.keycom = keycom;
	type.keyFree = keyfree;
	type.valFree = valfree;

	cc_table *table = create_table(&type);
	for (int i = 0; i < 10000; i++){
		insert_entry(table,&key[i],&val[i]);
	}

	for (int i = 9999;i>=0;i--) {
		void *p = get_value(table,&key[i]);
		if (p == NULL){
			printf("index %d  dot fond \n ",i);
			continue;
		}
		printf("index %d  , val %d \n",i,*((int*)p));
	}

	for (int i = 0; i < 9999;i++){
		delete_entry(table,&key[i]);
	}
	
	printf("next destory \n");
	
	destory_table(table);
      
        printf(" free count  %d \n",count);
	printf("run test ok \n");

}
