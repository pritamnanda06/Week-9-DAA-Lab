/* Q7: Minimise deviation (max-min) with  odd -> *2  and  even -> /2
 * Normalise: double every odd number (its largest reachable value); now elements can only DECREASE.
 * Max-heap + running min: ans=min(ans,top-min); if top is odd stop, else replace top by top/2 and update min.
 * Complexity: each element halves <= log2(M)+1 times => O(n log M log n) time, O(n) space.
 * Input: n, then n positive integers
 */
#include <stdio.h>
#include <stdlib.h>
typedef long long ll;
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
int main(void){
    int n; printf("Enter n, then array: ");
    if(scanf("%d",&n)!=1 || n<=0) return 1;
    h=malloc((n+1)*sizeof(ll)); ll mn=-1;
    for(int i=0;i<n;i++){
        ll x; if(scanf("%lld",&x)!=1) return 1;
        if(x&1) x*=2;
        push(x); if(mn<0 || x<mn) mn=x;
    }
    ll ans=h[0]-mn;
    for(;;){
        ll t=pop(); if(t-mn<ans) ans=t-mn;
        if(t&1) break;                               /* max cannot be reduced further */
        push(t/2); if(t/2<mn) mn=t/2;
    }
    printf("Minimum deviation = %lld\n",ans);
    return 0;
}