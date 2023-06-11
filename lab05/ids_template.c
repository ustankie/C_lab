#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8
#define MIN_DIGIT 48
#define MAX_DIGIT 57
#define MIN_CAPITAL 65
#define MAX_CAPITAL 90
#define MIN_LOWER 97
#define MAX_LOWER 122
#define DOWN 95


#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char identifiers[MAX_IDS][MAX_ID_LEN];

const char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

//char all_words[MAX_IDS][MAX_ID_LEN];
int curr_words=0;

int check_keys(char word[],int *count,int j){
	int key=32;
	int flag=1;

	for(int k=0;k<key;++k){
		int a=strlen(keywords[k]);
		if(a==j){
				int i=0;
				while(i<j && word[i]==keywords[k][i]){
					//	("%c ",word[i]);
					i+=1;

				}
				if(i==j){
					flag=0;
				}
			}
	}
	for(int k=0;k<curr_words;++k){
		int a=strlen(identifiers[k]);
		if(a==j){
				int i=0;
				while(i<j && word[i]==identifiers[k][i]){
					//printf("%c ",word[i]);
					i+=1;

				}
				if(i==j){
					flag=0;
				}
			}
	}
	if(j>0 && flag){
		//int len=0;
		for(int i=0;i<j;++i){
			//printf("%c",word[i]);
			identifiers[curr_words][i]=word[i];
			//len=i;

		}
		char tab[MAX_IDS][MAX_ID_LEN];
		qsort(identifiers,curr_words,sizeof(char*),index_cmp);
		
		curr_words+=1;
		*count+=1;
		for(int i=0; i<curr_words;++i){
			int a=strlen(tab[i]);
			for(int k=0;k<a;++k){
			identifiers[i][k]=tab[i][k];
			}
			printf("%s %d\n",tab[i],a);
			
			//printf("%s\n",identifiers[i]);
			
		 }
	}
	//printf("\n");
	//printf("%d",*count);
}
int find_idents() {
	int go_to=0;
	char word[MAX_ID_LEN];
	int j=0;
	int count=0;
	int q,c;
	q=getchar();
	switch(q){
			case '*':
			case '/':
				break;

			case '\"':
				go_to=IN_STRING;								
				break;

			case '\t':
			case ' ':
			case '\n':
				break;
			default:{
				if((q>=MIN_CAPITAL && q<=MAX_CAPITAL)||(q>=MIN_LOWER && q<=MAX_LOWER)||(q==DOWN)){
					word[j]=q;
					//printf("%c\n",word[j]);
					j+=1;
					go_to=IN_ID;			
				}				
				break;
					
			}
	}
	while((c=getchar())!=EOF){
		//printf("%c",c);
		switch(c){
			case '*':{
				if(q=='/'){

					if(go_to==0)
					{
						go_to=IN_BLOCK_COMMENT;	
					}
						
					j=0;				
				}
				else{
					if(go_to==IN_ID){
						go_to=0;
						//printf("nice");
						check_keys(word,&count,j);
						j=0;	
						}
				}
				break;
			}
		

			case '/':
				if(q=='*'){
					//						printf("nice");

					if(go_to==IN_BLOCK_COMMENT)
						{go_to=0;}
						j=0;
				}
				else if(q==c){
					if(go_to==0){
						go_to=IN_LINE_COMMENT;
						j=0;

					}
				}
				else{ 
					if(go_to==IN_ID){
						go_to=0;
						check_keys(word,&count,j);
						j=0;	
						}
				}
				break;

			case '\"':
			case '\'':
				if(go_to!=IN_BLOCK_COMMENT && go_to!=IN_LINE_COMMENT){
					if (go_to==IN_STRING){
						if(q=='\\'){
					
						}
						else{
							go_to=0;
							j=0;
						}
			
					}
					else if(go_to==IN_ID){
						go_to=IN_STRING;
						check_keys(word,&count,j);
						j=0;	
						}	
					else go_to=IN_STRING;				
					}
				
				break;

			case '\t':
			case ' ':
				if(go_to==IN_ID || go_to==5){
					go_to=0;
					check_keys(word,&count,j);
					
				}j=0;
				break;

			case '\n':
				if(go_to==IN_ID|| go_to==IN_LINE_COMMENT){
					go_to=0;
					check_keys(word,&count,j);
					
				}
				j=0;
				break;
			default:{
				if(go_to!=IN_BLOCK_COMMENT && go_to!=IN_LINE_COMMENT && go_to!=IN_STRING){
					if((c>=MIN_DIGIT && c<=MAX_DIGIT)||(c>=MIN_CAPITAL && c<=MAX_CAPITAL)||(c>=MIN_LOWER && c<=MAX_LOWER)||(c==DOWN)){
						if(j==0){
							if((c>=MIN_CAPITAL && c<=MAX_CAPITAL)||(c>=MIN_LOWER && c<=MAX_LOWER)||(c==DOWN)){
								word[j]=c;
								j+=1;
								go_to=IN_ID;
							}
						}
						else{
							word[j]=c;
							j+=1;
							go_to=IN_ID;
						}
					}
					else{
						go_to=0;
						check_keys(word,&count,j);
						j=0;
					}
				}
				else j=0;

				break;



			}
				

					
		}


		q=c;

		


		
	}
	// for(int i=0;i<j;++i){
	// 	printf("%c",word[i]);
	// }

	check_keys(word,&count,j);
	return count;

}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(identifiers[a], identifiers[b]);
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}

