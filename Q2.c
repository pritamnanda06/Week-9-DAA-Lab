/* Q2: Huffman coding + canonical codebook
 * Min-heap on frequency; merge the two smallest until one tree remains; code length = node depth.
 * Canonical code: sort by (length, symbol); first code = 0...0; next = (prev+1), then append zeros up to new length.
 * Complexity: n-1 merges x O(log n) heap ops = O(n log n); sort O(n log n); space O(n).
 * Input: n, then n lines: symbol freq
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 1000
typedef long long ll;
typedef struct { char name[32]; ll f; int len; } Sym;
ll F[2*N]; int Lc[2*N],Rc[2*N],dep[2*N],h[2*N],hs;
static int less(int a,int b){ return F[a]<F[b] || (F[a]==F[b] && a<b); }
static void push(int v){
    int i=hs++; h[i]=v;
    for(;i && less(h[i],h[(i-1)/2]); i=(i-1)/2){ int t=h[i]; h[i]=h[(i-1)/2]; h[(i-1)/2]=t; }
}
static int pop(void){
    int r=h[0]; h[0]=h[--hs];
    for(int i=0,m,l;;i=m){
        m=i; l=2*i+1;
        if(l<hs && less(h[l],h[m])) m=l;
        if(l+1<hs && less(h[l+1],h[m])) m=l+1;
        if(m==i) break;
        int t=h[i]; h[i]=h[m]; h[m]=t;
    }
    return r;
}
static int cmp(const void*a,const void*b){
    const Sym*p=a,*q=b;
    return p->len!=q->len ? p->len-q->len : strcmp(p->name,q->name);
}
int main(void){
    int n; Sym s[N];
    printf("Enter n, then symbol freq per line:\n");
    if(scanf("%d",&n)!=1 || n<=0 || n>N) return 1;
    for(int i=0;i<n;i++){ if(scanf("%31s %lld",s[i].name,&F[i])!=2) return 1; push(i); }
    int nn=n;
    while(hs>1){ int a=pop(),b=pop(); F[nn]=F[a]+F[b]; Lc[nn]=a; Rc[nn]=b; push(nn++); }
    for(int p=nn-1;p>=n;p--) dep[Lc[p]]=dep[Rc[p]]=dep[p]+1;   /* children have smaller index */
    ll cost=0;
    for(int i=0;i<n;i++){ s[i].f=F[i]; s[i].len=(n==1)?1:dep[i]; cost+=s[i].f*s[i].len; }
    qsort(s,n,sizeof(Sym),cmp);
    char code[N+8]; int cl=s[0].len; memset(code,'0',cl); code[cl]=0;
    printf("\n symbol  freq  len  code\n");
    for(int i=0;i<n;i++){
        if(i){                                   /* code = (code+1), then pad zeros */
            int k=cl-1; while(k>=0 && code[k]=='1') code[k--]='0';
            if(k>=0) code[k]='1';
            while(cl<s[i].len) code[cl++]='0';
            code[cl]=0;
        }
        printf(" %-7s %5lld %3d  %s\n",s[i].name,s[i].f,s[i].len,code);
    }
    printf("Sum f*len = %lld, expected length = %.4f bits/symbol\n",cost,(double)cost/F[nn-1]);
    return 0;
}