#include <stdio.h>
#include <math.h>

#define RECURS_LEVEL_MAX  10
#define N_MAX             10

// pointer to function of one variable
typedef double (*Func1vFp) (double);

// example functions of one variable

double f_poly(double x) { // polynomial a[0] + a[1]x + ... + a[n]x^n
	return (2*pow(x,5)-4*pow(x,4)+3.5*x*x+1.35*x-6.25);
}

double f_rat(double x) {
	double a=(x-0.5)*(x-0.5);
	a+=0.01;
	//printf("%lf",a);
	return (1/a);
}

double f_exp(double x) {
	double a=2*x*exp(-1.5*x);
	return a-1;
}

double f_trig(double x) {
	return x*tan(x);
}

// Quadratures

// rectangle rule, leftpoint
double quad_rect_left(Func1vFp f1, double a, double b, int n) {
	double step=(b-a)/n;
	double sum=0;
	for(double h=a;h<b;h+=step){
		sum+=f1(h);
	}
	sum*=step;
	return sum;
}

// rectangle rule, rightpoint
double quad_rect_right(Func1vFp f1, double a, double b, int n) {
	double step=(b-a)/n;
	double sum=0;
	double m=step*n+a;
	for(double h=a+step;h<(b+step);h+=step){
		double k=f1(h);
		sum+=k;
		//printf("%lf %lf\n",k,h);
	}
	sum*=step;
	return sum;
}

// rectangle rule, midpoint
double quad_rect_mid(Func1vFp f1, double a, double b, int n) {
	double step=(b-a)/n;
	double sum=0;
	double c=(step+2*a)/2;
	for(double h=c;h<b;h+=step){
		sum+=f1(h);
	}
	sum*=step;
	return sum;
}

// trapezoidal rule
double quad_trap(Func1vFp f1, double a, double b, int n) {
	double step=(b-a)/n;
	double sum=0;
	
	double left=f1(a);
	for(double h=a+step;h<(b+step);h+=step){
		double right=f1(h);
		sum+=right+left;
		left=right;

	}

	return ((sum*step)/2);
}

// Simpson's rule
double quad_simpson(Func1vFp f1, double a, double b, int n) {
	double step=(b-a)/n;
	double sum=0;
	double c;

	double left=f1(a);
	//printf("%lf\n",a);
	for(double h=step+a,c=((step+a+a)/2);h<(b+step);h+=step,c+=step){
		double right=f1(h);
		double mid=f1(c);
		//printf("%lf %lf\n",c,h);
		//printf("%lf %lf\n",mid,right);
		sum+=right+left+4*mid;
		left=right;

	}

	return ((sum*step)/6);
}

// pointer to quadrature function
typedef double (*QuadratureFp) (Func1vFp,double,double,int);

// array of pointers to integrand functions
Func1vFp func_tab[] = { f_poly, f_rat, f_trig, f_exp };

// array of pointers to quadrature functions
QuadratureFp quad_tab[] = {
	quad_rect_left, quad_rect_right, quad_rect_mid, quad_trap, quad_simpson };

// calls 'quad_no' quadrature function for 'fun_no' integrand function
// on interval [a, b] and n subintervals
double quad_select(int fun_no, int quad_no, double a, double b, int n) {
	return (quad_tab)[quad_no]((func_tab)[fun_no],a,b,n);
}

// adaptive algorithm
double recurs(Func1vFp f, double a, double b, double S, double delta, QuadratureFp quad, int level) {
	
	if(level==RECURS_LEVEL_MAX){
		//printf("%lf %lf\n",level,S);
		return NAN;
	}	
	// for(int i=0;i<level;++i){
	// 	printf(" ");
	// }

	//printf("%lf %lf,%lf\n",S,a,b);
	double S1=quad(f,a,((a+b)/2),1);
	double S2=quad(f,((a+b)/2),b,1);
	// for(int i=0;i<level;++i){
	// 	printf(" ");
	// }
	// printf("|");
	// printf("%lf %lf %lf %lf\n",S1,S2,fabs(S1+S2-S),delta);
	if(fabs(S1+S2-S)<=delta){
		//printf("				%lf %lf,%lf\n",S,a,b);
		return S1+S2;
	}
	else{
		return recurs(f,a,((a+b)/2),S1,delta/2,quad,level+1)+recurs(f,((a+b)/2),b,S2,delta/2,quad,level+1);
	}

}

// initialization for adaptive algorithm
double init_recurs(Func1vFp f, double a, double b, double delta, QuadratureFp quad) {
	return recurs(f,a,b,quad(f,a,b,1),delta,quad,0);
}

// double integrals

// pointer to function of two variables
typedef double (*Func2vFp) (double,double);

