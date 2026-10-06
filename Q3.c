/* Q3: Minimum refuelling stops (1 unit fuel per unit distance)
 * Sort stations by distance. Drive as far as fuel allows, remembering every station passed in a max-heap.
 * When stuck, retroactively refuel at the LARGEST remembered station (reverse greedy). Empty heap => -1.
 * Complexity: sort O(m log m); each station pushed/popped once => O(m log m) time, O(m) space.
 * Input: D F m, then m lines: d_i f_i
 */
#include <stdio.h>
#include <stdlib.h>
typedef long long ll;
typedef struct { ll d,f; } St;
static ll *h; static int hs;
static void push(ll v){
    int i=hs++; h[i]=v;
    for(;i && h[i]>h[(i-1)/2]; i=(i-1)/2){ ll t=h[i]; h[i]=h[(i-1)/2]; h[(i-1)/2]=t; }
}
static ll pop(void){
    ll r=h[0]; h[0]=h[--hs];
    for(int i=0,m,l;;i=m){
        m=i; l=2*i+1;
        if(l<hs && h[l]>h[m]) m=l;
        if(l+1<hs && h[l+1]>h[m]) m=l+1;
        if(m==i) break;
        ll t=h[i]; h[i]=h[m]; h[m]=t;
    }
    return r;
}
static int cmp(const void*a,const void*b){ ll x=((St*)a)->d,y=((St*)b)->d; return (x>y)-(x<y); }
int main(void){
    ll D,fuel; int m;
    printf("Enter D F m, then d f per station:\n");
    if(scanf("%lld %lld %d",&D,&fuel,&m)!=3) return 1;
    St*s=malloc((m+1)*sizeof(St)); h=malloc((m+1)*sizeof(ll));
    for(int i=0;i<m;i++) if(scanf("%lld %lld",&s[i].d,&s[i].f)!=2) return 1;
    qsort(s,m,sizeof(St),cmp);
    int i=0,stops=0;
    while(fuel<D){
        while(i<m && s[i].d<=fuel) push(s[i++].f);   /* stations now reachable */
        if(!hs){ printf("Cannot reach target: -1\n"); return 0; }
        fuel+=pop(); stops++;                         /* best station we "should have" stopped at */
    }
    printf("Minimum refuelling stops = %d\n",stops);
    return 0;
}