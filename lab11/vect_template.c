#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

typedef struct Vector
{
	void *data;
	size_t element_size;
	size_t size;
	size_t capacity;
} Vector;

typedef struct Person
{
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

typedef int (*cmp_ptr)(const void *, const void *);
typedef int (*predicate_ptr)(void *);
typedef void (*read_ptr)(void *);
typedef void (*print_ptr)(const void *);
// print integer value
void print_int(const void *v)
{
	int val = *(int *)v;
	printf("%d ", val);
}

// print char value
void print_char(const void *v)
{
	char val = *(char *)v;
	printf("%c ", val);
}

// print structure Person
void print_person(const void *v)
{	
	Person *p=(Person *)v;
	printf("%d %s %s\n",p->age,p->first_name,p->last_name);
}

// print capacity of the vector and its elements
void print_vector(Vector *vector, print_ptr print)
{
	int n = vector->size;
	printf("%ld\n", vector->capacity);
	for (int i = 0; i < n; i++)
	{
		print((void *)((char *)vector->data + i * (vector->element_size)));
	}
	printf("\n");
}

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size)
{
	vector->data = malloc(block_size * element_size);
	vector->element_size = element_size;
	vector->size = 0;
	vector->capacity = block_size;
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity)
{
	if ((vector->capacity) < new_capacity)
	{
		vector->data = realloc(vector->data, vector->element_size * new_capacity);
		vector->capacity = new_capacity;
	}
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size)
{
	int cp = 0;
	if ((vector->size) < new_size)
	{
		reserve(vector, new_size);
		int n = new_size - vector->size;
		for (int i = 0; i < n; i++)
		{
			memcpy((void *)((char *)vector->data + (vector->size+i)*vector->element_size), &cp, vector->element_size);
		}
		vector->size = new_size;
	}
	else
	{
		vector->size = new_size;
	}
	//print_vector(vector, print_int);

}

// Add element to the end of the vector
void push_back(Vector *vector, void *value)
{
	if (vector->capacity <= vector->size)
	{
		vector->data = realloc(vector->data, (vector->element_size) * (vector->capacity) * 2);
		vector->capacity = 2 * (vector->capacity);
	}
	// print_vector(vector,print_int);
	memcpy((void *)((char *)vector->data + vector->size*vector->element_size), (const void *)value, vector->element_size);
	(vector->size) += 1;
	// printf("%ld ",vector->capacity);
	//print_vector(vector, print_int);
}

// Remove all elements from the vector
void clear(Vector *vector)
{
	vector->size = 0;
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, size_t index, void *value)
{
	if ((vector->capacity) == (vector->size))
	{
		vector->data = realloc(vector->data, (vector->element_size) * (vector->capacity) * 2);
		vector->capacity = 2 * (vector->capacity);
	}
	memmove((void *)((char *)vector->data + (index+1)*vector->element_size) , (void *)((char *)vector->data + (index)*vector->element_size),
			((vector->size) - index) * (vector->element_size));
	memcpy((void *)((char *)vector->data + (index)*vector->element_size), value, vector->element_size);
	vector->size += 1;
	//print_vector(vector, print_int);
}

// Erase element at position index
void erase(Vector *vector, size_t index)
{
	if ((vector->size) > 0)
	{
		memmove((void *)((char *)vector->data + (index)*vector->element_size), (void *)((char *)vector->data + (index+1)*vector->element_size), ((vector->size) - index) * (vector->element_size));

		(vector->size) -= 1;
	}
	//print_vector(vector, print_int);
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, cmp_ptr cmp)
{
	int i = 0;
	while (i < (vector->size))
	{
		if (cmp((void *)((char *)vector->data + (i)*vector->element_size), value) == 0)
		{
			erase(vector, i);
		}
		else
		{
			i++;
		}
	}
	//print_vector(vector, print_int);

}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *))
{
	int i = 0;
	while (i < (vector->size))
	{
		if (predicate((void *)((char *)vector->data + i*vector->element_size)))
		{
			erase(vector, i);
		}
		else
		{
			//printf("%d %d\n",((Person *)((char *)vector->data + i*vector->element_size))->age,i);
			i++;
		}
	}
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector)
{
	vector->capacity = vector->size;
	// free((vector->data)+(vector->size));
}

// integer comparator
int int_cmp(const void *v1, const void *v2)
{
	int val1 = *(int *)v1;
	int val2 = *(int *)v2;
	return (val1 - val2);
}

