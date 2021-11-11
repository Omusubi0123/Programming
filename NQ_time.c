//Nクイーン問題   時間測定

#include<stdio.h>
#include<windows.h>
#define N 8
//#define DEBUG

int queen_BruteForce(int pos[],int n);
int queen_BackTrack(int pos[],int n);
int check_all(int pos[]);
int check_xy(int pos[],int n,int i);

int nSolution1=0,nSolution2=0;

void main()
{
    int pos[N];
    LARGE_INTEGER start1,end1,start2,end2,freq;
      //時間計測ための変数( start1,end1:for qsort_basic   start2,end2:for qsort_insert))
    QueryPerformanceFrequency(&freq);				//時間計測のための準備

    /*NQ_BruteForce ↓*/
    if(!QueryPerformanceCounter(&start1)) puts("Error");
    queen_BruteForce(pos,N-1);
    if(!QueryPerformanceCounter(&end1)) puts("Error");
    /*NQ_BruteForce ↑*/

    /*NQ_BackTrack ↓*/
    if(!QueryPerformanceCounter(&start2))   puts("Error");
    queen_BackTrack(pos,N-1);
    if(!QueryPerformanceCounter(&end2)) puts("Error");
    /*NQ_BackTrack ↑*/

	/*結果出力 ↓*/
	printf("Brute-Force:  解の数:%3d\n",nSolution1);
    printf("Back-Track:   解の数:%3d\n",nSolution2);
    printf("\n\n");
    printf("Brute-Force:  sort:%f\n",(double)(end1.QuadPart-start1.QuadPart)/(double)freq.QuadPart);
    printf("Back-Track:   sort:%f\n",(double)(end2.QuadPart-start2.QuadPart)/(double)freq.QuadPart);
	/*結果出力 ↑*/


    //queen_bf(pos,N-1);
    //queen_bt(pos,N-1);

    return;
}

int queen_BruteForce(int pos[],int n)
{
    int i;
    if(n<0){
        if(check_all(pos)==0){
            nSolution1++;
            #ifdef DEBUG
                printf("%2d:",nSolution1);
                for(int j=0;j<N;j++)    printf("  %d",pos[j]);
                printf("\n");
            #endif // DEBUG
        }
        return 0;
    }
    for(i=0;i<N;i++)
    {
        pos[(N-1)-n]=i;
        queen_BruteForce(pos,n-1);
        pos[(N-1)-n]=-1;
    }
    return 0;
}

int queen_BackTrack(int pos[],int n)
{
    int i;
    if(n<0){
        nSolution2++;
        #ifdef DEBUG
            printf("%2d:",nSolution2);
            for(int j=0;j<N;j++)    printf("  %d",pos[j]);
            printf("\n");
        #endif // DEBUG
        return 0;
    }
    for(i=0;i<N;i++)
    {
        if(check_xy(pos,(N-1)-n,i)==0){
            pos[(N-1)-n]=i;
            queen_BackTrack(pos,n-1);
            pos[(N-1)-n]=-1;
        }
    }
    return 0;
}

int check_all(int pos[])
{
    int i,j;
    for(i=0;i<N-1;i++)
    {
        for(j=i+1;j<N;j++)
        {
            if(pos[i]==pos[j])  return -1;
            if((pos[i]+i==pos[j]+j) || (pos[i]-i==pos[j]-j))    return -1;
        }
    }
    return 0;
}

int check_xy(int pos[],int n,int i)
{
    int j;
    for(j=0;j<n;j++)
    {
        if(i==pos[j])  return -1;
        if((i+n==pos[j]+j) || (i-n==pos[j]-j))    return -1;
    }
    return 0;
}
