#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int *get(int cols, int row, int col, const int *A) {
	return A+row*cols+col;

}

void set(int cols, int row, int col, int *A, int value) {
	int *addr=get(cols,row,col,A);
	*addr=value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
	int a,b;
	int prod;
	for(int i=0;i<rowsA;i++){
		for(int t=0;t<colsB;t++){
			prod=0;
			for(int j=0;j<colsA;j++){
				int *addr1=get(colsA,i,j,A);
				int *addr2=get(colsB,j,t,B);
				a=*addr1;
				b=*addr2;
				prod+=a*b;
			}
			set(colsB,i,t,AB,prod);
		}
	}
}

void read_mat(int rows, int cols, int *t) {
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			scanf("%d",(t+(i*cols)+j));
		}
		
	}
}

void print_mat(int rows, int cols, int *t) {
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			printf("%d ",*(t+i*cols+j));
		}
		printf("\n");
	}
}

int read_char_lines(char *array[]) {
	int n=0;
	char *tab=(char *)malloc(TAB_SIZE*sizeof(char));

	char buff[BUF_SIZE];
	char c=0;
	char i=0;
	char j=0;
	*array=tab;
	while (c!=EOF){
		j=0;
		c=getchar();			
		for(int k=0;k<BUF_SIZE;k++){
				buff[k]=0;
		}
		i=0;
		while (c!='\n' && c!=EOF){			
			buff[i]=c;
			i++;
			c=getchar();
		}
		buff[i]='\0';
		strcpy((*(array+n)),buff);
		*(array+n+1)=*(array+n)+i+1;
		n++;

	}

}

void write_char_line(char *array[], int n) {
	printf("%s\n",(*(array+n)));
}

void delete_lines(char *array[]) {
}

int read_int_lines_cont(int *ptr_array[]) {
	int n=0;
	int *tab=(int *)malloc(TAB_SIZE*sizeof(int));

	char buff[BUF_SIZE];
	char c=0;
	int i=0;
	int j=0;
	*(ptr_array)=tab;
	while(c!=EOF){
		j=0;
		c=getchar();		
		while(c!='\n' && c!=EOF){
			for(int k=0;k<BUF_SIZE;k++){
				buff[k]=0;
			}
			i=0;
			
			
			while(c!='\n' && c!=EOF && c!=' '){
				buff[i]=c;
				i++;
				c=getchar();
			}
			*(*(ptr_array+n)+j)=(int)strtol(buff,NULL,10);			
			
			j++;
			if(c==' '){
				c=getchar();
			}
		}
		if(c=='\n'){
			*(ptr_array+n+1)=(*(ptr_array+n)+j);
			
		}n++;


	}

	return n;
	
	
}

void write_int_line_cont(int *ptr_array[], int n) {
	int *i=*(ptr_array+n);
	int *j=*(ptr_array+n+1);
	while (i<j){
		printf("%d ",(*i));
		i+=1;
	}

}

typedef struct {
	int *values;
	int len;
	double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
	int n=0;
	int *tab=(int *)malloc(TAB_SIZE*sizeof(char));
	int num;
	double sum=0;
	char buff[TAB_SIZE];

	int i=0,j=0;
	char c=0;
	lines_array[0].values=tab;

	while(c!=EOF){			
		c=getchar();
		sum=0;
		j=0;
		while(c!='\n'&& c!=EOF){
			i=0;
			for(int k=0;k<BUF_SIZE;k++){
				buff[k]=0;
			}
			while(c!='\n'&& c!=EOF && c!=' '){
				buff[i]=c;
				i++;
				c=getchar();
			}

			num=strtol(buff,NULL,10);
			sum+=num;
			*(lines_array[n].values+j)=num;
			j++;
			if(c==' '){
				c=getchar();
			}

		}

		lines_array[n+1].values=lines_array[n].values+j;
		lines_array[n].average=(double)(sum/((double)j));
		lines_array[n].len=j;
		n++;

	}
	// for(int i=0;i<n;i++){
	// 	printf("%lf\n",lines_array[i].average);
	// }
	// printf("n: %d\n",n);
	return n;

}


void write_int_line(line_type lines_array[], int n) {
	int *a=lines_array[n].values;
	int *b=lines_array[n+1].values;

	int m=lines_array[n].len;
	for(int i=0;i<m;i++){
		printf("%d ",a[i]);
	}
	printf("\n%.2lf\n",lines_array[n].average);
}

void delete_int_lines(line_type array[], int line_count) {
}

int cmp (const void *a, const void *b) {
	double av1=((line_type *)a)->average;
	double av2=((line_type *)b)->average;
	if (av1<av2){
		return -1;
	}
	if (av1>av2){
		return 1;
	}
	return 0;

}

