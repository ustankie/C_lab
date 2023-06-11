#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int main(void){
    srand(time(NULL));
    int x,a,b,l=0;
    int tab[102];
    for(int i=0;i<101;++i){
        tab[i]=i;
    }
    printf("Podaj liczbe od 1 do 100: ");
    scanf("%d",&x);
    while(1){
        ++l;
        b=102-l;
        if (b<2){
            printf("found! %d\n",x);
            break;
        }
        //printf("%d\n",l);
        
        a=(rand()%b);
        printf("%d\n",tab[a]);

        if(a==x){
            printf("found! %d\n",x);

            break;
        }
        else{
            int j;
            j=a;
            while(j<(b-2)){
                tab[j]=tab[j+1];
                ++j;
            }
            printf("tab: \n");
            for(int k=0;k<(b-1);++k){
                printf("%d\n",tab[k]);
            }
            printf("\n");
        }
    
    }
    printf("\n%d\n",l);

}