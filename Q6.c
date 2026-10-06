/* Q6: Rearrange S so equal characters are at least K apart
 * Max-heap on remaining counts (key = count*256 + (255-char) so ties pick the smaller char) + a K-step cool-down:
 * at position i, the char placed at i-K becomes available again. Pop the best char; empty heap => impossible.
 * Complexity: n positions x O(log sigma) heap ops = O(n) (sigma<=256); space O(n+sigma).
 * Input: S (no spaces) and K (K<=1 means no restriction)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int h[300],hs;
static void push(int v){
    int i=hs++; h[i]=v;
    for(;i && h[i]>h[(i-1)/2]; i=(i-1)/2){ int t=h[i]; h[i]=h[(i-1)/2]; h[(i-1)/2]=t; }
}
static int pop(void){
    int r=h[0]; h[0]=h[--hs];
    for(int i=0,m,l;;i=m){
        m=i; l=2*i+1;
        if(l<hs && h[l]>h[m]) m=l;
        if(l+1<hs && h[l+1]>h[m]) m=l+1;
        if(m==i) break;
        int t=h[i]; h[i]=h[m]; h[m]=t;
    }
    return r;
}
int main(void){
    static char S[1000005]; int K, cnt[256]={0};
    printf("Enter S and K: ");
    if(scanf("%1000000s %d",S,&K)!=2) return 1;
    int n=strlen(S);
    for(int i=0;i<n;i++) cnt[(unsigned char)S[i]]++;
    for(int c=0;c<256;c++) if(cnt[c]) push(cnt[c]*256+(255-c));
    char*out=malloc(n+1);
    int back = K>1 ? K : 1;                          /* cool-down length */
    for(int i=0;i<n;i++){
        if(i>=back){ int p=(unsigned char)out[i-back]; if(cnt[p]) push(cnt[p]*256+(255-p)); }
        if(!hs){ printf("Impossible -> empty string \"\"\n"); return 0; }
        int c=255-(pop()&255); out[i]=c; cnt[c]--;
    }
    out[n]=0; printf("Rearranged: %s\n",out);
    return 0;
}