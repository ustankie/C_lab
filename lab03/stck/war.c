#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


int rand_from_interval(int a, int b) {
	int x;

	if(b<a){
		return INT_MIN;
	}
	if((b-a)>RAND_MAX){
		return INT_MAX;
	}
	
	x=rand()%(b-a+1)+a;
	return x;
}

void swap (int array[], int i, int k) {
	int temp=array[i];
	array[i]=array[k];
	array[k]=temp;
}

// random permutation of integers from [0, n-1]
void rand_permutation(int n, int array[]) {
	int k;
	for(int i=0;i<n;++i){
		array[i]=i;
	}
	for(int i=0;i<(n-1);++i){
		k=rand_from_interval(i,n-1);
		swap(array,i,k);
	}
}

#define STACK_SIZE 10
enum state { OK = 0, UNDERFLOW = -1, OVERFLOW = -2 };

int stack[STACK_SIZE];
int top = 0;

double stack_push(double x) {
	if(top<STACK_SIZE){
		stack[top]=x;
		++top;
		return OK;
		
	}
	return OVERFLOW;
	
}

int stack_pop(void) {
	if(top==0){
		return UNDERFLOW;
	}
	--top;
	double x=stack[top];
	return x;
}

int stack_state(void) {
	return top;
}

// FIFO queue with shifts

#define QUEUE_SIZE 10

int queue[QUEUE_SIZE];
int in = 0, curr_nr = 0;

double queue_push(int in_nr) { // in_nr clients try to enter the queue
	
	for(int i=0;i<in_nr;++i){
		if(in==QUEUE_SIZE){
			//printf("%d\n",in);
			curr_nr+=in_nr-i;
			return OVERFLOW;
		}
		++curr_nr;
		queue[in]=curr_nr;
		++in;
		
		


	}
	return OK;
}

int queue_pop(int out_nr) {
	// int ov=0;
	if(in<out_nr){
		in=0;
		return UNDERFLOW;
	}
	in-=out_nr;
	for(int i=0;i<(in);++i){
		queue[i]=queue[i+out_nr];
	}

	//in-=out_nr;
	return in;
}

int queue_state(void) {
	return in;
}

void queue_print(void) {
	for(int i=0;i<in;++i){
		printf("%d ",queue[i]);
	}

}

// Queue with cyclic buffer

#define CBUFF_SIZE 52

int A[CBUFF_SIZE];
int outA = 0, lenA = 0;

double A_push(int cli_nr) {
	if(lenA==CBUFF_SIZE){
		return OVERFLOW;
	}
	A[(outA+lenA)%CBUFF_SIZE]=cli_nr;
	++lenA;
	return OK;

}

int A_pop(void) {
	if(lenA==0){
		return UNDERFLOW;
	}
	--lenA;
	double r=A[outA];
	outA=(outA+1)%CBUFF_SIZE;
	return r;
}

int A_state(void) {
	return lenA;
}

void A_print(void) {
	for(int i=0;i<lenA;++i){
		printf("%d ",A[(outA+i)%CBUFF_SIZE]);

		}
	
	
}


#define CBUFF_SIZE 52

int B[CBUFF_SIZE];
int outB = 0, lenB = 0;

double B_push(int cli_nr) {
	if(lenB==CBUFF_SIZE){
		return OVERFLOW;
	}
	B[(outB+lenB)%CBUFF_SIZE]=cli_nr;
	++lenB;
	return OK;

}

int B_pop(void) {
	if(lenB==0){
		return UNDERFLOW;
	}
	--lenB;
	double r=B[outB];
	outB=(outB+1)%CBUFF_SIZE;
	return r;
}

int B_state(void) {
	return lenB;
}

void B_print(void) {
	for(int i=0;i<lenB;++i){
		printf("%d ",B[(outB+i)%CBUFF_SIZE]);

		}
	
	
}

