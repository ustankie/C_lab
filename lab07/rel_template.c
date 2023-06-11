#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct Pair {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair*, int, pair);

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(const pair*, int);

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(const pair*, int);

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(const pair*, int);

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(const pair*, int);

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(const pair*, int);

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(const pair*, int);

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(const pair*, int);

// A total order relation is a partial order relation that is connected
int is_total_order(const pair*, int);

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
//int is_connected(const pair*, int);

int find_max_elements(const pair*, int, int*);
int find_min_elements(const pair*, int, int*);
int get_domain(const pair*, int, int*);

// Case 3:

int composition (const pair*, int, const pair*, int, pair*);

// Comparator for pair
int cmp_pair (const void *a, const void *b) {
	int val1=*(int *)a;
	int val2=*(int *)b;
	return val1-val2;
}

// int insert_int (int *tab, int n, int new_element) {
// }

// Add pair to existing relation if not already there


//case 1
int is_reflexive(const pair* T, int n){
	int ref[2*n][2];
	int p=0;
	for(int i=0;i<n;++i){
		int Flag1=1;
		int Flag2=1;
		int ind=0;
		for(int j=0;j<p;++j){
			if(ref[j][0]==T[i].first){
				Flag1=0;
				ind=j;
			}
			if(ref[j][0]==T[i].second){
				Flag2=0;
				ind=j;
			}
		}
		if(T[i].first==T[i].second){
			Flag2=0;
		}
		if(Flag1==1){
			ref[p][0]=T[i].first;
			ref[p][1]=0;
			ind=p;
			p+=1;
		}
		if(Flag2==1){
			ref[p][0]=T[i].second;
			ref[p][1]=0;
			ind=p;
			p+=1;
		}

		if(T[i].first==T[i].second){
				ref[ind][1]=1;
				
		}		
		
	}

	for(int i=0;i<p;++i){
		if(ref[i][1]==0)return 0;
	}
	return 1;
	
}


int is_irreflexive(const pair* T, int n){	
	int ref[2*n][2];
	int p=0;
	for(int i=0;i<n;++i){
		int Flag1=1;
		int Flag2=1;
		int ind=0;
		for(int j=0;j<p;++j){
			if(ref[j][0]==T[i].first){
				Flag1=0;
				ind=j;
			}
			if(ref[j][0]==T[i].second){
				Flag2=0;
				ind=j;
			}
		}
		if(T[i].first==T[i].second){
			Flag2=0;
		}
		if(Flag1==1){
			ref[p][0]=T[i].first;
			ref[p][1]=0;
			ind=p;
			p+=1;
		}
		if(Flag2==1){
			ref[p][0]=T[i].second;
			ref[p][1]=0;
			ind=p;
			p+=1;
		}

		if(T[i].first==T[i].second){
				ref[ind][1]=1;
				
		}		
		
	}

	for(int i=0;i<p;++i){
		if(ref[i][1]==1)return 0;
	}
	return 1;
	
}


int is_symmetric(const pair* T, int n){
	int s[n];
	for(int i=0;i<n;++i){
		s[i]=0;
	}

	for(int i=0;i<(n-1);++i){
		for(int j=i;j<n;++j){
			if(T[i].first==T[j].second && T[i].second==T[j].first){
				s[i]=1;
				s[j]=1;
			}
		}
	}
	for(int i=0;i<n;++i){
		if(s[i]==0){
			return 0;
		}
	}
	return 1;
}


int is_antisymmetric(const pair* T, int n){

	for(int i=0;i<(n-1);++i){
		for(int j=i;j<n;++j){
			if(T[i].first==T[j].second && T[i].second==T[j].first && T[i].first!=T[i].second){
				return 0;
			}
		}
	}

	return 1;
}


int is_asymmetric(const pair* T, int n){

	for(int i=0;i<(n-1);++i){
		for(int j=i;j<n;++j){
			if(T[i].first==T[j].second && T[i].second==T[j].first){
				return 0;
			}
		}
	}

	return 1;
}


int is_transitive(const pair* T, int n){
	//printf("\n");
	int ref[2*n][2*n+1];
	int b=2*n;
	for(int i=0;i<b;++i){
		for(int j=0;j<b;++j){
			ref[i][j]=0;
			//printf("a");
		}
	}
		

	int p=0;
	for(int i=0;i<n;++i){
		int Flag1=1;
		//int Flag2=1;
		int ind=0;
		for(int j=0;j<p;++j){
			if(ref[j][0]==T[i].first){
				Flag1=0;
				ind=j;
			}
		}

		if(Flag1==1){
			ref[p][0]=T[i].first;
			ref[p][1]=2;
			ind=p;
			p+=1;
		}
		
		ref[ind][ref[ind][1]]=T[i].second;
		ref[ind][1]++;
	}
	// for(int i=0;i<b;++i){
	// 	for(int j=0;j<b;++j){
	// 		printf("%d ",ref[i][j]);
	// 	}
	// 	printf("\n");
	// }
	

	for(int i=0;i<p;++i){
		int x=ref[i][0];
		for(int j=2;j<ref[i][1];++j){
			int y=ref[i][j];
			
			for(int k=0;k<p;++k){

				if(ref[k][0]==y){
					for(int l=2;l<ref[k][1];++l){
						int z=ref[k][l];
						int Flag=0;
						//int e=0;

						for(int m=2;m<ref[i][1];++m){
							if(ref[i][m]==z){
								Flag=1;
								break;
							}
							//e=1;
								
							
						}//printf("%d %d %d %d\n",x,y,z,Flag);
						//printf("a");
						if(Flag==0){
							return 0;
						
						}
						

					}
				}
			}


			
		}
	}
	return 1;

	
}
int is_consistent(const pair* T,int n){
	int ref[2*n+1][2*n+1];
	int b=2*n+1;
	for(int i=0;i<b;++i){
		for(int j=0;j<b;++j){
			ref[i][j]=0;
		}
	}


	int p=1;
	for(int i=0;i<n;++i){
		int Flag1=1;
		int Flag2=1;
		int ind=0;
		int ind2=0;
		for(int j=0;j<p;++j){
			if(ref[j][0]==T[i].first){
				Flag1=0;
				ind=j;
			}
			if(ref[j][0]==T[i].second){
				Flag2=0;
				ind2=j;
			}
		}
		if(T[i].first==T[i].second){
			Flag2=0;
		}
		if(Flag1==1){
			ref[p][0]=T[i].first;
			ref[0][p]=T[i].first;
			ind=p;
			p+=1;
		}
		if(Flag2==1){
			ref[p][0]=T[i].second;
			ref[0][p]=T[i].second;
			ind2=p;
			p+=1;
		}

		ref[ind][ind2]=1;
		ref[ind2][ind]=1;	
		
	}

	for(int i=0;i<p;++i){
		for(int j=0;j<p;++j){
			//printf("%d ",ref[i][j]);
			if(ref[i][j]==0 && i!=j){
				//printf("\n");
				return 0;
			}
		}
		//printf("\n");
	}
	return 1;
	
}
//case 2 

