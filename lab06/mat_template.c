#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
	for(int i=0;i<m;++i){
		for(int j=0;j<p;++j){
			AB[i][j]=0;
		}
	}
	for(int i=0;i<m;++i){
		for(int j=0;j<n;++j){
			for(int k=0;k<p;++k){
				AB[i][j]+=A[i][k]*B[k][j];
			}
			
		}
	}
	//print_mat(A,m,n);
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
void swap_rows(double A[][SIZE],int n, int i,int j){
	for(int k=0;k<n;++k){
		int temp=A[i][k];
		A[i][k]=A[j][k];
		A[j][k]=temp;
	}
}

void add_rows(double A[][SIZE],int n, int i,int j){
	for(int k=0;k<n;++k){
		A[j][k]+=A[i][k];
	}
}
void multiply_row(double A[][SIZE],int n, int i,double w){
	for(int k=0;k<n;++k){
		A[i][k]*=w;
	}
}

int is_not_diagonal(int ZERO[],int n){
	for(int i=0;i<n;++i){
		// for(int j=0;j<i;++j){
		// 	if(A[i][j]!=0) return 1;
		// }
		if(ZERO[i]<i)return 1;
	}
	return 0;
}
void make_stairs(double A[][SIZE],int n,int ZERO[],int *ind){
	for(int i=0;i<(n-1);++i){
		
		for(int j=i+1;j<n;++j){
			
			if(ZERO[i]>ZERO[j]){

				int temp=ZERO[i];
				ZERO[i]=ZERO[j];
				ZERO[j]=temp;
				swap_rows(A,n,i,j);
				if(i==*ind)*ind=j;
				else if(j==*ind)*ind=i;
				
			}

		}
	}
	////for(int i=0;i<(n);++i)printf("%d\n",ZERO[i]);
	//return ind;
}
void correct_max(int ZERO[],int a,int n,int *max_k,int *ind){
	*max_k=1000;
	for(int i=a;i<n;++i){
		//printf("a");
		if(ZERO[i]<=*max_k){
			*max_k=ZERO[i];
			*ind=i;
			//printf("%d %d\n",*ind,*max_k);
		}
	}
}
int zero_on_diag(double A[][SIZE], int n){
	for(int i=0;i<n;++i){
		if( A[i][i]==0) return 1;
	}
	return 0;
}
double gauss_simplified(double A[][SIZE], int n) {
	int c;
	int ZERO[SIZE];
	int max_k=1000;
	int ind=0,a=0;
	double multiplier;
	double det=1;
	if(zero_on_diag(A,n))return NAN;
	for(int i=0;i<n;++i){
		int j=0;
		while(j<n && A[i][j]==0)j+=1;
		ZERO[i]=j;
		if((ZERO[i])<=max_k){
			max_k=j;
			ind=i;
		}
		////printf("%d\n",j);
	}
	make_stairs(A,n,ZERO,&ind);
	
	while((is_not_diagonal(ZERO,n))){
		while(ind>0 && ZERO[ind-1]==ZERO[ind]){
			//if(zero_on_diag(A,n))return NAN;
			if(abs(A[ind][ZERO[ind]])>abs(A[ind-1][ZERO[ind-1]])){
				multiplier=(-1)*A[ind][ZERO[ind]]/A[ind-1][ZERO[ind-1]];
				////printf("%lf\n",multiplier);
				multiply_row(A,n,ind-1,multiplier);
				//print_mat(A,n,n);

				swap_rows(A,n,ind,ind-1);
				det*=(-1);
			}
			else{
				multiplier=(-1)*A[ind-1][ZERO[ind-1]]/A[ind][ZERO[ind]];
				multiply_row(A,n,ind,multiplier);
				// swap_rows(A,n,ind,ind-1);

			}
			det*=1/multiplier;
			////printf("%d\n",ind);
			int ind_prev=ind;
			add_rows(A,n,ind-1,ind);
			int j=0;
			while(j<n && A[ind][j]==0)j+=1;
			ZERO[ind]=j;

			////print_mat(A,n,n);

			////for(int i=0;i<n;++i)printf("%d ",ZERO[i]);
			////printf("\n");
			make_stairs(A,n,ZERO,&ind);
			correct_max(ZERO,a,n,&max_k,&ind);

			//print_mat(A,n,n);
			////printf("%d\n",ind);
			
			//if(ind>0 && ind==ind_prev)ind-=1;


		}
		//printf("a");
		a+=1;
		correct_max(ZERO,a,n,&max_k,&ind);

		

		
	}


	
	for(int i=0;i<n;++i){
		det*=A[i][i];
	}
	return det;
	
}