void sort_by_average(line_type lines_array[], int line_count) {
	qsort(lines_array,line_count,sizeof(line_type),cmp);
}

typedef struct {
	int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
	int n=0;
	char buff[BUF_SIZE];
	char c=0;
	int i=0;
	int j=0;

	for(int i=0;i<n_triplets;i++){
		scanf("%d %d %d",&(triplet_array[i].r),&(triplet_array[i].c),&(triplet_array[i].v));
		n++;
	}

	return n;

}

int cmp_triplets(const void *t1, const void *t2) {
	int row1=((triplet *)t1)->r;
	int row2=((triplet *)t2)->r;
	int col1=((triplet *)t1)->c;
	int col2=((triplet *)t2)->c;

	if(row1!=row2){
		return row1-row2;
	}
	return col1-col2;
	
}
void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
	qsort(triplet_array, n_triplets, sizeof(triplet), cmp_triplets);
	for(int i=0;i<n_triplets;i++){
		V[i]=triplet_array[i].v;
		C[i]=triplet_array[i].c;
		R[triplet_array[i].r+1]+=1;
	}
	for(int i=1;i<=rows;i++)
		R[i]+=R[i-1];
}
// void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
// 	qsort(triplet_array,n_triplets,sizeof(triplet),cmp_triplets);
// 	int n=0;
// 	int j=1;
// 	R[0]=0;
// 	for(int i=0;i<n_triplets;i++){
// 		*(V+i)=0;
// 		*(C+i)=0;

// 	}

// 	for(int i=0;i<n_triplets;i++){
// 		*(V+i)=triplet_array[i].v;
// 		*(C+i)=triplet_array[i].c;
// 		if(i>0 && (triplet_array[i].r)!=(triplet_array[i-1].r)){
// 			int r1=(triplet_array[i-1].r)+1;
// 			int r2=(triplet_array[i].r);
// 			while (r1<r2){
// 				R[j]=0;
// 				j++;
// 				r1++;
// 			}
// 			R[j]=n+R[j-1];
// 			n=0;
// 			j++;
// 		}
// 		else if (i==0){
// 			int r1=0;
// 			int r2=(triplet_array[i].r);
// 			while (r1<r2){
				
// 				R[j]=0;
// 				j++;
// 				r1++;
// 			}

// 		}
// 		n++;
		
// 	}
// 	while(j<=rows){
// 		R[j]=n+R[j-1];
// 		n=0;
// 		j++;
// 	}
	
// }

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
	for(int i=0;i<rows;i++){
		y[i]=0;
		for(int j=R[i];j<(R[i+1]);j++){
			y[i]+=V[j]*x[C[j]];

		}
	}
}

void read_vector(int *v, int n) {
	for(int i=0;i<n;i++){
		scanf("%d",v+i);
	}
}

void write_vector(int *v, int n) {
	for(int i=0;i<n;i++){
		printf("%d ",v[i]);
	}
	printf("\n");
}

int read_int() {
	char c_buf[BUF_SIZE];
	fgets(c_buf, BUF_SIZE, stdin);
	return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
	int to_do = read_int();

	int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB;
	int rows, cols, n_triplets;
	char *char_lines_array[TAB_SIZE] = { NULL };
	int continuous_array[TAB_SIZE];
	int *ptr_array[TAB_SIZE];
	triplet triplet_array[TAB_SIZE];
	int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
	int x[TAB_SIZE], y[TAB_SIZE];
	line_type int_lines_array[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d", &rowsA, &colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d", &rowsB, &colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA, colsA, colsB, A, B, AB);
			print_mat(rowsA, colsB, AB);
			break;
		case 2:
			n = read_int() - 1; // we count from 1 :)
			ptr_array[0] = continuous_array;
			read_int_lines_cont(ptr_array);
			write_int_line_cont(ptr_array, n);
			break;
		case 3:
			n = read_int() - 1;
			read_char_lines(char_lines_array);
			write_char_line(char_lines_array, n);
			delete_lines(char_lines_array);
			break;
		case 4:
			n = read_int() - 1;
			lines_counter = read_int_lines(int_lines_array);
			sort_by_average(int_lines_array, lines_counter);
			write_int_line(int_lines_array, n);
			delete_int_lines(int_lines_array, lines_counter);
			break;
		case 5:
			scanf("%d %d %d", &rows, &cols, &n_triplets);
			n_triplets = read_sparse(triplet_array, n_triplets);
			read_vector(x, cols);
			make_CSR(triplet_array, n_triplets, rows, V, C, R);
			multiply_by_vector(rows, V, C, R, x, y);
			write_vector(V, n_triplets);
			write_vector(C, n_triplets);
			write_vector(R, rows + 1);
			write_vector(y, rows);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

