Franciszek Urbański
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double linear(double x);
double quadratic(double x);

#define X_MIN 0.0
#define X_MAX 5.0
#define DX 0.1
#define FUN_NR_MAX 8

void print_info(void);
void print_value(double x, int fun_nr);

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        print_info();
        exit(-1);
    }

    for (double x = X_MIN; x < X_MAX + 0.1 * DX; x += DX) 
    {
        printf("%10g ", x); // Wypisywanie argumentu x
        for (int i = 1; i < argc; ++i)
        {
            int fun_nr = atoi(*(argv + i)); // Numer funkcji, wybranej przez użytkownika.
            print_value(x, fun_nr);
        }
        printf("\n");
    }
    return 0;
}

double linear(double x) { return 2 * x + 1.5; }

double quadratic(double x) { return x * x - 2 * x + 1.5; }

void print_value(double x, int fun_nr)
{
    if (fun_nr < 0 || fun_nr > FUN_NR_MAX)
    {
        fprintf(stderr, "BLAD: numer funkcji musi byc dodatni i mniejszy od %d\n", FUN_NR_MAX);
        return;
    }
    double y = 0.;
    switch (fun_nr)
    {
    case 0:
        y = exp(x);
        break;
    case 1:
        y = fabs(x);
        break;
    case 2:
        y = exp2(x);
        break;
    case 3:
        y = sqrt(x);
        break;
    case 4:
        y = sin(x);
        break;
    case 5:
        y = cos(x);
        break;
    case 6:
        y = ceil(x);
        break;
    case 7:
        y = linear(x);
        break;
    case 8:
        y = quadratic(x);
        break;
    }
    printf("%10g ", y);
}

void print_info(void)
{
    printf("Podaj numer(y) funkcji:\n");
    printf("0 - f(x) = e^x\n");
    printf("1 - f(x) = |x|\n");
    printf("2 - f(x) = 2^x\n");
    printf("3 - f(x) = sqrt(x)\n");
    printf("4 - f(x) = sin(x)\n");
    printf("5 - f(x) = cos(x)\n");
    printf("6 - f(x) = [x]]\n");
    printf("7 - f(x) =  2x + 1.5\n");
    printf("8 - f(x) = x^2 - 2x + 1.5\n");
}