double func2v_2(double x, double y) {
	return (2 - x*x - y*y*y);
}

// sample functions to define the normal domain

double lower_bound2(double x) {
	return 0.7*exp(-2*x*x);
}
double upper_bound2(double x) {
	return sin(10*x);
}

// rectangle rule (leftpoint) - double integral over rectangular domain
double dbl_integr(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny) {
	//printf("%ld",quad_rect_left(f,y1,y2,ny));
	//printf("%lf \n",(f)(2,0));
	double step1=(x2-x1)/nx;
	double step2=(y2-y1)/ny;
	double sum1=0;
	double h=x1;
	


	for(int i=0;i<nx;i+=1){
		double h2=y1;
		for(int j=0;j<ny;j+=1){
			sum1+=f(h,h2);
			//printf("%lf\n",h2);
			h2+=step2;
		}
		h+=step1;
	}
	sum1*=step1*step2;

	return sum1;
}

// rectangle rule (midpoint) - double integral over normal domain with respect to the x-axis
double dbl_integr_normal_1(Func2vFp f, double x1, double x2, int nx, double hy, Func1vFp fg, Func1vFp fh) {
	int ny=0;
	double stepx=(x2-x1)/nx;
	double stepy;
	double sum=0;
	double c1=(2*x1+stepx)/2;

	for(double i=c1;i<x2;i+=stepx){
		double a=fg(i);
		double b=fh(i);
		ny=ceil((b-a)/hy);
		stepy=(b-a)/ny;
		double c2=(2*a+stepy)/2;
		for(double j=c2;j<fh(i);j+=stepy){
			sum+=(f(i,j)*stepy);
		}
	}

	sum*=stepx;
	return sum;
}


// rectangle rule (leftpoint) - double integral over multiple normal
// domains with respect to the x-axis
double dbl_integr_normal_n(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny, Func1vFp fg, Func1vFp fh)  {

	double stepx=(x2-x1)/nx;
	double stepy;
	double sum=0;
	double hy=(y2-y1)/ny;
    double i=x1;
	
	for(int v=0;v<nx;v+=1){

		double b=fh(i);
		double a=fg(i);
        double curr_sum=0;

//		if(a<b){
			double k=(y2>b ? b:y2);
			double p=(y1<a ? a:y1);	
			if(p < k) {
            double j=p;	
            curr_sum=0;
            
			ny=ceil((k-p)/hy);
			stepy=(k-p)/ny;
            
			for(int u=0;u<ny;u+=1){
				//if(p<j && j<k)
				curr_sum+=(f(i,j));
                j+=stepy;
			} 
            curr_sum*=stepy;
            sum+=curr_sum;
		}	
        i+=stepx;
	}
	sum*=stepx;
	return sum;
}

// multiple quadratures

typedef double (*FuncNvFp)(const double*, int);
typedef int (*BoundNvFp)(const double*, int);

// sample function of three variables
double func3v(const double v[], int n) {
	return v[0] - v[1] + 2*v[2];
}

// sample predicate in 3D
int bound3v(const double v[], int n) { // a cylinder
	return v[0] > 0 && v[0] < 0.5 && v[1]*v[1] + (v[2]-1)*(v[2]-1) < 1;
}

// sample function of n variables
double funcNv(const double v[], int n) {
	double fv = 1.;
	for (int i = 1; i < n; ++i) {
		fv += sin(i*v[i]);
	}
	return fv;
}
// sample n-dimensional predicate (n-dim hypersphere)
int boundNv(const double v[], int n) {
	double r = 0.0;
	for (int i = 0; i < n; ++i) r += (v[i]-1)*(v[i]-1);
	return r <= 1.;
}

// multiple integrals over a cuboid with predicate (if boundary != NULL)
// rectangular rule (rightpoint)
double trpl_quad_rect(FuncNvFp f, double variable_lim[][2], const int tn[], BoundNvFp boundary) {
	double stepx=(variable_lim[0][1]-variable_lim[0][0])/tn[0];
	double stepy=(variable_lim[1][1]-variable_lim[1][0])/tn[1];
	double stepz=(variable_lim[2][1]-variable_lim[2][0])/tn[2];
	double v[3];
	int n=3;
	double sum=0;

	double x=(variable_lim[0][0]+stepx);

	for(int i=0;i<tn[0];++i){
		double y=(variable_lim[1][0]+stepy);
		for(int j=0;j<tn[1];j++){
			double z=(variable_lim[2][0]+stepz);
			for(int k=0;k<tn[2];++k){
				v[0]=x;
				v[1]=y;
				v[2]=z;
				if ((boundary!=NULL && boundary(v,n))||boundary==NULL){ 
					sum+=f(v,n);
				}
			z+=stepz;
			}
			y+=stepy;
		}
		x+=stepx;
	}
	sum=sum*stepx*stepy*stepz;
	return sum;
}

