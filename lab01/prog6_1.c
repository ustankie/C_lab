#include <stdio.h>
//#include <string.h>

int main(void){
    
    int k,n;
    printf("Podaj k: ");
    scanf("%d%*c",&k);
    scanf("%d%*c",&n);
    char c[n+1],r[n+1];
    printf("Podaj slowo: ");
    //scanf("%s",c);
    fgets(c,n+1,stdin);
    // for(int i=0;i<n;++i){
    //     r[i]=(((c[i]-'a')+k)%26)+'a';

    // }
    printf("%s",c);




}