int main(void) {
    int seed, version, max_confl;
    int a,b,a_c,b_c;
    scanf("%d%d%d",&seed,&version,&max_confl);
    srand((unsigned) seed);
    int confl=0;
    int T[52],a_p[52],b_p[52];

    rand_permutation(52,T);

    for(int i=0;i<26;++i){
        A_push(T[i]);
    }
    
    for(int i=26;i<52;++i){
        B_push(T[i]);
    }

    switch(version){
        case 0:{
                while(confl<max_confl && lenA>0 && lenB>0){
                    a_c=A_pop();
                    b_c=B_pop();
                    a=a_c/4;
                    b=b_c/4;
                    //printf("A: %d %d B: %d %d, confl: %d\n",a_c,a,b_c,b,confl);
                    ++confl;
                    if(a<b){
                        B_push(b_c);
                        B_push(a_c);
                    }
                    else if (b<a){
                        A_push(a_c);
                        A_push(b_c);
                    }
                    else{ //a==b
                            //printf("war!!!\n");
                            int j=0;
                            int lenA_t=lenA+1;
                            int lenB_t=lenB+1;
                            while(confl<max_confl && b==a && lenA>0 && lenB>0){
                                a_p[j]=a_c;
                                b_p[j]=b_c;
                                ++j;

                                a_c=A_pop();
                                b_c=B_pop();
                                //printf("      A: %d  B: %d confl: %d\n",a_c,b_c,confl);
                                if((lenA==0 || lenB==0)&& b==a && confl<max_confl){
                                    printf("%d %d %d",1,lenA_t,lenB_t);
                                    return 0;
                                }
                                a_p[j]=a_c;
                                b_p[j]=b_c;
                                ++j;
                                if((lenA==0 || lenB==0)&& b==a && confl<max_confl){
                                    printf("%d %d %d",1,lenA_t,lenB_t);
                                    return 0;
                                }
                                a_c=A_pop();
                                b_c=B_pop();
                                a=a_c/4;
                                b=b_c/4;  
                                //printf("      A: %d %d B: %d %d confl: %d\n",a_c,a,b_c,b,confl);

                                a_p[j]=a_c;
                                b_p[j]=b_c;
                                ++j;

                                ++confl;                      


                            }


                            if(b>a){
                                for(int i=0;i<j;++i){
                                    B_push(b_p[i]);
                                    
                                }
                                for(int i=0;i<j;++i){
                                    B_push(a_p[i]);
                                    
                                }
                            }

                            else{
                                for(int i=0;i<j;++i){
                                    A_push(a_p[i]);
                                    
                                }
                                for(int i=0;i<j;++i){
                                    A_push(b_p[i]);
                                    
                                }
                            

                            }


                    }
                }
            if(confl==max_confl){
                printf("%d %d %d",0,lenA,lenB);
                break;
            }
            if(lenA==0){
                printf("%d\n",3);
                B_print();
                break;
            }
            if(lenB==0){
                printf("%d %d",2,confl);
                break;
            }
            //printf("blad");
            break;
        }
        case 1:{
            while(confl<max_confl && lenA>0 && lenB>0){
                a_c=A_pop();
                b_c=B_pop();
                a=a_c/4;
                b=b_c/4;
                //printf("A: %d %d B: %d %d, confl: %d\n",a_c,a,b_c,b,confl);
                ++confl;
                if(a<b){
                    B_push(b_c);
                    B_push(a_c);
                }
                else if (b<a){
                    A_push(a_c);
                    A_push(b_c);
                }
                else{
                    A_push(a_c);
                    B_push(b_c);
                }

            }


            if(confl==max_confl){
                printf("%d %d %d",0,lenA,lenB);
                break;
            }
            
            if(lenA==0){
                printf("%d\n",3);
                B_print();
                break;
            }
            if(lenB==0){
                printf("%d %d",2,confl);
                break;
            }
            //printf("blad");
            break;

            
        }
            


    }

	return 0;
}
