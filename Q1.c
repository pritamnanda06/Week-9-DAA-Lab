/* Q1: Fractional Knapsack with Deterioration Rate
 * MODEL: items consumed one after another at unit rate; portion consumed at time t has density rho_i - lambda_i*t.
 * 1) ORDER: swapping adjacent blocks i,j changes value by a*b*(lambda_i-lambda_j)  => consume in DECREASING lambda.
 * 2) AMOUNTS: in that order f(x)=sum rho_i x_i - 1/2 sum min(lam_a,lam_b) x_a x_b is CONCAVE on
 *    {0<=x<=w, sum x<=W}; maximise by projected gradient ascent (step 1/sum(lambda)).
 * Complexity: sort O(n log n) + K iterations x O(n log(1/eps)) (O(n) gradient via prefix sums, bisection projection).
 * Input: n W, then n lines: v_i w_i lambda_i
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct { int id; double w, lam, rho; } Item;
static int cmp(const void*a,const void*b){
    const Item*p=a,*q=b;
    if(p->lam!=q->lam) return p->lam>q->lam ? -1 : 1;
    return p->rho>q->rho ? -1 : p->rho<q->rho;
}
static double clip(double t,double hi){ return t<0 ? 0 : t>hi ? hi : t; }
/* project y onto {0<=x<=w, sum x<=W}: bisection on the multiplier tau */
static void project(int n,const double*y,const double*w,double W,double*x){
    double lo=0,hi=0,s=0;
    for(int i=0;i<n;i++){ x[i]=clip(y[i],w[i]); s+=x[i]; if(y[i]>hi) hi=y[i]; }
    if(s<=W) return;
    for(int k=0;k<100;k++){
        double tau=(lo+hi)/2; s=0;
        for(int i=0;i<n;i++) s+=clip(y[i]-tau,w[i]);
        if(s>W) lo=tau; else hi=tau;
    }
    for(int i=0;i<n;i++) x[i]=clip(y[i]-hi,w[i]);
}
int main(void){
    int n; double W;
    printf("Enter n and W, then v w lambda per item:\n");
    if(scanf("%d %lf",&n,&W)!=2 || n<=0) return 1;
    Item*it=malloc(n*sizeof(Item));
    for(int i=0;i<n;i++){ double v; it[i].id=i+1;
        if(scanf("%lf %lf %lf",&v,&it[i].w,&it[i].lam)!=3) return 1;
        it[i].rho=v/it[i].w; }
    qsort(it,n,sizeof(Item),cmp);                       /* step 1: order by lambda desc */
    double *w=malloc(n*8),*x=calloc(n,8),*y=malloc(n*8),*nx=malloc(n*8),L=0;
    for(int i=0;i<n;i++){ w[i]=it[i].w; L+=it[i].lam; }
    for(int iter=0; iter<500000; iter++){               /* step 2: projected gradient ascent */
        double tot=0,pref=0,pl=0,d=0;
        for(int i=0;i<n;i++) tot+=it[i].lam*x[i];
        for(int i=0;i<n;i++){
            pref+=x[i]; pl+=it[i].lam*x[i];
            /* gradient = rho_i - lam_i*sum_{b<=i}x_b - sum_{b>i}lam_b x_b */
            y[i]=x[i]+(it[i].rho-it[i].lam*pref-(tot-pl))/L;
        }
        project(n,y,w,W,nx);
        for(int i=0;i<n;i++){ d=fmax(d,fabs(nx[i]-x[i])); x[i]=nx[i]; }
        if(d<1e-13) break;
    }
    printf("\n item  start  amount  value\n");
    double T=0,total=0;
    for(int i=0;i<n;i++) if(x[i]>1e-9){
        double v=it[i].rho*x[i]-it[i].lam*(T*x[i]+x[i]*x[i]/2);
        printf(" %3d  %6.3f  %6.3f  %7.3f\n",it[i].id,T,x[i],v);
        total+=v; T+=x[i];
    }
    printf("Weight used = %.4f, MAXIMUM TOTAL VALUE = %.4f\n",T,total);
    return 0;
}