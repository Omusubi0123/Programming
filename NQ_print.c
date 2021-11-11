//NƒNƒC[ƒ“–â‘è   Brute-force

#include<stdio.h>
#define N 8
//#define DEBUG

int queen_bf(int pos[],int n);
int queen_bt(int pos[],int n);
int check_all(int pos[]);
int check_xy(int pos[],int n,int i);

int nSolution=0;

void main()
{
    int pos[N];
    //queen_bf(pos,N-1);
    queen_bt(pos,N-1);
    printf("\n‰ğ‚Ì”:%2d\n",nSolution);
    return;
}

int queen_bf(int pos[],int n)
{
    int i;
    if(n<0){
        if(check_all(pos)==0){
            nSolution++;
            #ifdef DEBUG
                printf("%2d:",nSolution);
                for(int j=0;j<N;j++)    printf("  %d",pos[j]);
                printf("\n");
            #endif // DEBUG
        }
        return 0;
    }
    for(i=0;i<N;i++)
    {
        pos[(N-1)-n]=i;
        queen_bf(pos,n-1);
        pos[(N-1)-n]=-1;
    }
    return 0;
}

int queen_bt(int pos[],int n)
{
    int i;
    if(n<0){
        nSolution++;
        #ifdef DEBUG
            printf("%2d:",nSolution);
            for(int j=0;j<N;j++)    printf("  %d",pos[j]);
            printf("\n");
        #endif // DEBUG
        return 0;
    }
    for(i=0;i<N;i++)
    {
        if(check_xy(pos,(N-1)-n,i)==0){
            pos[(N-1)-n]=i;
            queen_bt(pos,n-1);
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
