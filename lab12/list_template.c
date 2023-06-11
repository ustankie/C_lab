#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

struct List;
typedef void (*DataFp)(void *);
typedef void (*ConstDataFp)(const void *);
typedef int (*CompareDataFp)(const void *, const void *);

typedef struct ListElement
{
	struct ListElement *next;
	void *data;
} ListElement;

typedef struct
{
	ListElement *head;
	ListElement *tail;
	ConstDataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	DataFp modify_data;
} List;

void *safe_malloc(size_t size)
{
	void *ptr = malloc(size);
	if (ptr)
		return ptr;
	printf("malloc error\n");
	exit(EXIT_FAILURE);
}

void *safe_strdup(const char *string)
{
	void *ptr = strdup(string);
	if (ptr)
		return ptr;
	printf("strdup error\n");
	exit(EXIT_FAILURE);
}
// -----------------------------------------------------------
// --- type-specific definitions

// int element

void dump_int(const void *d)
{
	int val = *(int *)d;
	printf("%d ", val);
}

void free_int(void *d)
{
	free(d);
}

int cmp_int(const void *a, const void *b)
{
	int v1 = *(int *)a;
	int v2 = *(int *)b;
	return v1 - v2;
}

int *create_data_int(int v)
{
	int *data = malloc(sizeof(int));
	*data = v;
	return data;
}

// Word element

typedef struct DataWord
{
	char *word;
	int counter;
} DataWord;

void dump_word(const void *d)
{
	char *w=((DataWord *)d)->word;
	while (*w!='\0'){
		printf("%c",*w);
		w++;
	}
	printf(" ");
}
void dump_word_lowercase(const void *d){
	char *w=((DataWord *)d)->word;
	while (*w!='\0'){
		*w=tolower(*w);
		printf("%c",*w);
		w++;
	}
	printf(" ");

}
void free_word(void *d)
{
	//char v=(char *)d;
	free(d);
}

int cmp_word_alphabet(const void *a, const void *b)
{
	char *v1=((DataWord *)a)->word;
	char *v2=((DataWord *)b)->word;

	char w1[BUFFER_SIZE];
	char w2[BUFFER_SIZE];
	char c;
	int i=0;
	while (*v1!='\0'){
		c=tolower(*v1);
		w1[i]=c;
		i++;
		v1++;
	}
	w1[i]='\0';
	i=0;
	while (*v2!='\0'){
		c=tolower(*v2);
		w2[i]=c;
		i++;
		v2++;
	}
	
	w2[i]='\0';
	//printf("%s %s %d\n",w1,w2,strcmp(w1,w2));

	
	return strcmp(w1,w2);
}

int cmp_word_counter(const void *a, const void *b)
{
	int v1=((DataWord *)a)->counter;
	int v2=((DataWord *)b)->counter;
	return v1-v2;
}

void modify_word(void *p)
{
	DataWord *a=(DataWord *)p;
	a->counter=a->counter+1;
}

void *create_data_word(const char *string, int counter)
{
	// char delimiters={" ","!","?",".",":",";","-"};

	// char buff[BUFFER_SIZE];
	// fgets(buff,BUFFER_SIZE,stdin);

	// char *word;
	// word=strtok(buff,delimiters);

	DataWord *data = safe_malloc(sizeof(DataWord));
	data->word = string;
	data->counter = counter;
	return (void *)data;
}
// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List *p_list, FILE *stream, CompareDataFp cmp)
{
	char delimiters[] = " \n\t\r\v\f.,?!:;-"; //{" ","!","?",".",":",";","-"};

	char buff[BUFFER_SIZE];
	char *words[BUFFER_SIZE];
	void *data;

	int i = 0;
	while (fgets(buff, sizeof(buff), stream))
	{
		char *word = strtok(buff, delimiters);

		while (word != NULL)
		{
			words[i++] = strdup(word);
			word = strtok(NULL, delimiters);
		}


	}

	for (int j = 0; j < i; j++)
	{
		data = create_data_word(words[j], 1);
		if(cmp==NULL){
			push_back(p_list, data);
		}
		else{
			p_list->compare_data=cmp;
			//printf("%d\n",cmp(((DataWord *)data)->word,"ala"));

			insert_in_order(p_list,data);
		}
		//dump_list(p_list);

	}
	//dump_list(p_list);
}
// --- generic functions --- for any data type

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
			   CompareDataFp compare_data, DataFp modify_data)
{
	p_list->dump_data = dump_data;
	p_list->free_data = free_data;
	p_list->compare_data = compare_data;
	p_list->modify_data = modify_data;

	p_list->head = NULL;
	p_list->tail = p_list->head;
}

// Print elements of the list
void dump_list(const List *p_list)
{
	ListElement *ptr = p_list->head;
	while (ptr != NULL)
	{
		p_list->dump_data(ptr->data);
		//printf(": %d ",((DataWord *)(ptr->data))->counter);
		ptr = ptr->next;
	}
	printf("\n");
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data)
{
	ListElement *ptr=p_list->head;
	while(ptr!=NULL){
		if(p_list->compare_data(ptr->data,data)==0){
			p_list->dump_data(ptr->data);
		}
		ptr=ptr->next;
	}
}

// Free element pointed by data using free_data() function
void free_element(DataFp free_data, ListElement *to_delete)
{
	free_data(to_delete);
}

