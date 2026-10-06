/* Q5: Candy distribution (two-pass slope greedy)
 * ALGORITHM: every child starts with 1.
 *   Left->right: if r[i]>r[i-1] then c[i]=c[i-1]+1   (satisfies left-neighbour constraints minimally)
 *   Right->left: if r[i]>r[i+1] then c[i]=max(c[i],c[i+1]+1) (satisfies right constraints, keeps left ones)
 *   Each c[i] is the smallest value forced by the longest strictly-increasing run reaching i from
 *   either side, so the result is pointwise minimal => total minimal.
 * COMPLEXITY: two linear scans: O(n) time, O(n) space (O(1) extra possible with slope counting).
 * Input: n, then n ratings.
 */
#include <stdio.h>
#include <stdlib.h>
int main(void){
    int n; printf("Enter number of children: "); if(scanf("%d",&n)!=1||n<=0) return 1;
    int *r=malloc(n*sizeof(int)),*c=malloc(n*sizeof(int));
    printf("Enter ratings: "); for(int i=0;i<n;i++) if(scanf("%d",&r[i])!=1) return 1;
    for(int i=0;i<n;i++) c[i]=1;
    for(int i=1;i<n;i++) if(r[i]>r[i-1]) c[i]=c[i-1]+1;
    for(int i=n-2;i>=0;i--) if(r[i]>r[i+1]&&c[i]<=c[i+1]) c[i]=c[i+1]+1;
    long long tot=0; printf("Candies: "); for(int i=0;i<n;i++){ printf("%d ",c[i]); tot+=c[i]; }
    printf("\nMinimum total candies = %lld\n",tot);
    return 0;
}