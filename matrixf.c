#include "matrixf.h"
#include "stdio.h"
#include "stdlib.h"
/*
typedef struct{
float **data;
int filas;
int columnas;
}matrix;
*/

 
matrix matrix_init(int filas, int columnas, float *M)
{
    matrix temp;
    temp.filas=filas;
    temp.columnas=columnas;
    int i,j;
    float **m;
  
    m=malloc(filas*sizeof(float*));
    
    for(i=0;i<filas;i++){
        m[i]=malloc(columnas*sizeof(float));
    }
    
    for(i=0;i<filas;i++){
        for(j=0;j<columnas;j++)
        {
           
            m[i][j]=M[i*columnas+j]; 
        }
    }
    
    temp.data=m;
    return temp;
}

void matrix_print(matrix M)
{
    printf("\n");
    int i,j;
    for(i=0;i<M.filas;i++)
    {
        for(j=0;j<M.columnas;j++)
        {
            printf("%.3f\t",M.data[i][j]);
        }
        printf("\n");
    }
}

matrix matrix_sum(matrix A, matrix B)
{
    matrix temp;
    temp.columnas=A.columnas;
    temp.filas=B.filas;
    int i,j;
    float **m;
    m=malloc(temp.filas*sizeof(float*));
    for(i=0;i<temp.filas;i++){
        m[i]=malloc(temp.columnas*sizeof(float));
    }

    for(i=0;i<temp.filas;i++){
        for(j=0;j<temp.columnas;j++)
        {
           
            m[i][j]=A.data[i][j]+B.data[i][j]; 
        }
    }
    temp.data=m;
    return temp;



}

matrix escalar(matrix A, float escalar)
{
    matrix temp;
    temp.columnas=A.columnas;
    temp.filas=A.filas;
    int i,j;
    float **m;
    m=malloc(temp.filas*sizeof(float*));
    for(i=0;i<temp.filas;i++){
        m[i]=malloc(temp.columnas*sizeof(float));
    }

    for(i=0;i<temp.filas;i++){
        for(j=0;j<temp.columnas;j++)
        {
           
            m[i][j]=escalar*A.data[i][j]; 
        }
    }
    temp.data=m;
    return temp;


}

matrix matrix_product(matrix A, matrix B)
{
    
    matrix temp;
    if(A.columnas!=B.filas){printf("Las matrices no son multiplicables");}
    temp.filas=A.filas;
    temp.columnas=B.columnas;
    int i,j;
    float **m;
    m=malloc(temp.filas*sizeof(float*));
    for(i=0;i<temp.filas;i++){
        m[i]=malloc(temp.columnas*sizeof(float));
    }
    int k;
    float sum;
    for(i=0;i<temp.filas;i++){
        for(j=0;j<temp.columnas;j++)
        {
           sum=0;
           for(k=0;k<A.columnas;k++)
           {
            sum=sum+(A.data[i][k]*B.data[k][j]);
           }
            m[i][j]=sum;
        }
    }
    temp.data=m;
    return temp;

}

matrix matrix_traspuesta(matrix A)
{
    matrix temp;
    temp.columnas=A.filas;
    temp.filas=A.columnas;
    int i,j;
    float **m;
    m=malloc(temp.filas*sizeof(float*));
    for(i=0;i<temp.filas;i++){
        m[i]=malloc(temp.columnas*sizeof(float));
    }

    for(i=0;i<temp.filas;i++){
        for(j=0;j<temp.columnas;j++)
        {
           
            m[i][j]=A.data[j][i]; 
        }
    }
    temp.data=m;
    return temp;
}
