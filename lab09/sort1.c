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
float cmp_date(const void *d1, const void *d2)
{ // printf("a\n");
    Food *f1 = (Food *)d1;
    Food *f2 = (Food *)d2;
    Date date1 = (f1->valid_date);
    Date date2 = (f2->valid_date);
    float diff = date1.year - date2.year;
    //printf("date: %d %f %d %d %d\n",date1.day,diff,date2.day,date2.month,date2.year);

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

    // printf("%p %p\n", l, q);

    if (nmemb == 0)
    {
        result = 0;
        return (void *)tab;
    }

    while (l < r)
    {
        int diff = (((r - l) / size) / 2) * size;
        q = l + (diff);
        float res = compar((void *)el, (void *)q);
        // printf("res: %f\n",res);
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
            // printf("res1:%f\n", res);
            *result = 1;
            return (void *)q;
        }
    }
    if (l == r && compar((void *)el, (void *)l) == 0)
    {
        *result = 1;
    }
    // printf("%d %p %p\n", size, l, r);
    if (l > (tab + nmemb * size))
    {
        l = tab + nmemb * size;

        return (void *)l;
    }

    return (void *)l;
}

// print goods of given name
void print_art(Food *p, int n, char *art)
{
    for (int i = 0; i < n; i++)
    {
        Food *new = (p + i);

        int a = (strcmp(new->name, art));

        if (a == 0)
        {

            printf("%.2f %d %d.%d.%d\n", new->price, new->amount, new->valid_date.day, new->valid_date.month, new->valid_date.year);
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
    // printf("result: %d\n",result);
    if (addr < (tab + *np) && cmp((void *)new, (void *)addr) > 0)
    {
        addr += 1;
    }
    if (result)
    {
        // printf("%s %s\n",addr->name,new->name);
        (addr->amount) += new->amount;
    }
    else
    {
        Food *p = tab + *np;

        //("\nw miejsce:%s %lf %d %d.%d.%d  dodaj: %s %lf %d %d.%d.%d\n", addr->name, addr->price, addr->amount, addr->valid_date.day, addr->valid_date.month, addr->valid_date.year, new->name, new->price, new->amount, new->valid_date.day, new->valid_date.month, new->valid_date.year);
        for (Food *i = p; i > (addr); i -= 1)
        {
            *i = *(i - 1);
        }
        *addr = *new;
        (*np)++;
        // printf("\n\n");
        // print_tab(tab, *np);
        // printf("\n\n");
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
    // printf("%d %s\n", np, tab->name);
    // print_tab(tab, np);
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
{ // print_tab(food_tab,n);
    qsort(food_tab, n, sizeof(Food), cmp_date);
    // printf("\n");
    // print_tab(food_tab,n);

    struct tm t;
    t.tm_sec = 0;
    t.tm_min = 0;
    t.tm_year = curr_date.year - 1900;
    t.tm_mon = curr_date.month; // 0 = Jan, 1 = Feb etc
    t.tm_mday = curr_date.day;
    t.tm_hour = 0; // fictious
    t.tm_mday += days;
    mktime(&t);
    t.tm_year += 1900;
    curr_date.year = t.tm_year;
    curr_date.month = t.tm_mon;
    curr_date.day = t.tm_mday;

    Food date;
    date.valid_date = curr_date;
    // printf("%d %d %d\n",date.valid_date.day,date.valid_date.month,date.valid_date.year);
    char result = 0;

    Food *addr = (Food *)bsearch2(&date, food_tab, n, sizeof(Food), cmp_date, &result);
    // printf("%d\n",result);
    // printf(" %s %lf %d %d.%d.%d\n", addr->name, addr->price, addr->amount, addr->valid_date.day, addr->valid_date.month, addr->valid_date.year);

    float sum = 0;
    Food *addr2 = addr;
    while ((addr->valid_date).day == date.valid_date.day && (addr->valid_date).month == date.valid_date.month && (addr->valid_date).year == date.valid_date.year)
    {
        // printf(" %s %lf %d %d.%d.%d\n", addr->name, addr->price, addr->amount, addr->valid_date.day, addr->valid_date.month, addr->valid_date.year);

        // printf("%f\n",sum);
        sum += (addr->amount) * (addr->price);
        // printf("%f\n",sum);
        addr += 1;
    }
    addr = addr2;
    addr--;
    while ((addr->valid_date).day == date.valid_date.day && (addr->valid_date).month == date.valid_date.month && (addr->valid_date).year == date.valid_date.year)
    {
        // printf(" %s %lf %d %d.%d.%d\n", addr->name, addr->price, addr->amount, addr->valid_date.day, addr->valid_date.month, addr->valid_date.year);

        // printf("%f\n",sum);
        sum += (addr->amount) * (addr->price);
        // printf("%f\n",sum);
        addr -= 1;
    }

    return sum;
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
    int children;
} Parent;

const Date primo_date = {28, 10, 2011}; // new succession act

float cmp_primo(Person *person1, Person *person2)
{

    if (person1->bits.sex == person2->bits.sex)
        return 0;        {        
    }
    if (person1->bits.sex == F && cmp_date2(&person1->born, &primo_date) > 0)
        return 0;
    if (person2->bits.sex == F && cmp_date2(&person2->born, &primo_date) > 0)
        return 0;

    return person2->bits.sex - person1->bits.sex;
}
int cmp_date2(const void *d1, const void *d2)
{ // printf("a\n");

    Date *date1 = (Date *)d1;
    Date *date2 = (Date *)d2;
    float diff = date1->year - date2->year;
    //printf("date: %d %f %d %d %d\n",date1->day,diff,date2->day,date2->month,date2->year);

    if (date1->year != date2->year)
    {
        return ((date1->year) - (date2->year));
    }
    if (date2->month != date1->month)
    {
        return (date1->month - date2->month);
    }
    return (date1->day - date2->day);
}

// compare persons
int cmp_person(const void *a, const void *b)
{
    Person *p1 = (Person *)a;
    Person *p2 = (Person *)b;
    // printf("%s %s\n",p1->parent,p2->parent);
    if (p1->parent == NULL)
    {
        return -1;
    }
    if (p2->parent == NULL)
    {
        return 1;
    }
    int c = strcmp(p1->parent, p2->parent);
    if (c == 0)
    {
        float p = cmp_primo(p1, p2);
        // if(strcmp(p1->name,"Anne")==0){
        //     printf("Anne %f %s\n",p,p2->name);
        // }
        if (p != 0)
        {
            return p;
        }
        Date *d1 = &(p1->born);
        Date *d2 = &(p2->born);
        return cmp_date2((void *)d1, (void *)d2);
    }
    else
    {
        return c;
    }
}

float cmp_par(const void *a, const void *b)
{
    Parent *p1 = (Parent *)a;
    Parent *p2 = (Parent *)b;
    return strcmp(p1->par_name, p2->par_name);
}
int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size)
{
    int n = 1;
    idx_tab[0].par_name = pers_tab[1].parent;
    idx_tab[0].index = 1;
    idx_tab[0].children = 0;
    for (int i = 2; i < size; i++)
    {
        idx_tab[n - 1].children += 1;
        if (pers_tab[i].parent != idx_tab[n - 1].par_name)
        {
            idx_tab[n].par_name = pers_tab[i].parent;
            idx_tab[n].index = i;
            n++;
            idx_tab[n].children = 0;
        }
    }
    idx_tab[n - 1].children += 1;
    return n;
}
void print_people(Person *person_tab, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%s %d %d %d.%d.%d %s\n", person_tab[i].name, person_tab[i].bits.sex, person_tab[i].bits.in_line,
               person_tab[i].born.day, person_tab[i].born.month, person_tab[i].born.year, person_tab[i].parent);
    }
}
void print_parent(Parent *par, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%s %d %d\n", par[i].par_name, par[i].index, par[i].children);
    }
}
void persons_shifts(Person *person_tab, int size, Parent *idx_tab, int no_parents)
{
    Person *temp[size];
    // int i=0;
    int children_sum = 0;
    int children_prev = 0;
    Parent *cperson;
    char result = 0;

    for (int i = 0; i < size; i++)
    {
        Parent comp;
        comp.par_name = person_tab[i].name;
        result = 0;
        cperson = (Parent *)bsearch2(&comp, idx_tab, no_parents, sizeof(Parent), cmp_par, &result);
        //printf("res: %d\n", result);

        int children = cperson->children;
        int idx = cperson->index;

        //printf("%d %d %d\n", idx, i, idx - i - 1);
        // printf("\n%d %d\n\n",idx,children);
        if (result == 1 && idx > (i))
        {
            memcpy(temp, person_tab + idx, children * sizeof(Person));
            memmove(person_tab + i + children + 1, person_tab + i + 1, (idx - i - 1) * sizeof(Person));
            memcpy(person_tab + i + 1, temp, children * sizeof(Person));

            for (int j = 0; j < no_parents; j++)
            {
                int a = idx_tab[j].index;
                if (a > (i) && a < (idx))
                {

                    idx_tab[j].index += children;
                    // printf("%d\n",idx_tab[j].index);
                }
            }
            cperson->index = i + 1;
            // if (i < 6)
            // {
            //     printf("%s\n", person_tab[i].name);
            //     printf("\n%d\n", cperson->index);
            //     print_parent(idx_tab, no_parents);
            //     print_people(person_tab, size);
            // }

            children_prev = children_sum;
            children_sum += children;
            //printf("%d %d\n", i, children + i);
            // print_parent(idx_tab,no_parents);
        }

        // print_people(person_tab,size);
    }


}

int cleaning(Person *person_tab, int n)
{
    int s = n;
    int i = 0;

    while (i < s)
    {
        if (person_tab[i].bits.in_line == no)
        {
            memmove(person_tab + i, person_tab + (i + 1), (s - i - 1) * sizeof(Person));
            s--;
            int j = 0;
        }
        else
        {
            i++;
        }
    }
    return s;
}

void print_person(const Person *p)
{
    printf("%s\n", p->name);
}

int create_list(Person *person_tab, int n)
{
    qsort(person_tab, n, sizeof(Person), cmp_person);
    //print_people(person_tab, n);

    Parent idx_tab[n];
    int m = fill_indices_tab(idx_tab, person_tab, n);
    //print_parent(idx_tab, m);

    qsort(idx_tab, m, sizeof(Parent), cmp_par);

    //printf("\n");

    persons_shifts(person_tab, n, idx_tab, m);
    //print_people(person_tab, n);
    //print_parent(idx_tab, m);
    n = cleaning(person_tab, n);
    //print_people(person_tab, n);
    //print_parent(idx_tab, m);
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