// multiple integrals over a n-dim hypercuboid with predicate (if boundary != NULL)
// rectangular rule (midpoint)
void recur_quad_rect_mid(double *p_sum, FuncNvFp f, int variable_no, double t_variable[],double variable_lim[][2], const int tn[], int level, BoundNvFp boundary) {
	if(level==variable_no){
		if(boundary!=NULL && boundary(t_variable,variable_no)){
			double a=f(t_variable,variable_no);
			*p_sum+=a;
		}
		else if(boundary==NULL){
			double a=f(t_variable,variable_no);
			*p_sum+=a;
		}		
	}
	else{
		double step=(variable_lim[level][1]-variable_lim[level][0])/tn[level];
		double c=((2*variable_lim[level][0])+step)/2;
		for(double i=c;i<variable_lim[level][1];i+=step){
			t_variable[level]=i;
			recur_quad_rect_mid(p_sum,f,variable_no,t_variable,variable_lim,tn,level+1,boundary);
		}
	}
	if(level==0){
		for(int i=0;i<variable_no;++i){
			(*p_sum)*=(variable_lim[i][1]-variable_lim[i][0])/tn[i];
		}
	}
}

int main(void) {
	int to_do, n, nx, ny, integrand_fun_no, method_no, flag;
	int no_funcs = sizeof(func_tab) / sizeof(Func1vFp);
	int no_quads = sizeof(quad_tab) / sizeof(QuadratureFp);
	double a, b, x1, x2, y1, y2, hy, sum, delta;
	double t_variable[N_MAX], variable_lim[N_MAX][2];
	int tn[N_MAX];
	// printf("%lf\n",quad_simpson(f_poly,1,4,10));
	// printf("%lf\n",quad_rect_left(f_poly,1,4,10));
	// printf("%lf\n",quad_rect_right(f_poly,1,4,10));
	// printf("%lf\n",quad_rect_mid(f_poly,1,4,10));
	// printf("%lf\n",quad_trap(f_poly,1,4,10));
 	//printf("%lf\n",func2v_2(2,2));

	scanf("%d", &to_do);
	switch (to_do) {
		case 1: // loop over quadratures and integrands
			scanf("%lf %lf %d", &a, &b, &n);
			for(int q = 0; q < no_quads; ++q) {
				for(int f = 0; f < no_funcs; ++f) {
					printf("%.5f ",quad_select(f, q, a, b, n));
				}
				printf("\n");
			}
			break;
		case 2: // adaptive algorithm
			scanf("%d %d",&integrand_fun_no,&method_no);
			scanf("%lf %lf %lf", &a, &b, &delta);
			printf("%.5f\n", init_recurs(func_tab[integrand_fun_no],a,b,delta,quad_tab[method_no]));
			break;
		case 3: // double integral over a rectangle
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf %lf %d", &y1, &y2, &ny);
			printf("%.5f\n", dbl_integr(func2v_2, x1, x2, nx, y1, y2, ny));
			break;
		case 4: // double integral over normal domain
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf", &hy);
			printf("%.5f\n", dbl_integr_normal_1(func2v_2, x1, x2, nx, hy, lower_bound2, upper_bound2));
			break;
		case 5: // double integral over multiple normal domains
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf %lf %d", &y1, &y2, &ny);
			printf("%.5f\n",dbl_integr_normal_n(func2v_2, x1, x2, nx, y1, y2, ny, lower_bound2, upper_bound2));
			break;
		case 6: // triple integral over a cuboid
			scanf("%lf %lf %d", &variable_lim[0][0], &variable_lim[0][1], tn);
			scanf("%lf %lf %d", &variable_lim[1][0], &variable_lim[1][1], tn+1);
			scanf("%lf %lf %d", &variable_lim[2][0], &variable_lim[2][1], tn+2);
			scanf("%d", &flag);
			printf("%.5f\n", trpl_quad_rect(func3v, variable_lim, tn, flag ? bound3v : NULL));
			break;
		case 7: // multiple integral over hyper-cuboid
			scanf("%d", &n);
			if(n > N_MAX) break;
			for(int i = 0; i < n; ++i) {
				scanf("%lf %lf %d", &variable_lim[i][0], &variable_lim[i][1], tn+i);
			}
			scanf("%d", &flag);
			sum = 0.;
			recur_quad_rect_mid(&sum, funcNv, n, t_variable, variable_lim, tn, 0, flag ? boundNv : NULL);
			printf("%.5f\n", sum);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}
	return 0;
}

