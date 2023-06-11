#include <stdio.h>
#include <stdbool.h>
#include <math.h>


bool is_prime(int n){
    if(n==1)
        return false;
    if(n==2 || n==3)
        return true;
    if(n%2==0 || n%3==0)
        return false;
    int i=5;
    int a=sqrt(n)+1;
    //printf("\n\n%d:%d\n",n,a);
    while(i<=a){
        if(n%i==0){
            return false;
        }
        i+=2;
        if(n%i==0){
            return false;
        }
        i+=4;

    }

    return true;

}

bool nondec(int n){
    int p,q=n%10;
    n=n/10;
    //printf("%d",n);
    while(n>0){
        p=n%10;
        if (q<p){
            return false;
        }
        n/=10;
        q=p;
    }
    return true;
}

int main(){
    int n;
    scanf("%d", &n);
    for(int i=2;i<n;++i){
        if(is_prime(i)){
            if(nondec(i))
            printf("%d\n",i);
        }
    }
    return 0;

}