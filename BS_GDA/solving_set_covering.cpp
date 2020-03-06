#include "mex.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Input 1: sets N*(ripple)
// Input 2: s_dims
// Input 3: N
// Input 4: sets_order
// Input 5: K

// Output 1: selected_sets_out
// Output 2: vf

int array_max(int*a, int *order, int n);
int any(int *a, int N);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) 
{

    if (nrhs != 5)
    {
        mexErrMsgTxt("Parameter number is not correct!");
    }

    int i,j,idx;

    //input 3: N
    const int N=*(int*)mxGetPr(prhs[2]);  
    // mexPrintf("N:%d\n",N);

    //input 5: K
    const int K=*(int*)mxGetPr(prhs[4]);  
    // mexPrintf("K:%d\n",K);

    //input 4: s_order
    int* s_order=(int*)mxGetPr(prhs[3]);  

    // mexPrintf("s_order: ");
    for(i=0;i<N;i++)
    {
        s_order[i]--;
        // mexPrintf("%d ",s_order[i]);
    }
    // mexPrintf("\n");

    //input 2: s_dims
    int* s_dims=(int*)mxGetPr(prhs[1]);  

    // mexPrintf("s_dims: ");
    // for(i=0;i<N;i++)
    // {
    //     mexPrintf("%d ",s_dims[i]);
    // }
    // mexPrintf("\n");

    //input 1: sets 
    mxArray *p_sets=NULL;
    int** sets = (int**)malloc(sizeof(int*)*N);

    for(i=0;i<N;i++)
    {
        p_sets=mxGetCell(prhs[0],i); 
        sets[i]=(int*) mxGetPr(p_sets);

        for(j=0;j<s_dims[i];j++)
        {
            sets[i][j]--;
            // mexPrintf("%d ",sets[i][j]);
        }
        // mexPrintf("\n");
    }

    // variables
    int* uncovered=(int*)malloc(sizeof(int)*N);
    int* selected_sets=(int*)malloc(sizeof(int)*N);

    for(i=0;i<N;i++) {uncovered[i]=1;}
    memset(selected_sets,0,sizeof(int)*N);

    //output 1: selected_sets_out
	plhs[0] = mxCreateDoubleMatrix(N, 1, mxREAL);
	double *selected_sets_out = (double*)mxGetPr(plhs[0]);

    //output 2: vf
	plhs[1] = mxCreateDoubleMatrix(1, 1, mxREAL);
	double *vf = (double*)mxGetPr(plhs[1]);

    if(K<1)
    {
        *vf=0;
        for(i=0;i<N;i++)
        {
            selected_sets_out[i]=(double)selected_sets[i];
        }
        free(sets);
        free(uncovered);
        free(selected_sets);
        return;
    }

    //select first node
    idx=array_max(s_dims,s_order,N);
    // mexPrintf("first node:%d, covered:%d nodes\n",idx+1,s_dims[idx]);

    for(i=0;i<s_dims[idx];i++)
    {
        j=sets[idx][i];
        uncovered[j]=0;
    }
    selected_sets[idx]=1;

    //select 2->K nodes
    int selected_num=1,uncovered_flag=0;
    int* uncovered_num=(int*)malloc(sizeof(int)*N);
    int* covered_sets=(int*)malloc(sizeof(int)*N);

    memset(covered_sets,0,sizeof(int)*N);
    uncovered_flag=any(uncovered,N);
    // mexPrintf("uncovered_flag:%d\n",uncovered_flag);

    while(selected_num<K && uncovered_flag)
    {
        memset(uncovered_num,0,sizeof(int)*N);
        for(i=0;i<N;i++)
        {
            if(selected_sets[i]<1 && covered_sets[i]<1)
            {
                for(j=0;j<s_dims[i];j++)
                {
                    idx=sets[i][j];
                    uncovered_num[i]+=uncovered[idx];
                }
                if(uncovered_num[i]==0)
                {
                    covered_sets[i]=1;
                }

            }
        }
        idx=array_max(uncovered_num,s_order,N);
        // mexPrintf("selected node:%d, covered: %d nodes\n",idx+1,uncovered_num[idx]);

        for(i=0;i<s_dims[idx];i++)
        {
            j=sets[idx][i];
            uncovered[j]=0;
        }
        selected_sets[idx]=1;

        selected_num++;
        uncovered_flag=any(uncovered,N);
    }

    if(uncovered_flag)
    {
        *vf=0;
    }
    else
    {
        *vf=1;
    }
    for(i=0;i<N;i++)
    {
        selected_sets_out[i]=(double)selected_sets[i];
    }

    free(sets);
    free(uncovered);
    free(selected_sets);
    free(uncovered_num);
    free(covered_sets);
}

int array_max(int*a, int *order, int n)
{
    int i,idx;
    int max_idx=0,max_num=0;

    for(i=0;i<n;i++)
    {
        idx=order[i];
        if(a[idx]>max_num)
        {
            max_idx=idx;
            max_num=a[idx];
        }
    }
    return max_idx;
}

int any(int *a, int N)
{
    int i,res=0;
    for(i=0;i<N;i++)
    {
        if(a[i]!=0)
        {
            res=1;
            break;
        }
    }
    return res;
}