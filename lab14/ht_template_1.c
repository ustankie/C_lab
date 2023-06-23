#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR (-1)
typedef struct DataWord
{
	char *word;
	int counter;
} DataWord;
typedef union
{
	int int_data;
	char char_data;
	// ... other primitive types used
	void *ptr_data;
} data_union;

typedef struct ht_element
{
	struct ht_element *next;
	data_union data;
} ht_element;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union *);
typedef int (*CompareDataFp)(data_union, data_union);
typedef size_t (*HashFp)(data_union, size_t);
typedef data_union (*CreateDataFp)(void *);

typedef struct
{
	size_t size;
	size_t no_elements;
	ht_element *ht;
	DataFp dump_data;
	CreateDataFp create_data;
	DataFp free_data;
	CompareDataFp compare_data;
	HashFp hash_function;
	DataPFp modify_data;
} hash_table;

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, size_t size, DataFp dump_data, CreateDataFp create_data,
			 DataFp free_data, CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data)
{

	p_table->size = size;
	p_table->dump_data = dump_data;
	p_table->create_data = create_data;
	p_table->free_data = free_data;
	p_table->compare_data = compare_data;
	p_table->hash_function = hash_function;
	p_table->modify_data = modify_data;
	p_table->no_elements = 0;
	p_table->ht = malloc(size * sizeof(ht_element));

	for (int i = 0; i < size; i++)
	{
		((p_table->ht)[i]).next = NULL;
		// printf("%zu\n",((p_table->ht)+i)->next);
	}
}

// print elements of the list with hash n
void dump_list(const hash_table *p_table, size_t n)
{

	ht_element *ptr = ((p_table->ht)[n]).next;
	// printf("%zu\n",ptr);
	while (ptr != NULL)
	{
		p_table->dump_data(ptr->data);
		ptr = ptr->next;
	}
	printf("\n");
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete)
{
	ht_element *ptr = to_delete;
	ptr = ptr->next;
	if (free_data == NULL)
	{
		return;
	}
	while (ptr != NULL)
	{
		free_data(ptr->data);
		ptr = ptr->next;
	}


}

// free all elements from the table (and the table itself)
void free_table(hash_table *p_table)
{
	for (int i = 0; i < p_table->size; i++)
	{
		free_element(p_table->free_data, (p_table->ht) + i);
	}
	free(p_table->ht);
}

// calculate hash function for integer k
size_t hash_base(int k, size_t size)
{
	static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
	double tmp = k * c;
	return (size_t)floor((double)size * (tmp - floor(tmp)));
}

void rehash(hash_table *p_table)
{
	ht_element *new_ht = malloc((p_table->size) * 2 * sizeof(ht_element));
	for (int i = 0; i < (p_table->size) * 2; i++)
	{
		(new_ht[i]).next = NULL;
		// printf("%zu\n",((new_ht)+i)->next);
	}

	size_t hash;
	ht_element *new;
	for (int i = 0; i < p_table->size; i++)
	{
		ht_element *ptr = ((p_table->ht)[i]).next;
		//ptr = ptr->next;
		while (ptr != NULL)
		{
			hash = p_table->hash_function(ptr->data, (p_table->size) * 2);
			// printf("h: %d ",hash);
			new = ptr;
			ptr = ptr->next;
			new->next = ((new_ht)[hash]).next;
			((new_ht)[hash]).next = new;
		}
	}
	free(p_table->ht);
	p_table->ht = new_ht;
	(p_table->size) = (p_table->size) * 2;
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data)
{
	size_t n = p_table->hash_function(data, p_table->size);
	ht_element *ptr = &((p_table->ht)[n]);
	ht_element *prev = ptr;
	ptr = ptr->next;
	ht_element *prev_prev = prev;
	while (ptr != NULL && p_table->compare_data(ptr->data, data) != 0)
	{   
		prev_prev = prev;
		prev = ptr;
		ptr = ptr->next;
	}
    
	if (ptr == NULL)
	{
		return prev_prev;
	}
	return prev;
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data)
{
	size_t hash = p_table->hash_function(*data, p_table->size);
	ht_element *ptr = ((p_table->ht)[hash]).next;//&!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//printf("%s %s\n",((DataWord *)(data->ptr_data))->word,((DataWord *)data->ptr_data)->word);
	//printf("%d",strcmp(((DataWord *)(data->ptr_data))->word,((DataWord *)data->ptr_data)->word));
	while (ptr != NULL )
	{	
		if(p_table->compare_data(*data, ptr->data) == 0)
			return ptr;
		ptr = ptr->next;
	}
	return ptr;
}

