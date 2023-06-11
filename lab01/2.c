#include <stdio.h>
#include <stdbool.h>

bool is_product(int n);

bool is_product(int n){
    int temp,a=0,b=1;

    while ((b*a)<=(n)){
        if((a*b)==n){
            return true;
        }
        temp=b;
        b=a+temp;
        a=temp;

    }
    return (a*b==n);

}


int main(void){
    int n;
    printf("Wpisz liczbe: ");
    scanf("%d", &n);
    printf("%s", is_product(n) ? "true\n":"false\n");
    return 0;


}
