#include <stdio.h>
#include <stdbool.h>
bool waga(int [],int,int,int,int);


int main(void){
    int n,w;
    scanf("%d %d",&n,&w);

    int T[n];
    for (int i=0;i<n;++i){
        scanf("%d",&T[i]);
    }
    if(waga(T,n,w,0,0)){
        printf("YES");
    }
    else{
        printf("NO");
    }

    return 0;
}


bool waga(int T[],int n, int l, int p,int i){
    if(l==p){
        return true;
    }
    if(i==(n)){
        return false;
    }
    //printf("%d %d\n",l,p);
    return (waga(T,n,l+T[i],p,i+1) || waga(T,n,l,p,i+1) || waga(T,n,l,p+T[i],i+1));
}