int is_partial_order(const pair* T, int n){
	return(is_reflexive(T,n) && is_antisymmetric(T,n) && is_transitive(T,n));
}

int is_total_order(const pair* T, int n){
	return(is_partial_order(T,n) && is_consistent(T,n));
}


//int is_connected(const pair*, int){return 0;}

int find_max_elements(const pair* T, int n, int* max_el){
	int m=0;
	for(int i=0;i<n;++i){
		int s=T[i].second;
		//int Flag=0;
		int j=0;
		while(j<n && (T[j].first!=s || T[j].first==T[j].second)){
			j++;
		}
		if(j==n){
			int k=0;
			while(k<m && max_el[k]<s){
				k++;
			}
			//printf("%d\n",k);
			if((k<m && max_el[k]!=s)|| k==m){
				max_el[m]=s;
				qsort(max_el,m,sizeof(int),cmp_pair);
				//print_int_array(max_el,m);
				//printf("%d\n",s);
				m++;
			}
	
		}

		
	}
	return m;
}
int find_min_elements(const pair* T, int n, int* max_el){	
	int m=0;
	for(int i=0;i<n;++i){
		int s=T[i].first;
		//int Flag=0;
		int j=0;
		while(j<n && (T[j].second!=s || T[j].first==T[j].second)){
			j++;
		}
		if(j==n){
			int k=0;
			while(k<m && max_el[k]<s){
				k++;
			}
			//printf("%d\n",k);
			if((k<m && max_el[k]!=s)|| k==m){
				max_el[m]=s;
				qsort(max_el,m+1,sizeof(int),cmp_pair);
				//print_int_array(max_el,m);
				//printf("%d\n",s);
				m++;
			}
	
		}

		
	}
	return m;
}
int get_domain(const pair* T, int n, int* domain){
	int m=0;
	for(int i=0;i<n;++i){
		int s=T[i].first;
		int j=0;
		while(j<m && domain[j]<s){
			j++;
		}
		if((j<m && domain[j]!=s)|| j==m){
			domain[m]=s;
			qsort(domain,m+1,sizeof(int),cmp_pair);
			m++;
		}
		s=T[i].second;
		j=0;
		while(j<m && domain[j]<s){
			j++;
		}
		if((j<m && domain[j]!=s)|| j==m){
			domain[m]=s;
			qsort(domain,m+1,sizeof(int),cmp_pair);
			m++;
		}
	}
	return m;
}

//case 3
int composition (const pair* T1, int n1, const pair* T2, int n2, pair* R){
	int m=0;
	for(int i=0;i<n1;++i){
		int x=T1[i].first;
		int y=T1[i].second;
		for(int j=0;j<n2;++j){
			if(T2[j].first==y){
				pair a;
				a.first=x;
				a.second=T2[j].second;
				//printf("%d %d\n",a.first,a.second);
				m=add_relation(R,m,a);
			}
		}
	}
	//printf("\n");
	// for(int i=0;i<m;++i){
	// 	printf("%d %d\n",R[i].first,R[i].second);
	// }
	return m;
}




int add_relation (pair *tab, int n, pair new_pair) {
	int Flag=0;
	for(int i=0;i<n;++i){
		if (new_pair.first==tab[i].first && new_pair.second==tab[i].second){
			Flag=1;
			break;
		}
	}

	if (Flag==0){
		tab[n].first=new_pair.first;
		tab[n].second=new_pair.second;
		//printf("%d %d\n",tab->first,tab->second);

		n++;
	}
	return n;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
	int n;
	scanf("%d",&n);
	for(int i=0;i<n;++i){
		scanf("%d", &(relation[i].first));
		scanf("%d", &(relation[i].second));
		// printf("%d %d\n",relation->first,relation->second);
	}
	return n; //struct->field == (*struct).field

}

void print_int_array(const int *array, int n) {
	printf("%d\n",n);
	for(int i=0;i<n;++i){
		printf("%d ",array[i]);
	}
	printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;
	// pair a;
	// a.first=1;
	// a.second=2;
	//add_relation(relation_2,0,a);
	//printf("a");

	// for(int i = 0; i < size; i++) {
	// 	printf("%d %d", (relation[i]).first, (relation[i]).second);
	// }
	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			//printf("%d\n", is_consistent(relation, size));
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