// insert element
void insert_element(hash_table *p_table, data_union *data)
{
	if (get_element(p_table, data) != NULL)
	{	

		return;
	}

	size_t hash = p_table->hash_function(*data, p_table->size);
	// p_table->dump_data(*data);
	// printf("%d\n", hash);
	ht_element *new = malloc(sizeof(ht_element));
	new->data = *data;
	new->next = ((p_table->ht) + hash)->next;
	((p_table->ht) + hash)->next = new;
	p_table->no_elements++;

	if (((p_table->no_elements) / (p_table->size)) > MAX_RATE)
	{
		rehash(p_table);
	}
    // for (int i = 0; i < p_table->size; i++)
    //     {
    //         printf("%d: ", i);
    //         dump_list(p_table, i);
    //     }

}

// remove element
void remove_element(hash_table *p_table, data_union data)
{
	ht_element *prev = find_previous(p_table, data);
	if (p_table->compare_data(prev->next->data, data) == 0)
	{
		if (p_table->free_data != NULL)
		{
			p_table->free_data(prev->next->data);
		}

		prev->next = prev->next->next;
	}
}

// type-specific definitions

// int element

size_t hash_int(data_union data, size_t size)
{
	return hash_base(data.int_data, size);
}

void dump_int(data_union data)
{
	printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b)
{
	return a.int_data - b.int_data;
}

data_union create_int(void *value)
{
	data_union *ptr = malloc(sizeof(data_union));
	data_union b;
	int a;
	scanf("%d", &a);
	ptr->int_data = a;
	b.int_data = a;
	if (value != NULL)
	{
		value = ptr;
	}
	return b;
}

// char element

size_t hash_char(data_union data, size_t size)
{
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data)
{
	printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b)
{
	return (int)a.char_data - (int)b.char_data;
}

data_union create_char(void *value)
{
	data_union *ptr = malloc(sizeof(data_union));
	data_union b;
	char a;
	scanf(" %c", &a);
	ptr->char_data = a;
	b.char_data = a;
	if (value != NULL)
	{
		value = ptr;
	}
	// printf("b: %c\n",b.char_data);
	return b;
}

// Word element



void dump_word(data_union data)
{
	char *a = ((DataWord *)data.ptr_data)->word;
	printf("%s %d\n", a,((DataWord *)data.ptr_data)->counter);
}

void free_word(data_union data)
{   
    free(((DataWord *)data.ptr_data)->word);
	free((DataWord *)data.ptr_data);
}

int cmp_word(data_union a, data_union b)
{
	
	char *v1=((DataWord*)(a.ptr_data))->word;
	//printf("cmp_word: %s ",v1);
	char *v2=((DataWord*)(b.ptr_data))->word;
	//printf("cmp_word: %s ",v1);
	// char s1=v1->word;
	// char s2=v2->word;
	//return -1;
	//printf("cmp_word: %s %s \n",v1,v2);
	// printf("%d\n\n",strcmp(v1,v2));
	return strcmp(v1,v2);
}

size_t hash_word(data_union data, size_t size)
{
	int s = 0;
	DataWord *dw = (DataWord *)data.ptr_data;
	char *p = dw->word;
	while (*p)
	{
		s += *p++;
	}
	return hash_base(s, size);
}

void modify_word(data_union *data)
{
	(((DataWord *)(data->ptr_data))->counter)++;
}

