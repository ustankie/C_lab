#include <stdio.h>
#include <stdbool.h>
#include <math.h>

void perf_num(int n, int *npn){
    int sum,h,k; 
    for(int j=2;j<=n;++j){
        sum=1;
        h=j;
        k=(sqrt(j));
        for(int i=2; i<=k;++i){
            if(h%i==0){
                sum+=i;
                sum=sum+(h/i);
                if(j==6 || j==28 || j==8128)
                printf("%d\n",sum);
            }

        }
        if(sum==j){
            printf("%d\n\n",j);
            ++*npn;
        }
    }
}


int main(void){
    int n   ;
    int npn=0;
    scanf("%d", &n);
    perf_num(n, &npn);
    printf("%d\n",npn);
    return 0;
}