void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {
}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.
void swap_rows_2(int indices[],int n, int i,int j){
	int temp=indices[i];
	indices[i]=indices[j];
	indices[j]=temp;

}
void make_stairs_2(double A[][SIZE],int n,int ZERO[],int *ind,int indices[]){
	for(int i=0;i<(n-1);++i){
		for(int k=0;k<n;++k)
		printf("i: %d %d\n",k,indices[k]);
		for(int j=i+1;j<n;++j){
			
			if(ZERO[indices[i]]>ZERO[indices[j]]){
				printf("%d %d\n",indices[i],indices[j]);

				swap_rows_2(indices,n,i,j);
				int temp=ZERO[indices[i]];
				ZERO[indices[i]]=ZERO[indices[j]];
				ZERO[indices[j]]=temp;
					for(int k=0;k<(n);++k)printf("%d\n",ZERO[indices[k]]);

				if(i==*ind)*ind=j;
				else if(j==*ind)*ind=i;
				
			}


		}
		printf("\n");
	}
	for(int i=0;i<(n);++i)printf("%d\n",ZERO[indices[i]]);
	//return ind;
}
void correct_max_2(int ZERO[],int a,int n,int *max_k,int *ind,int indices[]){
	*max_k=1000;
	for(int i=a;i<n;++i){
		//printf("a");
		if(ZERO[indices[i]]<=*max_k){
			*max_k=ZERO[indices[i]];
			*ind=indices[i];
			//printf("%d %d\n",*ind,*max_k);
		}
	}
}
int is_not_diagonal_2(int ZERO[],int n,int indices[]){
	for(int i=0;i<n;++i){
		// for(int j=0;j<i;++j){
		// 	if(A[i][j]!=0) return 1;
		// }
		if(ZERO[indices[i]]<indices[i])return 1;
	}
	return 0;
}

void print_mat_2(double A[][SIZE], int m, int n,int indices[]) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[indices[i]][j]);
		}
		printf("\n");
	}
}
double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) {
	
	int indices[n];
	
	for(int i=0;i<n;++i){
		indices[i]=i;
		//printf("a");
	}
		
	
	int c;
	int ZERO[SIZE];
	int max_k=1000;
	int ind=0,a=0;
	double multiplier;
	double det=1;

	for(int i=0;i<n;++i){
		int j=0;
		while(j<n && A[indices[i]][j]==0)j+=1;
		ZERO[indices[i]]=indices[j];
		if((ZERO[indices[i]])<=max_k){
			max_k=j;
			ind=indices[i];
		}
		printf("%d\n",j);
	}
	make_stairs_2(A,n,ZERO,&ind,indices);
	
	while((is_not_diagonal_2(ZERO,n,indices))){
		while(indices[ind]>0 && ZERO[indices[ind-1]]==ZERO[indices[ind]]){
			//if(zero_on_diag(A,n))return NAN;
			if(abs(A[indices[ind]][ZERO[indices[ind]]])>abs(A[indices[ind-1]][ZERO[indices[ind-1]]])){
				multiplier=(-1)*A[ind][ZERO[ind]]/A[ind-1][ZERO[ind-1]];
				printf("%lf\n",multiplier);
				multiply_row(A,n,indices[ind-1],multiplier);
				//print_mat(A,n,n);

				swap_rows_2(indices,n,ind,ind-1);
				det*=(-1);
			}
			else{
				multiplier=(-1)*A[indices[ind-1]][ZERO[indices[ind-1]]]/A[indices[ind]][ZERO[indices[ind]]];
				multiply_row(A,n,indices[ind],multiplier);
				// swap_rows(A,n,ind,ind-1);

			}
			det*=1/multiplier;
			printf("%d\n",ind);
			//int ind_prev=ind;
			add_rows(A,n,indices[ind-1],indices[ind]);
			int j=0;
			while(j<n && A[indices[ind]][j]==0)j+=1;
			ZERO[indices[ind]]=j;
			printf("1  :\n");
			print_mat_2(A,n,n,indices);
			printf("\n");

			print_mat(A,n,n);


			for(int i=0;i<n;++i)printf("%d ",ZERO[indices[i]]);
			printf("\n");
			make_stairs_2(A,n,ZERO,&ind,indices);
			correct_max_2(ZERO,a,n,&max_k,&ind,indices);
			print_mat_2(A,n,n,indices);
			printf("\n");

			print_mat(A,n,n);
			//print_mat(A,n,n);
			printf("%d\n",ind);
			
			//if(ind>0 && ind==ind_prev)ind-=1;


		}
		//printf("a");
		a+=1;
		correct_max_2(ZERO,a,n,&max_k,&ind,indices);

		

		
	}


	
	// for(int i=0;i<n;++i){
	// 	det*=A[i][i];
	// }
	// return det;

}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			det = matrix_inv(A, B, n, eps);
			printf("%.4f\n", det);
			if (det) print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