// char comparator
int char_cmp(const void *v1, const void *v2)
{
	char val1 = *(char *)v1;
	char val2 = *(char *)v2;
	return (val1 - val2);
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2) {
	Person *v1=(Person *)p1;
	Person *v2=(Person *)p2;
	if(v1->age!=v2->age){
		return v2->age-v1->age;
	}
	int a=strcmp(v1->first_name,v2->first_name);
	if(a!=0){
		return a;
	}
	return strcmp(v1->last_name,v2->last_name);

}

// predicate: check if number is even
int is_even(void *value)
{
	int *val = (int *)value;
	return ((*val) % 2 == 0);
}

// predicate: check if char is a vowel
int is_vowel(void *value)
{
	int vowels_num = 12;
	char vowels[] = {'a', 'o', 'i', 'e', 'u', 'y','A','O','I','E','U','Y'};
	char val = *(char *)value;
	//printf("%c\n",val);
	for (int i = 0; i < vowels_num; i++)
	{
		if (val == vowels[i])
		{
			return 1;
		}
	}
	return 0;
}

// predicate: check if person is older than 25
int is_older_than_25(void *person)
{
	int age = ((Person *)person)->age;
	return (age > 25);
}

// read int value
void read_int(void *value)
{
	scanf("%d", (int *)value);
	//printf("r: %d\n",*((int*)value));
}

// read char value
void read_char(void *value)
{
	scanf(" %c", (char *)value);
	//printf("r: %c\n",*((char*)value));

}

// read struct Person
void read_person(void *value)
{
	
	char line[MAX_STR_LEN];
	fgets(line,MAX_STR_LEN,stdin);
	sscanf(line," %d %s %s",&(((Person *)value)->age),((Person *)value)->first_name,((Person *)value)->last_name);
	//print_person(value);
	
}

void vector_test(Vector *vector, size_t block_size, size_t elem_size, int n, read_ptr read,
				 cmp_ptr cmp, predicate_ptr predicate, print_ptr print)
{
	init_vector(vector, block_size, elem_size);
	void *v = malloc(vector->element_size); // safe_malloc bez (void*)
	size_t index, size;
	for (int i = 0; i < n; ++i)
	{
		char op;
		scanf(" %c", &op);
		switch (op)
		{
		case 'p': // push_back
			read(v);
			push_back(vector, v);
			//print_vector(vector,print_int);
			break;
		case 'i': // insert
			scanf("%zu", &index);
			read(v);
			insert(vector, index, v);
			break;
		case 'e': // erase
			scanf("%zu", &index);
			erase(vector, index);
			break;
		case 'v': // erase
			read(v);
			erase_value(vector, v, cmp);
			break;
		case 'd': // erase (predicate)
			erase_if(vector, predicate);
			break;
		case 'r': // resize
			scanf("%zu", &size);
			resize(vector, size);
			break;
		case 'c': // clear
			clear(vector);
			break;
		case 'f': // shrink
			shrink_to_fit(vector);
			break;
		case 's': // sort
			qsort(vector->data, vector->size,
				  vector->element_size, cmp);
			break;
		default:
			printf("No such operation: %c\n", op);
			break;
		}
	}
	print_vector(vector, print);
	free(vector->data);
	free(v);
}

int main(void)
{
	int to_do, n;
	Vector vector_int, vector_char, vector_person;
	// Vector test;
	// test.size = 3;
	// test.element_size=sizeof(int);
	// test.capacity = 4;
	// int value=1;
	// memcpy((void *)((char *)test.data), (const void *)&value, test.element_size);
	// value=2;
	// memcpy((void *)((char *)test.data+1), (const void *)&value, test.element_size);
	// value=3;
	// memcpy((void *)((char *)test.data+2), (const void *)&value, test.element_size);

	// print_vector(&test, print_int);

	scanf("%d%d", &to_do, &n);

	switch (to_do)
	{
	case 1:
		vector_test(&vector_int, 4, sizeof(int), n, read_int, int_cmp,
					is_even, print_int);
		break;
	case 2:
		vector_test(&vector_char, 2, sizeof(char), n, read_char, char_cmp,
					is_vowel, print_char);
		break;
	case 3:
		vector_test(&vector_person, 2, sizeof(Person), n, read_person,
					person_cmp, is_older_than_25, print_person);
		break;
	default:
		printf("Nothing to do for %d\n", to_do);
		break;
	}

	return 0;
}
