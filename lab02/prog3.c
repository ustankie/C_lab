#include <stdio.h>
#include <math.h>

int is_narc(int base,int m,unsigned int n);
void print_narc(int base, unsigned int n);

int main(void){
    int m,b,printed=0;
    scanf("%d %d",&m,&b);
    unsigned int min,max;
    min=pow(b,m-1);
    max=pow(b,m);
    for(unsigned int i=min;i<=max;++i){
        if(is_narc(b,m,i)){
            print_narc(b,i);
            printf("\n");
            printed=1;
        }
    }
    if(!printed){
        printf("NO\n");
    }

}

int is_narc(int base, int m,unsigned int n){
    unsigned int a,b,sum=0;
    b=n;
    while(n>0){
        a=n%base;
        n=n/base;
        sum+=pow(a,m);
    }
    return(b==sum);
}

void print_narc(int base, unsigned int n){
    unsigned int a,b;
    char sings[]
    char out[base];
    b=n;
    int i=0;
    while(n>0){
        a=n%base;
        n=n/base;
        if(a>9){
            a=a+'A'-10;
        }
        else{
            a=a+'0';
        }
        out[i]=a;
        i+=1;
    }
    for(int j=i-1;j>=0;--j){
        printf("%c",out[j]);
    }
}