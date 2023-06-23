#include <stdio.h>
#include <stdlib.h>
#define MAX_NAME 100
enum VehicleType
{
    car,
    bus,
    truck
};
enum DriveType
{
    electric,
    hybrid,
    combustion
};

struct car
{
    int max_people;
    float power;
};
struct bus
{
    int seating_places;
    int standing_places;
};
struct truck
{
    float capacity;
    float pressure;
};

typedef struct Vehicle
{
    char *owner;
    int date[3];
    enum VehicleType v_type;
    enum DriveType d_type;
    union
    {
        struct car Car;
        struct bus Bus;
        struct truck Truck;
    };

} Vehicle;

void new_owner(struct Vehicle *pvehicle)
{
    char *new = malloc(sizeof(char) * MAX_NAME);
    fgets(new, MAX_NAME, stdin);
    free(pvehicle->owner);
    pvehicle->owner = new;
}
int date_cmp(int date1[3], int date2[3])
{
    if (date1[2] != date2[2])
    {
        return date1[2] - date2[2];
    }
    if (date1[1] != date2[1])
    {
        return date1[1] - date2[1];
    }
    return date1[0] - date2[0];
}

int delayed(Vehicle *pvehicle, int size, int *base_date, char ***pdelayed_owners)
{
    int delays = 0;
    for (int i = 0; i < size; i++)
    {
        if (date_cmp(pvehicle->date, *base_date) < 0)
        {
            *pdelayed_owners[delays] = (pvehicle->owner);
            delays++;
        }
    }
    if (delays == 0)
    {
        *pdelayed_owners = NULL;
    }
    return delays;
}

void print_pojazd(Vehicle *pvehicle)
{
    printf("name: %s\n", pvehicle->owner);
    printf("data przegladu: %d.%d.%d\n", pvehicle->date[0], pvehicle->date[1], pvehicle->date[2]);
    printf("typ pojazdu: %d\n", pvehicle->v_type);
    printf("typ napędu: %d\n", pvehicle->d_type);
    switch (pvehicle->v_type)
    {
    case car:
        printf("max_people: %d\npower: %d\n", pvehicle->Car.max_people, pvehicle->Car.power);
        break;
    case bus:
        printf("seating: %d\nstanding: %d\n", pvehicle->Bus.seating_places, pvehicle->Bus.standing_places);
        break;
    case truck:
        printf("capacity: %f\npressure: %f\n", pvehicle->Truck.capacity, pvehicle->Truck.pressure);
        break;
    }
}
