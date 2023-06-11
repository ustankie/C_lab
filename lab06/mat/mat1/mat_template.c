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
			if(A[i][j]>(-0.00005)&&A[i][j]<=(0.0))printf("%.4f ",(-0.0000));
			else
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
	double det=1;
	double multiplier=0;
	for(int i=0;i<(n-1);++i){
		if(A[i][i]==0)return NAN;
		for(int j=i+1;j<n;++j){
			if(A[j][i]==0)multiplier=0;
			else multiplier=A[j][i]/A[i][i];
			for (int k=i;k<n;k++){
                A[j][k]-=multiplier*A[i][k];
            }
		}
	}
	//print_mat(A,n,n);

	
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
void swap_rows_2(int indices[],int n, int i,int j,int ZERO[]){
	int temp=indices[i];
	indices[i]=indices[j];
	indices[j]=temp;
    temp=ZERO[i];
    ZERO[i]=ZERO[j];
    ZERO[j]=temp;
    //printf("aa");

}
void make_stairs_2(double A[][SIZE],int n,int ZERO[],int *ind,int indices[]){
    int temp;
	for(int i=0;i<(n-1);++i){
		// for(int k=0;k<n;++k)
		// printf("i: %d %d\n",k,indices[k]);
		for(int j=i+1;j<n;++j){
			
			if(ZERO[i]>ZERO[j]){
				////printf("%d %d\n",i,j);

                ////printf("%d      %d\n",ZERO[i],ZERO[j]);

				swap_rows_2(indices,n,i,j,ZERO);

				////for(int k=0;k<(n);++k)printf("%d\n",ZERO[k]);

				if(i==*ind)*ind=j;
				else if(j==*ind)*ind=i;
				
			}


		}
		////printf("\n");
	}
	////for(int i=0;i<(n);++i)printf("%d\n",ZERO[i]);
	//return ind;
}
void sorting(double A[][SIZE], int n,int indices[],int i,double *det){
	for(int k=i;k<(n-1);++k){
		for(int j=k+1;j<n;++j){
			if(abs(A[indices[k]][i])<abs(A[indices[j]][i])){
				*det*=(-1);
				int temp=indices[i];
				indices[i]=indices[j];
				indices[j]=temp;
			}
		}
	}
}
void correct_max_2(int ZERO[],int a,int n,int *max_k,int *ind,int indices[]){
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
int is_not_diagonal_2(int ZERO[],int n,int indices[]){
	for(int i=0;i<n;++i){
		// for(int j=0;j<i;++j){
		// 	if(A[i][j]!=0) return 1;
		// }
		if(ZERO[i]<i)return 1;
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
	double det=1;
	double multiplier=0;
	double B[n];
	for(int i=0;i<n;++i){
		indices[i]=i;
		B[i]=b[i];
	}
	for(int i=0;i<(n-1);++i){
		// print_mat_2(A,n,n,indices);
		// printf("\n");
		sorting(A,n,indices,i,&det);
		//print_mat_2(A,n,n,indices);
		//for(int j=0;j<n;++j)printf("%lf\n",B[j]);
		//printf("\n");
		if(A[indices[i]][i]<eps)return 0;
		
		for(int j=i+1;j<n;++j){
			if(A[indices[j]][i]==0)
				multiplier=0;
			else 
				multiplier=A[indices[j]][i]/A[indices[i]][i];
			//printf("%lf %lf %lf %d\n",multiplier,A[indices[j]][i],A[indices[i]][i],indices[j]);
			for (int k=i;k<n;k++){
                A[indices[j]][k]-=multiplier*A[indices[i]][k];
            }
			B[indices[j]]-=multiplier*B[indices[i]];
			
		}
	}
	// for(int i=(n-1);i>=0;--i){
    //     //t=B[indices[i]];
	// 	printf("indices: %d\n",indices[i]);
	// }
	// //print_mat(A,n,n);
	// print_mat_2(A,n,n,indices);
	// printf("\n");;
	int count=0;

	for(int i=0;i<n;++i){
		det=det*A[indices[i]][i];
		if(b[i]==0)count+=1;
	}
	if(det==0 || count==n)return 0;
	//for(int j=0;j<n;++j)printf("%lf\n",B[j]);

	//printf("\n");

    double t=0;
    for(int i=(n-1);i>=0;--i){

        t=B[indices[i]];
	// 					for(int j=(n-1);j>=0;--j){
    //     //t=B[indices[i]];
	// 	printf("indices: %d\n",indices[j]);
	// }
		// printf("%lf %d\n",t,indices[i]);
		// 	printf("\n");

        for(int j=(n-1);j>i;--j){
            //printf("%lf %lf\n",A[indices[i]][j],x[j] );
            t-=A[indices[i]][j]*x[j];

        }
        x[i]=t/A[indices[i]][i];
        ////printf("%lf\n",x[indices[i]]);
    }
	return det;

}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.

double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
	int indices[2*n];
	double det=1;
	double multiplier=0;
	double devide=1;
	//double B[n];
	for(int i=0;i<(n);++i){
		indices[i]=i;
		//B[i]=b[i];
	}
	//printf("aaa");
	for(int i=0;i<(n);++i){
		for(int j=n;j<(2*n);++j){
			if((i+n)==j)A[i][j]=1;
			else A[i][j]=0;
			//printf("%lf ",A[i][j]);
		}
		//printf("\n");
	}
	for(int i=0;i<(n-1);++i){
		// print_mat_2(A,n,2*n,indices);
		// printf("\n");
		sorting(A,n,indices,i,&det);
		//print_mat_2(A,n,2*n,indices);
		// //for(int j=0;j<n;++j)printf("%lf\n",B[j]);
		//printf("\n");
		if(abs(A[indices[i]][i])<eps){printf("%d %d ",i,indices[i]);return 0;}
		
		for(int j=i+1;j<(n);++j){
			if(A[indices[j]][i]==0)
				multiplier=0;
			else 
				multiplier=A[indices[j]][i]/A[indices[i]][i];
			//printf("%lf %lf %lf %d\n",multiplier,A[indices[j]][i],A[indices[i]][i],indices[j]);
			for (int k=i;k<(2*n);k++){
                A[indices[j]][k]-=multiplier*A[indices[i]][k];
            }
			//B[indices[j]]-=multiplier*B[indices[i]];
			
		}
		devide=A[indices[i]][i];
		det*=devide;
		//printf("%lf\n",devide);
		for (int k=i;k<(2*n);k++){
			A[indices[i]][k]/=devide;
		}
		// 		print_mat_2(A,n,2*n,indices);
		// printf("\n");

	}
	devide=A[indices[n-1]][n-1];
	det*=devide;
	//printf("%lf\n",devide);
	for (int k=n-1;k<(2*n);k++){
		A[indices[n-1]][k]/=devide;
	}
	// 		print_mat_2(A,n,2*n,indices);
	// printf("\n");
	//  print_mat_2(A,n,2*n,indices);




	for(int i=n-1;i>0;--i){
		// print_mat_2(A,n,n,indices);
		// printf("\n");
		//sorting(A,n,indices,i,&det);
		//print_mat_2(A,n,2*n,indices);
		//for(int j=0;j<n;++j)printf("%lf\n",B[j]);
		//printf("\n");
		if(A[indices[i]][i]<eps)return 0;
		
		for(int j=i-1;j>=0;--j){
			if(A[indices[j]][i]==0)
				multiplier=0;
			else 
				multiplier=A[indices[j]][i]/A[indices[i]][i];
			//printf("%lf %lf %lf %d\n",multiplier,A[indices[j]][i],A[indices[i]][i],indices[j]);
			for (int k=i;k<(2*n);k++){
                A[indices[j]][k]-=multiplier*A[indices[i]][k];
            }
			//B[indices[j]]-=multiplier*B[indices[i]];
			
		}
	}
			//print_mat_2(A,n,2*n,indices);
	for(int i=0;i<(n);++i){
		for(int j=n;j<(2*n);++j){
			B[i][j-n]=A[indices[i]][j];
		}
	}
	if(det==(-0.00000001))det=-0.0000;
	return det;

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

