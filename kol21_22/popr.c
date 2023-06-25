#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PESEL_SIZE 12

typedef struct
{
    char *name;
    char *surname;
    char pesel[12];
} Person;

typedef struct
{
    Person **pdata;
    size_t size;
    size_t no_el;
} PersonList;

void init_pl(PersonList *p_pl)
{
    p_pl->pdata = malloc(sizeof(Person *) * 4);
    p_pl->size = 4;
    p_pl->no_el = 0;
}

void free_pl(PersonList *p_pl){
    for(int i=0;i<(p_pl->no_el);i++){
        //if((p_pl->pdata[i])!=NULL){
        free((p_pl->pdata[i])->name);        
        free((p_pl->pdata[i])->surname);
        free(p_pl->pdata[i]);
    }
    p_pl->no_el=0;
    //p_pl->size=4;
}
void fill_person_data(Person *p_p, const char *pesel, const char *name, const char *surname)
{
    for (int i = 0; i < PESEL_SIZE; i++)
    { // wiemy, że pesel zawsze ma 12 znaków (łącznie ze znakiem '\0')
        (p_p->pesel)[i] = pesel[i];
    }
    p_p->name = malloc(sizeof(name));
    p_p->surname = malloc(sizeof(surname));

    for (int i = 0; i < sizeof(surname); i++)
    {
        (p_p->surname)[i] = surname[i];
    }

    for (int i = 0; i < sizeof(name); i++)
    {
       ( p_p->name)[i] = name[i];
    }
}

void dump_person(Person *p)
{
    if(p==NULL){
        printf("NULL\n");
        return;
    }
    printf("%s %s %s\n", p->name, p->surname, p->pesel);
}

void dump_list(PersonList *pl)
{
    for (int i = 0; i < pl->no_el; i++)
    {
        dump_person(pl->pdata[i]);
    }
}

int add(PersonList *p_pl, const char *pesel, const char *name, const char *surname)
{
    for (int i = 0; i < p_pl->no_el; i++)
    {
        if (strcmp(p_pl->pdata[i]->pesel, pesel) == 0)
        {
            free(((p_pl->pdata)[i])->name);
            free(((p_pl->pdata)[i])->surname);
            fill_person_data(p_pl->pdata[i], pesel, name, surname);
            return 1;
        }
    }
    if (p_pl->size == p_pl->no_el)
    {
        p_pl ->pdata= realloc((p_pl->pdata), (p_pl->size) * 2);
        (p_pl->size) *= 2;
    }
    Person *new_person = malloc(sizeof(Person));
    fill_person_data(new_person, pesel, name, surname);
    p_pl->pdata[p_pl->no_el] = new_person;
    (p_pl->no_el)++;
    return 0;
}

Person *find(const PersonList *p_pl, const char *pesel)
{
    for(int i=0;i<p_pl->no_el;i++){
        if(strcmp(pesel,p_pl->pdata[i]->pesel)==0){
            return p_pl->pdata[i];
        }
    }
    return NULL;
}

void merge(PersonList*p_target,const PersonList*p_src){
    for(int i=0;i<p_src->no_el;i++){
        add(p_target,(p_src->pdata[i])->pesel,(p_src->pdata[i])->name,(p_src->pdata[i])->surname);
    }
}

int main()
{
    Person p_p;
    char pesel1[PESEL_SIZE];
    char surname1[20];
    char name1[20];
    scanf("%s %s %s", name1, surname1, pesel1);
    //printf("%s %s %s\n", name, surname, pesel);
    fill_person_data(&p_p, pesel1, name1, surname1);
    dump_person(&p_p);

    PersonList pl;
    init_pl(&pl);

    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        char pesel[PESEL_SIZE];
        char surname[20];
        char name[20];
        scanf("%s %s %s", name, surname, pesel);
        add(&pl, pesel, name, surname);
    }
    PersonList pl2;
    init_pl(&pl2);
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        char pesel[PESEL_SIZE];
        char surname[20];
        char name[20];
        scanf("%s %s %s", name, surname, pesel);
        add(&pl2, pesel, name, surname);
    }
    printf("%d",sizeof(name1));
    dump_list(&pl);
    Person *fnd=find(&pl,pesel1);
    printf("\n");

    dump_person(fnd);
    printf("\n");
    dump_list(&pl2);
    printf("\n");

    merge(&pl,&pl2);
    dump_list(&pl);
    free_pl(&pl);



}