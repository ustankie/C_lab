#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    int day;
    int month;
    int year;
} Date;

// 1 bsearch2

#define FOOD_MAX 30   // max. number of goods
#define ART_MAX 15    // max. length of name's name + 1
#define RECORD_MAX 40 // max. line length

typedef struct
{
    char name[20];
    float price;
    int amount;
    Date valid_date;
} Food;

typedef float (*ComparFp)(const void *, const void *);

// compare dates
int cmp_date(const void *d1, const void *d2)
{ // printf("a\n");
    Food *f1 = (Food *)d1;
    Food *f2 = (Food *)d2;
    Date date1 = (f1->valid_date);
    Date date2 = (f2->valid_date);

    if (date1.year != date2.year)
    {
        return (date1.year - date2.year);
    }
    if (date2.month != date1.month)
    {
        return (date1.month - date2.month);
    }
    return (date1.day - date2.day);
}

// compare foods
float cmp(const void *a, const void *b)
{
    float l1 = ((Food *)a)->price;
    float l2 = ((Food *)b)->price;
    float f = (l1 - l2);
    // printf("price: %f",f);
    if (f == 0)
    {
        return cmp_date(a, b);
    }

    return f;
}

// bsearch returning address where to insert new element
void *bsearch2(const void *key, const void *base, size_t nmemb, size_t size, ComparFp compar, char *result)
{
    char *tab = (char *)base;
    char *el = (char *)key;

    char *l = tab;
    char *r = tab + (nmemb)*size;

    char *q;

    printf("%p %p\n", l, q);

    if (nmemb == 0)
    {
        result = 0;
        return (void *)tab;
    }

    while (l < r)
    {
        int diff = (((r - l) / size) / 2) * size;
        // printf("diff: %d\n",diff);
        q = l + (diff);
        // printf("%p %p %p\n",l,q,r);
        float res = compar((void *)el, (void *)q);
        if (res < 0)
        {
            r = q - size;
        }
        else if (res > 0)
        {
            l = q + size;
        }
        else
        {
            printf("res1:%f\n", res);
            *result = 1;
            return (void *)q;
        }
    }
    if(l==r && compar((void *)el, (void *)l)==0){
        *result = 1;
    }
    printf("%d %p %p\n", size, l, r);
    if (l > (tab + nmemb * size))
    {
        q = tab + nmemb * size;
        //result = 0;
        return (void *)l;
    }

    
    return (void *)l;
}

// print goods of given name
void print_art(Food *p, int n, char *art)
{ 
    for (int i = 0; i < n; i++)
    {   Food *new =(p+i);
    
        int a=(strcmp(new->name,art));

        if(a==0)
        {
            
           printf("%lf %d %d.%d.%d\n", new->price, new->amount, new->valid_date.day, new->valid_date.month, new->valid_date.year);
        }
    }
}
void print_tab(Food *p, int n)
{
    for (int i = 0; i < n; i++)
    {

        {
            Food new = p[i];
            printf("%s %lf %d %d.%d.%d\n", new.name, new.price, new.amount, new.valid_date.day, new.valid_date.month, new.valid_date.year);
        }
    }
}

// add record to table if absent
Food *add_record(Food *tab, int *np, ComparFp compar, Food *new)
{
    char result = 0;
    Food *addr = (Food *)bsearch2(new, tab, (size_t)*np, sizeof(Food), cmp, &result);
    printf("result: %d\n",result);
    // if (addr < (tab + *np) && cmp((void *)new, (void *)addr) > 0)
    // {
    //     addr += 1;
    // }
    //printf("%d\n",(addr->name)>(new->name));
    if (result )
    {
        printf("%s %s\n",addr->name,new->name);
        (addr->amount) += new->amount;
    }
    else // if (((addr - tab) / sizeof(Food)) < FOOD_MAX)
    {
        Food *p = tab + *np;
        // addr+=1;

        printf("\nw miejsce:%s %lf %d %d.%d.%d  dodaj: %s %lf %d %d.%d.%d\n", addr->name, addr->price, addr->amount, addr->valid_date.day, addr->valid_date.month, addr->valid_date.year, new->name, new->price, new->amount, new->valid_date.day, new->valid_date.month, new->valid_date.year);
        for (Food *i = p; i > (addr); i -= 1)
        {
            // printf("%s %s\n",i->name,new->name);
            // printf("%p\n",i);
            *i = *(i - 1);
        }
        *addr = *new;
        (*np)++;
        // printf("\n%d\n\n",*np);
        printf("\n\n");
        print_tab(tab, *np);
        printf("\n\n");
    }
}

// read no lines of data
// calls add_record if sorted = 1 and just adds element if sorted = 0
int read_goods(Food *tab, int no, FILE *stream, int sorted)
{
    Food new;
    int np = 0;
    char buff[RECORD_MAX];
    for (int i = 0; i < no; ++i)
    {
        fgets(buff, RECORD_MAX, stream);
        sscanf(buff, "%s %f %d %d.%d.%d", new.name, &new.price, &new.amount, &new.valid_date.day, &new.valid_date.month, &new.valid_date.year);
        // printf("%s %f %d %d %d %d\n", new.name, new.price, new.amount,  new.valid_date.day,new.valid_date.month,new.valid_date.year);
        if (sorted)
        {
            add_record(tab, &np, cmp, &new);
        }
        else
        {
            tab[np] = new;
            np += 1;
        }
    }
    printf("%d %s\n", np, tab->name);
    print_tab(tab, np);
    return np;
}

