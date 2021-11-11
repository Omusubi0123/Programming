//組み合わせを生成するプログラム    （自然な順序）

#include<stdio.h>
#define N_MAX 10

void combination(int a[],int n,int r,int i,int j);
void printComb(int a[],int r);

int number[N_MAX],c=0;

void main()
{
    int n,r,i;
    printf("nCr の n と r を入力してください\n");
    scanf("%d%d",&n,&r);
    for(i=0;i<n;i++)    number[i]=i+1;

    int a[n];
    combination(a,n,r,0,0);

    return;
}
void combination(int a[],int n,int r,int i,int j)
{
    if(i==r)   printComb(a,r);
    else{
        int k;
        for(k=j;k<=n-r+i;k++)
        {
            a[i]=number[k];
            combination(a,n,r,i+1,k+1);
        }
    }
}

void printComb(int a[],int r)
{
    printf("%3d: ",++c);
    for(int i=0;i<r;i++)    printf("%3d ",a[i]);
    printf("\n");
    return;
}
