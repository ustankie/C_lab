#include <stdio.h>
//#include <string.h>

int main(void){
    char c,r;
    int k,word;
    c='a';
    printf("Podaj k: ");
    scanf("%d%*c",&k);
    
    printf("Podaj slowo: ");
    //getchar();
    while((c=getchar())!=EOF){
        if(c!='\n'){
        r=(((c-'a')+k)%26)+'a';
        //printf("%c",r);
        putchar(r);

        }
        else{putchar(c);}

    }




}