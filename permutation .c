//順列を生成するプログラム    （自然な順序）

#include<stdio.h>
#define N 4

int permutation(int number[],int start,int n);
void rotate(int number[],int a[],int start,int end);
void printPerm(int number[]);

int c=0;

void main()
{
    int number[N],i;
    for(i=0;i<N;i++)    number[i]=i+1;

    permutation(number,0,N);

    return;
}

int permutation(int number[],int start,int n)
{
    int moto[N],a[N],i;
    if(start==n)     printPerm(number);
    else{
        for(i=0;i<N;i++)
        {
            moto[i]=number[i];
            a[i]=number[i];
        }
        for(i=1;(i+start)<=n;i++)
        {
            //再起処理(n=n-1)
            permutation(a,start+1,n);
            //i-1から左回転する
            rotate(moto,a,start,start+i);
        }
    }
    //if(n>0)    rotate(number,N-n-1,N-1);
    return 1;
}

void rotate(int number[],int a[],int start,int end)
{
    int j,k;
    for(j=0;j<N;j++)    a[j]=number[j];
    k=a[end];
    for(j=end;j>start;j--)   a[j]=a[j-1];
    a[start]=k;
    return;
}

void printPerm(int a[])
{
    printf("%3d: ",++c);
    for(int i=0;i<N;i++)    printf("%3d ",a[i]);
    printf("\n");
    return;
}
