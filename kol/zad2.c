#include <stdio.h>
#include <stdlib.h>

enum Drive_Type
{
    cumbustion,
    electric,
    d_ndef
};
enum Fuel_type
{
    gasoline,
    diesel,
    gas,
    hydrogen,
    f_d_ndef
};
enum Battery_Type
{
    Li_Ion,
    NiMHb_d_ndef
};
typedef struct
{
    int d;
    int m;
    int y
} Date;
typedef struct
{
    char emission[15];
    enum Fuel_type fuel_t;

} Comb_data;

typedef struct
{
    Battery_Type bat_t;
    float capacity;

} El_data;

typedef struct
{
    char owner[30];
    Date valid_date;
    enum Drive_Type d_type;
    union
    {
        Comb_data cs;
        El_data es;
    } drv_union;

} Vehicle;

int date_cmp(Date date1, Date date2)
{
    if (date1.y != date2.y)
    {
        return date1.y - date2.y;
    }
    if (date1.m != date2.m)
    {
        return date1.m - date2.m;
    }
    return date1.d - date2.d;
}

int delayed(const Vehicle *tv, int size, Date *base_date, int **pdelayed_owners)
{
    int delays = 0;
    int *delayed_owners = malloc(sizeof(int) * 4);
    int size1 = 4;
    for (int i = 0; i < size; i++)
    {
        if (date_cmp((tv[i]).valid_date, *base_date) < 0)
        {
            delayed_owners[delays] = i;
            if (delays == 4)
            {
                delayed_owners = realloc(delayed_owners, size1 * 2);
                size1 *= 2;
            }
            delays++;
        }
    }
    *pdelayed_owners = delayed_owners;
    if (delays == 0)
    {
        *pdelayed_owners = NULL;
    }
    return delays;
}

void print_owners(const Vehicle *tv, int *t_ind, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%s ", (tv[t_ind[i]]).owner);
    }
}

Vehicle *create_vehicle(void)
{
    Vehicle *new = malloc(sizeof(Vehicle));
    *(new->owner) = '\0';
    new->valid_date.d = 1;
    new->valid_date.m = 1;
    new->valid_date.y = 1970;
    new->d_type = d_ndef;
    return new;
}

void print_drive_data(Vehicle *v){
    if(v->d_type==electric){
        printf("Naped: el");
        switch(v->drv_union.es.bat_t){
            case Li_Ion: 
                printf("Li_Ion, ");
                break;
            case NiMHb_d_ndef:
                printf("NiMHb_d_ndef, ");
                break;
        }
        printf("Pojemnosc: %f",v->drv_union.es.capacity);

    }
    if(v->d_type==combustion){
        printf("Naped: comb");
        printf("emission: %s",v->drv_union.cs.emission);
        switch(v->drv_union.cs.fuel_t){
            case Li_Ion: 
                printf("Li_Ion, ");
                break;
            case NiMHb_d_ndef:
                printf("NiMHb_d_ndef, ");
                break;
        }
    }

}
int main()
{
}