data_union create_data_word(void *value)
{
	data_union *ptr = malloc(sizeof(data_union));
	DataWord *dword = malloc(sizeof(DataWord));
	ptr->ptr_data = (void *)dword;
	dword->word = strdup(value);
	dword->counter = 1;


	return *ptr;
}

// read text, parse it to words, and insert these words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream)
{
	data_union tmp;
	//data_union data;
	//int Flag=0;
	char delimiters[] = " \n\t\r\v\f.,?!:;-";
	char buff[BUFFER_SIZE];

	while (fgets(buff, BUFFER_SIZE, stream))
	{
		char *word = strtok(buff, delimiters);
		while (word != NULL)
		{

			char *letter = word;
			while (*letter != '\0')
			{
				*letter = tolower(*letter);
				letter++;
			}
			
			
			//data.ptr_data=tmp.ptr_data;
						
			tmp = create_data_word(word);
            //dump_word(tmp);


			// if(Flag){printf(" word: %s %zu ", ((DataWord *)tmp.ptr_data)->word,&(((DataWord *)tmp.ptr_data)->word));
			// 	printf("\ncword: %s %zu ", ((DataWord *)data.ptr_data)->word,&(((DataWord *)data.ptr_data)->word));
			// 	printf(" word: %s %zu ", ((DataWord *)tmp.ptr_data)->word,&(((DataWord *)tmp.ptr_data)->word));
			// 	printf("\ncword: %s %zu ", ((DataWord *)data.ptr_data)->word,&(((DataWord *)data.ptr_data)->word));
				
			// 	printf("cmp: %d \n",p_table->compare_data(tmp,data));
			// }
			// Flag=1;
			
			ht_element *position = get_element(p_table, &tmp);
            //printf("pos: %d %d\n",position, p_table->size);
        //     //dump_list(p_table, 7);
        //     for (int i = 0; i < p_table->size; i++)
        // {
        //     printf("%d: ", i);
        //     dump_list(p_table, i);
        // }
			if (position == NULL)
			{
				insert_element(p_table, &tmp);
			}
			else
			{
				if (p_table->free_data != NULL)
				{
					(p_table->free_data)(tmp);
				}
				modify_word(&(position->data));
			}


			word = strtok(NULL, delimiters);
		}
	}
}

// test primitive type list
void test_ht(hash_table *p_table, int n)
{
	char op;
	data_union data;
	for (int i = 0; i < n; ++i)
	{
		scanf(" %c", &op);
		//		p_table->create_data(&data);
		data = p_table->create_data(NULL); // should give the same result as the line above
		switch (op)
		{
		case 'r':
			remove_element(p_table, data);
			break;
		case 'i':
			insert_element(p_table, &data);
			break;
		default:
			printf("No such operation: %c\n", op);
			break;
		}
	}
}

int main(void)
{
	int to_do, n;
	size_t index;
	hash_table table;
	char buffer[BUFFER_SIZE];
	data_union data;

	scanf("%d", &to_do);
	switch (to_do)
	{
	case 1: // test integer hash table
		scanf("%d %zu", &n, &index);
		init_ht(&table, 4, dump_int, create_int, NULL, cmp_int, hash_int, NULL);
		test_ht(&table, n);
		printf("%zu\n", table.size);
		dump_list(&table, index);
		break;
	case 2: // test char hash table
		scanf("%d %zu", &n, &index);
		init_ht(&table, 4, dump_char, create_char, NULL, cmp_char, hash_char, NULL);
		test_ht(&table, n);
		printf("%zu\n", table.size);
		dump_list(&table, index);
		break;
	case 3: // read words from text, insert into hash table, and print
		scanf("%s", buffer);
		init_ht(&table, 8, dump_word, create_data_word, free_word, cmp_word, hash_word, modify_word);
		stream_to_ht(&table, stdin);
		printf("%zu\n", table.size);
		data = table.create_data(buffer);
		ht_element *e = get_element(&table, &data);
		if (e)
			table.dump_data(e->data);
		if (table.free_data)
			table.free_data(data);
		break;
	default:
		printf("NOTHING TO DO FOR %d\n", to_do);
		break;
	}
	free_table(&table);

	return 0;
}
