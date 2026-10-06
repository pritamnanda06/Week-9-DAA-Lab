/* Q4: Minimum cost to connect sticks
 * Huffman-style greedy: repeatedly join the two shortest sticks (min-heap), push the sum back.
 * (Each length is paid once per merge it takes part in; total = sum L_i*depth_i, minimised as in Huffman.)
 * Complexity: n-1 rounds x O(log n) = O(n log n) time, O(n) space.
 * Input: n, then n lengths
 */
#include <stdio.h>
#include <stdlib.h>
typedef long long ll;
static ll *h; static int hs;
static void push(ll v){
    int i=hs++; h[i]=v;
    for(;i && h[i]<h[(i-1)/2]; i=(i-1)/2){ ll t=h[i]; h[i]=h[(i-1)/2]; h[(i-1)/2]=t; }
}
static ll pop(void){
    ll r=h[0]; h[0]=h[--hs];
    for(int i=0,m,l;;i=m){
        m=i; l=2*i+1;
        if(l<hs && h[l]<h[m]) m=l;
        if(l+1<hs && h[l+1]<h[m]) m=l+1;
        if(m==i) break;
        ll t=h[i]; h[i]=h[m]; h[m]=t;
    }
    return r;
}
int main(void){
    int n; printf("Enter n, then lengths: ");
    if(scanf("%d",&n)!=1 || n<=0) return 1;
    h=malloc(n*sizeof(ll));
    for(int i=0;i<n;i++){ ll x; if(scanf("%lld",&x)!=1) return 1; push(x); }
    ll cost=0;
    while(hs>1){ ll a=pop(),b=pop(); cost+=a+b; push(a+b); }
    printf("Minimum total cost = %lld\n",cost);
    return 0;
}