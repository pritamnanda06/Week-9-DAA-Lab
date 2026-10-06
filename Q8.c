/* Q8: Minimum number of meeting rooms
 * ALGORITHM: sort all start times and all end times separately; sweep with two pointers.
 *   If next start < current earliest end -> a new room is needed (rooms++); else a room is freed
 *   (reuse). Meetings touching at a boundary ([1,5],[5,8]) do NOT conflict.
 *   Answer = maximum overlap depth = max rooms in use at any time.
 *   (Equivalent: min-heap of end times of rooms in use.)
 * COMPLEXITY: two sorts O(n log n) + linear sweep O(n) => O(n log n) time, O(n) space.
 * Input: n, then n lines: s_i e_i
 */
#include <stdio.h>
#include <stdlib.h>
static int cmp(const void*a,const void*b){ long long x=*(long long*)a,y=*(long long*)b; return (x>y)-(x<y); }
int main(void){
    int n; printf("Enter number of meetings: "); if(scanf("%d",&n)!=1||n<0) return 1;
    long long*s=malloc((n+1)*8),*e=malloc((n+1)*8);
    printf("Enter start end for each meeting:\n");
    for(int i=0;i<n;i++) if(scanf("%lld %lld",&s[i],&e[i])!=2) return 1;
    qsort(s,n,8,cmp); qsort(e,n,8,cmp);
    int rooms=0,cur=0,j=0;
    for(int i=0;i<n;i++){
        if(s[i]<e[j]){ cur++; } else { j++; }   /* reuse freed room: count unchanged */
        if(cur>rooms) rooms=cur;
    }
    printf("Minimum number of meeting rooms = %d\n",rooms);
    return 0;
}