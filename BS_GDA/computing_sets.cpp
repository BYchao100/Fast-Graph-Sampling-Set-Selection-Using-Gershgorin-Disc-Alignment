#include "mex.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// input 1: neis
// input 2: neis_n
// input 3: neis_w
// input 4: d
// input 5: N
// input 6: mu
// input 7: thres
// input 8: p hops

// output 1: sets
// output 2: s_dims

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) 
{
    if (nrhs != 8)
    {
        mexErrMsgTxt("Parameter number is not correct!");
    }

    int i,j;

    //input 8: p hops
    const int p_hops=*(int*)mxGetPr(prhs[7]);
    // mexPrintf("p_hops:%d\n",p_hops);

    //input 5: N
    const int N=*(int*)mxGetPr(prhs[4]);  
    // mexPrintf("N:%d\n",N);

    //input 6: mu
    const double mu=*(double*)mxGetPr(prhs[5]);  
    // mexPrintf("mu:%f\n",mu);

    //input 7: thres
    const double thres=*(double*)mxGetPr(prhs[6]);  
    // mexPrintf("thres:%f\n",thres);

    //input 4: d
    double* d=(double*)mxGetPr(prhs[3]);

    // mexPrintf("d:");
    // for(i=0;i<N;i++)
    // {
    //     mexPrintf("%f ",d[i]);
    // }
    // mexPrintf("\n");  

    //input 2: neis_n
    int* neis_n=(int*)mxGetPr(prhs[1]);

    // mexPrintf("nei_n:");
    // for(i=0;i<N;i++)
    // {
    //     mexPrintf("%d ",neis_n[i]);
    // }
    // mexPrintf("\n");    

    //input 1: neis 
    mxArray *p_neis=NULL;
    int** neis = (int**)malloc(sizeof(int*)*N);

    for(i=0;i<N;i++)
    {
        p_neis=mxGetCell(prhs[0],i); 
        neis[i]=(int*) mxGetPr(p_neis);

        // for(j=0;j<neis_n[i];j++)
        // {
        //     // neis[i][j]--;
        //     mexPrintf("%d ",neis[i][j]);
        // }
        // mexPrintf("\n");
    }

    //input 3: neis_w
    mxArray *p_neis_w=NULL;
    double** neis_w = (double**)malloc(sizeof(double*)*N);

    for(i=0;i<N;i++)
    {
        p_neis_w=mxGetCell(prhs[2],i); 
        neis_w[i]=(double*) mxGetPr(p_neis_w);

        // for(j=0;j<neis_n[i];j++)
        // {
        //     mexPrintf("%f ",neis_w[i][j]);
        // }
        // mexPrintf("\n");
    }

    //output 1: sets
    int ndim=2;
    int sets_dims[2]={0};
    sets_dims[0]=N;sets_dims[1]=1;
    plhs[0]=mxCreateCellArray(ndim,sets_dims);

    //output 2: s_dims
    plhs[1]=mxCreateNumericArray(ndim,sets_dims,mxINT32_CLASS,mxREAL);
    int* s_dims=(int*)mxGetPr(plhs[1]);

    if (thres>1)
    {
        mexErrMsgTxt("Threshold is set too large!");
    }

    int set_idx,idx;
    double* scales=(double*)malloc(sizeof(double)*N);
    double* scales_tmp=(double*)malloc(sizeof(double)*N);
    int* is_visited=(int*)malloc(sizeof(int)*N);
    int* in_queue=(int*)malloc(sizeof(int)*N);
    int* queue=(int*)malloc(sizeof(int)*(N+10));
    int q_front,q_rear,nei,q_num;
    double s_i,tmp,is_sampled;
    mxArray* set_value=NULL;
    int set_dim[2]={0,1};
    int* set_value2=NULL;

    int* hops=(int*)malloc(sizeof(int)*N);

    for(set_idx=0;set_idx<N;set_idx++)
    {
        q_front=0;
        q_rear=0;
        q_num=0;
        for(i=0;i<N;i++) {scales[i]=1;}
        memset(is_visited,0,sizeof(int)*N);
        memset(in_queue,0,sizeof(int)*N);
        memset(queue,0,sizeof(int)*(N+10));
        memset(hops,0,sizeof(int)*N);

        queue[q_rear]=set_idx;
        in_queue[set_idx]=1;
        q_rear++;
        is_sampled=1;
        while(q_front<q_rear)
        {
            idx=queue[q_front];
            q_front++;

            for(i=0;i<N;i++)
            {
                if(scales[i]>=1)
                {
                    scales_tmp[i]=scales[i];
                }
                else
                {
                    scales_tmp[i]=1;
                }
            }
            s_i=d[idx]*mu-thres+is_sampled;
            is_sampled=0;
            tmp=0;
            for(i=0;i<neis_n[idx];i++)
            {
                nei=neis[idx][i];
                tmp=tmp+(mu*neis_w[idx][i]/scales_tmp[nei]);
            }
            s_i/=tmp;
            // is_visited[idx]=1;
            scales[idx]=s_i;

            if(s_i>=1 && hops[idx]<=p_hops)
            {
                q_num++;
                is_visited[idx]=1;
                // mexPrintf("idx:%d,hops:%d\n",idx,hops[idx]);

                for(i=0;i<neis_n[idx];i++)
                {
                    nei=neis[idx][i];
                    if(in_queue[nei]<1)
                    {
                        queue[q_rear]=nei;
                        q_rear++;
                        in_queue[nei]=1;
                        hops[nei]=hops[idx]+1;
                    }
                }
            }


        }
        s_dims[set_idx]=q_num;
        set_dim[0]=q_num;
        set_value=mxCreateNumericArray(ndim,set_dim,mxINT32_CLASS,mxREAL);
        mxSetCell(plhs[0],set_idx,set_value);
        set_value2=(int*)mxGetPr(set_value);

        j=0;
        for(i=0;i<N && j<q_num;i++)
        {
            if(is_visited[i] && scales[i]>=1)
            {
                set_value2[j]=i+1;
                j++;

                // mexPrintf("is_visited:");
                // for(int k=0;k<N;k++)
                // {
                //     mexPrintf("%d ",is_visited[k]);
                // }
                // mexPrintf("\n");
            }
        }


    }

    // for(i=0;i<N;i++)
    // {
    //     for(j=0;j<neis_n[i];j++)
    //     {
    //         neis[i][j]++;
    //     }
    // }


    free(neis);
    free(neis_w);
    free(scales);
    free(is_visited);
    free(in_queue);
    free(queue);
    free(scales_tmp);
    free(hops);
}