// Free all elements of the list
void free_list(List *p_list)
{
	ListElement *ptr=p_list->head;
	while(ptr!=p_list->tail){
		p_list->free_data((void *)ptr->data);
		ptr=ptr->next;
	}
	p_list->head = NULL;
	p_list->tail=NULL;
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data)
{
	ListElement *new_head = safe_malloc(sizeof(ListElement));
	new_head->data = data;
	if (p_list->head == NULL)
	{
		p_list->head = new_head;
		p_list->tail = p_list->head;
		p_list->tail->next = NULL;
	}
	else
	{
		new_head->next = p_list->head;
		p_list->head = new_head;
	}
}

// Push element at the end of the list
void push_back(List *p_list, void *data)
{

	ListElement *new_tail;
	new_tail = safe_malloc(sizeof(ListElement));
	new_tail->data = data;
	// dump_int(data);
	if (p_list->head == NULL)
	{
		p_list->head = new_tail;
		p_list->tail = p_list->head;
		p_list->tail->next = NULL;
	}
	else
	{
		p_list->tail->next = new_tail;
		new_tail->next = NULL;
		p_list->tail = new_tail;
	}
}

// Remove the first element
void pop_front(List *p_list)
{
	p_list->head = p_list->head->next;
}

// Reverse the list
void reverse(List *p_list)
{
	ListElement *p = p_list->head;
	ListElement *q = p_list->head;
	ListElement *tmp;
	ListElement *old_head;
	if (p != NULL)
	{
		p = p->next;
		old_head = q;
		while (p != NULL)
		{
			tmp = p->next;
			p->next = q;
			q = p;
			p = tmp;
		}
		p_list->head = q;
		p_list->tail = old_head;
		p_list->tail->next = NULL;
	}
}

// find element in sorted list after which to insert given element
ListElement *find_insertion_point(const List *p_list, ListElement *p_element)
{
	ListElement *ptr = p_list->head;
	ListElement *prev = ptr;

	while (ptr != p_list->tail && p_list->compare_data(ptr->data, p_element->data) < 0)
	{
		prev = ptr;
		ptr = ptr->next;
	}

	if (ptr==NULL || p_list->compare_data(ptr->data, p_element->data) > 0)
		return prev;
	return ptr;
}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous)
{
	ListElement *new_element = safe_malloc(sizeof(ListElement));
	new_element->data = data;
	new_element->next = previous->next;
	previous->next = new_element;
}

// Insert element preserving order
void insert_in_order(List *p_list, void *p_data)
{
	ListElement *new_element = safe_malloc(sizeof(ListElement));
	new_element->data = p_data;
	if(p_list->head==NULL){
		//printf("NULL\n");
		push_back(p_list, p_data);
	}
	else
	{
		ListElement *previous;
		previous= find_insertion_point(p_list, new_element);
		if (p_list->compare_data(previous->data, p_data) == 0)
		{
			if (p_list->modify_data != NULL)
			{
				p_list->modify_data(previous->data);
			}
		}
		else
		{
			if (previous == p_list->tail&& p_list->compare_data(previous->data, p_data) < 0)
			{
				push_back(p_list, p_data);
			}
			else if (previous == p_list->head && p_list->compare_data(previous->data, p_data) > 0)
			{
				push_front(p_list, p_data);
			}
			else
			{
				push_after(p_list, p_data, previous);
			}
		}
	}

	free(new_element);
}

// test integer list
void list_test(List *p_list, int n)
{
	char op;
	int v;
	for (int i = 0; i < n; ++i)
	{
		scanf(" %c", &op);
		switch (op)
		{
		case 'f':
			scanf("%d", &v);
			push_front(p_list, create_data_int(v));
			// dump_list(p_list);
			break;
		case 'b':
			scanf("%d", &v);
			push_back(p_list, create_data_int(v));
			// dump_list(p_list);
			break;
		case 'd':
			pop_front(p_list);
			// dump_list(p_list);
			break;
		case 'r':
			reverse(p_list);
			// dump_list(p_list);
			break;
		case 'i':
			scanf("%d", &v);
			insert_in_order(p_list, create_data_int(v));
			// dump_list(p_list);
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
	//ConstDataFp dump_word_lowercase; // error
	List list;

	scanf("%d\n", &to_do);
	switch (to_do)
	{
	case 1: // test integer list
		scanf("%d", &n);
		init_list(&list, dump_int, free_int, cmp_int, NULL);
		list_test(&list, n);
		dump_list(&list);
		free_list(&list);
		break;
	case 2: // read words from text, insert into list, and print
		init_list(&list, dump_word, free_word, NULL, NULL);
		stream_to_list(&list, stdin, NULL);
		dump_list(&list);
		free_list(&list);
		break;
	case 3: // read words, insert into list alphabetically, print words encountered n times
		scanf("%d", &n);
		init_list(&list, dump_word_lowercase, free_word, NULL, modify_word);
		stream_to_list(&list, stdin, cmp_word_alphabet);
		list.compare_data = cmp_word_counter;
		DataWord data = {NULL, n};
		dump_list_if(&list, &data);
		free_list(&list);
		break;
	default:
		printf("NOTHING TO DO FOR %d\n", to_do);
		break;
	}
	return 0;
}
