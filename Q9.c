/* Q9: Optimal alphabetic binary tree (Hu-Tucker, via the equivalent Garsia-Wachs greedy)
 * Phase 1: list with +inf sentinels. Take the smallest k with a[k-1] <= a[k+1]; merge a[k-1],a[k] into t
 *          (cost += t); re-insert t just right of the nearest element on its left with weight >= t.
 * Phase 2: leaf depths in the merge tree = depths of an optimal alphabetic tree (Garsia-Wachs theorem).
 * Phase 3: scan leaves left->right with a stack; merge top two subtrees of equal depth (left gets 0, right 1).
 * Validation: O(n^3) interval DP optimum is compared.
 * Complexity: n-1 rounds x O(n) list scan/shift = O(n^2) time, O(n) space (O(n log n) needs fancier structures).
 * Input: n, then n weights
 */
#include <stdio.h>
#include <string.h>
#define N 200
#define INF 4000000000000000000LL
typedef long long ll;
ll w[N],a[2*N+5],dp[N][N],pre[N+1];
int id[2*N+5],Lc[2*N],Rc[2*N],dep[2*N];
char code[N][N+5];
int main(void){
    int n; printf("Enter n and weights in order: ");
    if(scanf("%d",&n)!=1 || n<=0 || n>N) return 1;
    for(int i=0;i<n;i++) if(scanf("%lld",&w[i])!=1) return 1;
    /* Phase 1 */
    int m=n,nn=n; ll cost=0;
    a[0]=a[n+1]=INF;
    for(int i=1;i<=n;i++){ a[i]=w[i-1]; id[i]=i-1; }
    while(m>1){
        int k=2; while(a[k-1]>a[k+1]) k++;
        ll t=a[k-1]+a[k]; cost+=t;
        Lc[nn]=id[k-1]; Rc[nn]=id[k]; int node=nn++;
        int j=k-2; while(a[j]<t) j--;                       /* nearest left with weight >= t */
        for(int p=k-2;p>j;p--){ a[p+1]=a[p]; id[p+1]=id[p]; } /* shift right to open slot j+1 */
        a[j+1]=t; id[j+1]=node;
        for(int p=k+1;p<=m+1;p++){ a[p-1]=a[p]; id[p-1]=id[p]; } /* close the gap */
        m--;
    }
    /* Phase 2 */
    for(int p=nn-1;p>=n;p--) dep[Lc[p]]=dep[Rc[p]]=dep[p]+1;
    ll c2=0; for(int i=0;i<n;i++) c2+=w[i]*dep[i];
    /* Phase 3 */
    int lo[N],hi[N],d[N],sp=0;
    for(int i=0;i<n;i++){
        lo[sp]=hi[sp]=i; d[sp++]=dep[i];
        while(sp>=2 && d[sp-1]==d[sp-2]){
            for(int s=0;s<2;s++) for(int x=lo[sp-2+s];x<=hi[sp-2+s];x++){
                memmove(code[x]+1,code[x],strlen(code[x])+1); code[x][0]='0'+s;
            }
            hi[sp-2]=hi[sp-1]; d[sp-2]--; sp--;
        }
    }
    /* validation DP */
    for(int i=0;i<n;i++) pre[i+1]=pre[i]+w[i];
    for(int len=2;len<=n;len++) for(int i=0;i+len<=n;i++){
        int j=i+len-1; ll b=INF;
        for(int k=i;k<j;k++) if(dp[i][k]+dp[k+1][j]<b) b=dp[i][k]+dp[k+1][j];
        dp[i][j]=b+pre[j+1]-pre[i];
    }
    printf("\n leaf  weight  depth  code\n");
    for(int i=0;i<n;i++) printf(" %3d  %6lld  %5d  %s\n",i+1,w[i],dep[i],n==1?"(root)":code[i]);
    printf("Optimal cost = %lld (sum w*depth = %lld), DP check = %lld -> %s\n",
           cost,c2,dp[0][n-1],(cost==c2 && c2==dp[0][n-1])?"MATCH":"MISMATCH");
    return 0;
}