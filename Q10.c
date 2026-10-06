/* Q10: Greedy Superstring experiment (shortest common superstring is NP-hard)
 * GREEDY: drop strings contained in others; repeatedly merge the ordered pair with maximum overlap
 *         (suffix of a == prefix of b); with no overlap left, concatenate.
 * EXACT (n<=16): Held-Karp bitmask DP, dp[mask][last] = shortest superstring covering mask and ending in last.
 * Prints |greedy|, |optimal| and the ratio (conjecture: ratio <= 2; the sheet's claimed Sept-2026 disproof
 * is unverified and its instances are too large for the exact solver).
 * Complexity: greedy O(n^3 * total length); exact DP O(2^n n^2) time, O(2^n n) space.
 * Input: n, then n strings (<=60 chars)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MX 16
static int ov(const char*a,const char*b){          /* longest suffix of a that is a prefix of b */
    int la=strlen(a),lb=strlen(b);
    for(int k=la<lb?la:lb;k>0;k--) if(!strncmp(a+la-k,b,k)) return k;
    return 0;
}
static int dp[1<<MX][MX];
int main(void){
    int n0,n=0; char in[MX][64],s[MX][64],g[MX][1100];
    printf("Enter n (<=16), then the strings:\n");
    if(scanf("%d",&n0)!=1 || n0<=0 || n0>MX) return 1;
    for(int i=0;i<n0;i++) if(scanf("%60s",in[i])!=1) return 1;
    for(int i=0;i<n0;i++){                          /* remove substrings / duplicates */
        int drop=0;
        for(int j=0;j<n0;j++) if(j!=i && strstr(in[j],in[i]) && (strcmp(in[i],in[j]) || j<i)) drop=1;
        if(!drop) strcpy(s[n++],in[i]);
    }
    /* greedy */
    int m=n; for(int i=0;i<n;i++) strcpy(g[i],s[i]);
    while(m>1){
        int bi=0,bj=1,bo=-1;
        for(int i=0;i<m;i++) for(int j=0;j<m;j++) if(i!=j){ int o=ov(g[i],g[j]); if(o>bo){ bo=o; bi=i; bj=j; } }
        strcat(g[bi],g[bj]+bo);
        if(bj!=m-1) strcpy(g[bj],g[m-1]);
        m--;
    }
    /* exact DP */
    int O[MX][MX],len[MX],full=(1<<n)-1;
    for(int i=0;i<n;i++){ len[i]=strlen(s[i]); for(int j=0;j<n;j++) O[i][j]=(i==j)?0:ov(s[i],s[j]); }
    for(int a=0;a<=full;a++) for(int l=0;l<n;l++) dp[a][l]=1<<29;
    for(int i=0;i<n;i++) dp[1<<i][i]=len[i];
    for(int a=1;a<=full;a++) for(int l=0;l<n;l++) if((a>>l&1) && dp[a][l]<(1<<29))
        for(int j=0;j<n;j++) if(!(a>>j&1)){
            int v=dp[a][l]+len[j]-O[l][j];
            if(v<dp[a|1<<j][j]) dp[a|1<<j][j]=v;
        }
    int best=1<<29; for(int l=0;l<n;l++) if(dp[full][l]<best) best=dp[full][l];
    int gl=strlen(g[0]);
    printf("\nGreedy superstring (len %d): %s\nOptimal length: %d\nRatio greedy/optimal = %.4f%s\n",
           gl,g[0],best,(double)gl/best,gl>2*best?"  <-- exceeds 2!":"");
    return 0;
}