int cmp_qs(const void *a, const void *b)
{
    Food *fa = (Food *)a, *fb = (Food *)b;
    return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void *a, const void *b)
{
    Date *pd = (Date *)a;
    Food *fb = (Food *)b;
    return cmp_date(pd, &fb->valid_date);
}

// finds the value of goods due on 'curr_date'
float value(Food *food_tab, size_t n, Date curr_date, int days)
{
}

/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex
{
    F,
    M
};
enum BOOL
{
    no,
    yes
};

struct Bit_data
{
    enum Sex sex : 1;
    enum BOOL in_line : 1;
};

typedef struct
{
    char *name;
    struct Bit_data bits;
    Date born;
    char *parent;
} Person;

typedef struct
{
    char *par_name;
    int index;
} Parent;

const Date primo_date = {28, 10, 2011}; // new succession act

int cmp_primo(Person *person1, Person *person2)
{
    if (person1->bits.sex == person2->bits.sex)
        return 0;
    if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0)
        return 0;
    if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0)
        return 0;
    return person2->bits.sex - person1->bits.sex;
}

// compare persons
int cmp_person(const void *a, const void *b)
{
}

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size)
{
}

void persons_shifts(Person *person_tab, int size, Parent *idx_tab, int no_parents)
{
}

int cleaning(Person *person_tab, int n)
{
}

void print_person(const Person *p)
{
    printf("%s\n", p->name);
}

int create_list(Person *person_tab, int n)
{
}

int main(void)
{
    Person person_tab[] = {
        {"Charles III", {M, no}, {14, 11, 1948}, "Elizabeth II"},
        {"Anne", {F, yes}, {15, 8, 1950}, "Elizabeth II"},
        {"Andrew", {M, yes}, {19, 2, 1960}, "Elizabeth II"},
        {"Edward", {M, yes}, {10, 3, 1964}, "Elizabeth II"},
        {"David", {M, yes}, {3, 11, 1961}, "Margaret"},
        {"Sarah", {F, yes}, {1, 5, 1964}, "Margaret"},
        {"William", {M, yes}, {21, 6, 1982}, "Charles III"},
        {"Henry", {M, yes}, {15, 9, 1984}, "Charles III"},
        {"Peter", {M, yes}, {15, 11, 1977}, "Anne"},
        {"Zara", {F, yes}, {15, 5, 1981}, "Anne"},
        {"Beatrice", {F, yes}, {8, 8, 1988}, "Andrew"},
        {"Eugenie", {F, yes}, {23, 3, 1990}, "Andrew"},
        {"James", {M, yes}, {17, 12, 2007}, "Edward"},
        {"Louise", {F, yes}, {8, 11, 2003}, "Edward"},
        {"Charles", {M, yes}, {1, 7, 1999}, "David"},
        {"Margarita", {F, yes}, {14, 5, 2002}, "David"},
        {"Samuel", {M, yes}, {28, 7, 1996}, "Sarah"},
        {"Arthur", {M, yes}, {6, 5, 2019}, "Sarah"},
        {"George", {M, yes}, {22, 7, 2013}, "William"},
        {"George VI", {M, no}, {14, 12, 1895}, NULL},
        {"Charlotte", {F, yes}, {2, 5, 2015}, "William"},
        {"Louis", {M, yes}, {23, 4, 2018}, "William"},
        {"Archie", {M, yes}, {6, 5, 2019}, "Henry"},
        {"Lilibet", {F, yes}, {4, 6, 2021}, "Henry"},
        {"Savannah", {F, yes}, {29, 12, 2010}, "Peter"},
        {"Isla", {F, yes}, {29, 3, 2012}, "Peter"},
        {"Mia", {F, yes}, {17, 1, 2014}, "Zara"},
        {"Lena", {F, yes}, {18, 6, 2018}, "Zara"},
        {"Elizabeth II", {F, no}, {21, 4, 1925}, "George VI"},
        {"Margaret", {F, no}, {21, 8, 1930}, "George VI"},
        {"Lucas", {M, yes}, {21, 3, 2021}, "Zara"},
        {"Sienna", {F, yes}, {18, 9, 2021}, "Beatrice"},
        {"August", {M, yes}, {9, 2, 2021}, "Eugenie"}};

    int to_do, no;
    int n;
    Food food_tab[FOOD_MAX];
    char buff[ART_MAX];
    fgets(buff, ART_MAX, stdin);
    sscanf(buff, "%d", &to_do);

    switch (to_do)
    {
    case 1: // bsearch2
        fgets(buff, ART_MAX, stdin);
        sscanf(buff, "%d", &no);
        n = read_goods(food_tab, no, stdin, 1);
        scanf("%s", buff);
        print_art(food_tab, n, buff);
        break;
    case 2: // qsort
        fgets(buff, ART_MAX, stdin);
        sscanf(buff, "%d", &no);
        n = read_goods(food_tab, no, stdin, 0);
        Date curr_date;
        int days;
        scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
        scanf("%d", &days);
        printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
        break;
    case 3: // succession
        scanf("%d", &no);
        int no_persons = sizeof(person_tab) / sizeof(Person);
        create_list(person_tab, no_persons);
        print_person(person_tab + no - 1);
        break;
    default:
        printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}
