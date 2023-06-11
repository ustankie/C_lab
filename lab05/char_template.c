#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb]) return va - vb;	return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
	int c;
	int state=0;
	*nl=0;
	*nw=0;
	*nc=0;
	//printf("%d %d %d\n",*nw,*nl,*nc);
	while ((c=getchar())!=EOF){
		*nc+=1;
		//printf("%d\n",*nc);
		if(c=='\t' || c==' '){
			if (state){
				*nw+=1;
				state=0;
			}
		}
		else if(c=='\n'){
			if (state){
				*nw+=1;
				state=0;
			}
			*nl+=1;
		}
		else{
			state=1;
		}
		//printf("%d %d %d",&nl,&nw,&nc);
	}
}
void char_count(int char_no, int *n_char, int *cnt) {
	int c;
	int tab[MAX_CHARS];

	for (int i=0;i<MAX_CHARS;++i){
		tab[i]=i;
	}

	while((c=getchar())!=EOF){
		//printf("%c ",c);
		if(c>=FIRST_CHAR && c<LAST_CHAR){
			count[c-FIRST_CHAR]+=1;

		}

	}
	//printf("\n");
	//*n_char="a";
	//*cnt=-1;
	// for (int i=0;i<MAX_CHARS;++i){
	// 	printf("%c | %d\n",i+FIRST_CHAR,count[i]);
	// }
	//printf("\n");
	qsort(tab,MAX_CHARS,sizeof(tab[0]),cmp);
	// for (int i=0;i<MAX_CHARS;++i){
	// 	printf("%d\n",tab[i]);
	// }
	*n_char=tab[char_no-1]+FIRST_CHAR;
	*cnt=count[tab[char_no-1]];
	

}

void bigram_count(int bigram_no, int bigram[]) {
	int c,q,k;
	int tab[MAX_BIGRAMS];

	for (int i=0;i<MAX_BIGRAMS;++i){
		tab[i]=i;
	}
	q=getchar();
	while((c=getchar())!=EOF){
		if(q>=FIRST_CHAR && q<LAST_CHAR && c>=FIRST_CHAR && c<LAST_CHAR){
		k=(q-FIRST_CHAR)*MAX_CHARS+(c-FIRST_CHAR);
		//printf("%d %d %d\n",q,c,k);
		count[k]+=1;
		}
		q=c;

	}


	qsort(tab,MAX_BIGRAMS,sizeof(tab[0]),cmp_di);
	bigram[0]=(int)(tab[bigram_no-1]/MAX_CHARS)+FIRST_CHAR;
	bigram[1]=(int)(tab[bigram_no-1]%MAX_CHARS)+FIRST_CHAR;
	bigram[2]=count[tab[bigram_no-1]];

}

void find_comments(int *line_comment_counter, int *block_comment_counter) {
	int c,q;
	int inside_block=0;
	int inside_line=0;
	q=getchar();
	*line_comment_counter=0;
	*block_comment_counter=0;
	while((c=getchar())!=EOF){
		if(q==c && c=='/' && !inside_line && !inside_block){
			*line_comment_counter+=1;
			inside_line=1;
		}
		else if(c=='\n'){
			inside_line=0;
		}
		else if(q=='/' && c=='*' && !inside_line && !inside_block){
			*block_comment_counter+=1;
			inside_block=1;
			if((c=getchar()==EOF)){
				break;
			}
		}
		else if(q=='*' && c=='/' && inside_block){
			inside_block=0;
		}
		q=c;
	}

}

#define MAX_LINE 128

int read_int() {
	char line[MAX_LINE];
	fgets(line, MAX_LINE, stdin); // to get the whole line
	return (int)strtol(line, NULL, 10);
}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int bigram[3];

	to_do = read_int();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_int();
			char_count(char_no, &n_char, &cnt);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // bigram_count()
			char_no = read_int();
			bigram_count(char_no, bigram);
			printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

