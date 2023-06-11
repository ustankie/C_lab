#include <stdio.h>

int main(void){
    int n,j,q,l=0,p=0;
    scanf("%d",&n);

    int T[n];
    for(int i=0;i<n;++i){
        scanf("%d",&T[i]);
        p+=T[i];
    }
    
    j=1;
    p-=T[0];
    while(j<n && l!=p){
        l+=T[j-1];
        p-=T[j];
        ++j;
    }
    printf("%d",j-1);
    return 0;

}