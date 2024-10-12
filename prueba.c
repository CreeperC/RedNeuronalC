#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matrixf.c"

int LIMIT_CHAR=20; //Maximo de caracteres por celda de registro


typedef struct 
{
    int cols;
    int registros;
    char** name_cols;
    char*** data;
}dframe;

typedef struct {
    int tam;
    float* pesos;
    float sesgo;
    int FuncionDeActivacion;
    float salida;
}neurona;



float ejecutar(neurona N, float* vector)
{
    int i;
    float sum=0;
    for(i=0;i<N.tam;i++)
    {   
        sum=sum+N.pesos[i]*vector[i];
    }
    sum=sum+N.sesgo;
   
    /*ESPACIO   PARA PROGRAMAR LA FUNCION DE ACTIVACION*/

    return sum;

}


dframe readCSV(char* text)
{
    FILE* arch;
    arch = fopen(text,"r");
    int c;
    int i=0,k=0;

    do{
        k++;
        c= fgetc(arch);
        if(c==','){i++;}
        //printf("%c",c);

    }while(c!='\n');
    i++;
    //printf("\nNumero de columnas=%d\n",i);
    int col=i;

    char** columnas;
    char **aux;
    columnas= malloc(col*sizeof(char*));
    aux=malloc(col*sizeof(char*));
    for(i=0;i<col;i++)
    {
        aux[i]=malloc(k*sizeof(char));
    }
    rewind(arch);
    
    for(k=0;k<col;k++)
    {   
        i=0;
        c=0;
        do{
            
            if(c==',' || c=='\n')
            {
                columnas[k]=aux[k];
                break;
            }
            c= fgetc(arch);
            if(c=='"'){continue;}
            if(c==','){continue;}
            if(c=='\n'){continue;}
            aux[k][i]=c;
            i++;
        }while(1);
    }
    free(aux);

    rewind(arch);
    i=0;
    do{
        c= fgetc(arch);
        if(c=='\n'){i++;}
        //printf("%c",c);

    }while(c!=EOF);
    //printf("\nNumero de registros=%d\n",i);
    int rows=i;
    

    
    rewind(arch);
    do{
        c= fgetc(arch);
    }while(c!='\n');
    

    char*** ref;
    ref=malloc(rows*sizeof(char**));
    for(i=0;i<rows;i++)
    {
        ref[i]=malloc(col*sizeof(char*));
    }
    int j;
    for(i=0;i<rows;i++)
    {
        for(j=0;j<col;j++)
        {
            ref[i][j]=malloc(LIMIT_CHAR*sizeof(char));
        }
    }

    for(j=0;j<rows;j++)
    {
        //printf("\n%d-->",j+1);
        for(k=0;k<col;k++)
        {   
            i=0;
            c=0;
            do{
                
                if(c==',' || c=='\n' || c==EOF)
                {
                    
                    //printf("%s ",ref[j][k]);

                    break;
                }
                c= fgetc(arch);
                if(c==EOF){continue;}
                if(c=='"'){continue;}
                if(c==','){continue;}
                if(c=='\n'){continue;}
                ref[j][k][i]=c;
                i++;
            }while(1);
        }  
    }
    dframe temp;
    temp.cols=col;
    temp.registros=rows;
    temp.name_cols=columnas;
    temp.data=ref;
    fclose(arch);
    return temp;
}


int red()
{
    dframe df;
    df=readCSV("iris.csv");
    float aux;
    float** vector;
    int i,j;

    vector=malloc(df.registros*sizeof(float*));
    for(i=0;i<df.registros;i++)
    {
        vector[i]=malloc(df.cols*sizeof(float));
    }
    for(i=0;i<df.registros;i++)
    {
        for(j=0;j<df.cols;j++)
        {
            
            if(df.data[i][j][3]=='g'){aux=1;}
            if(df.data[i][j][3]=='s'){aux=2;}
            if(df.data[i][j][3]=='o'){aux=3;}
            else{sscanf(df.data[i][j],"%f",&aux);}
            vector[i][j]=aux;
            //printf("|%s",df.data[i][j]);
        }
        //printf("\n");
    
    }


    /*INICIO DEL CODIGO DE LA RED NEURONAL: 3 NEURONAS EN LA CAPA DE ENTRADA*/
    neurona in_A, in_B, in_C;
    float v1[4]={.01,.02,.03,.04};
    float v2[4]={.01,.03,.05,.04}; 
    float v3[4]={.01,.03,.03,.07};

    in_A.tam=4;
    in_A.pesos=v1;
    in_A.sesgo=.02;

    in_B.tam=4;
    in_B.pesos=v2;
    in_B.sesgo=.02;

    in_C.tam=4;
    in_C.pesos=v3;
    in_C.sesgo=.02;

    /*2 NEURONAS EN CAPA OCULTA*/
    neurona ocult_A, ocult_B;
    float p1[3]={.01,.02,.03};
    float p2[3]={.01,.02,.03};
    ocult_A.tam=3;
    ocult_A.pesos=p1;
    ocult_A.sesgo=.02;

    ocult_B.tam=3;
    ocult_B.pesos=p2;
    ocult_B.sesgo=.02;


    /*Una neurona como capa de salida*/
    neurona salida;
    float s[2]={.02,.04};
    salida.tam=2;
    salida.pesos=s;
    salida.sesgo=.03;



    float* dL;
    float* dY;
    dL=malloc(df.registros*sizeof(float));
    dY=malloc(df.registros*3*sizeof(float));
    float VDAETDS[3]; //VectorDeApoyoEnTraspasoDeSeniales
    float* salidas_optimizacion;
    salidas_optimizacion=malloc(df.registros*4*(sizeof(float)));



    float error=0;
    for(i=0;i<df.registros;i++)
    {  
        /*Ejecucion de la primera capa*/
        in_A.salida=ejecutar(in_A,vector[i]);
        in_B.salida=ejecutar(in_B,vector[i]);
        in_C.salida=ejecutar(in_C,vector[i]);

        VDAETDS[0]=in_A.salida;
        VDAETDS[1]=in_B.salida;
        VDAETDS[2]=in_C.salida;
        salidas_optimizacion[4*i+0]=in_A.salida;
        salidas_optimizacion[4*i+0]=in_B.salida;
        salidas_optimizacion[4*i+0]=in_C.salida;
        salidas_optimizacion[4*i+0]=(float)1;


        /*Ejecucion de la segunda capa*/
        ocult_A.salida=ejecutar(ocult_A,VDAETDS);
        ocult_B.salida=ejecutar(ocult_B,VDAETDS);

        VDAETDS[0]=ocult_A.salida;
        VDAETDS[1]=ocult_B.salida;
        dY[3*i]=ocult_A.salida;
        dY[3*i+1]=ocult_B.salida;
        dY[3*i+2]=(float)1;

        //printf("%f\t%f\t%f\n",ocult_A.salida,ocult_B.salida,(float)1);
        /*Ejecucion de la tercera capa*/
        salida.salida=ejecutar(salida,VDAETDS);

        
        
        dL[i]=(salida.salida-vector[i][4])/(float)df.registros;
        //printf("A=%.4f, real=%.4f, dif=%.4f\n",salida.salida,vector[i][4],dL[i]);

        error=error+((salida.salida-vector[i][4])*(salida.salida-vector[i][4]));

        //printf("\n\n");
          
    }
    printf("Error=%.2f\n\n\n",(error)/df.registros);

    /*Calculando el gradiente de la función de costo*/
   
    //matrix_print(matrix_init(1,df.registros,dL)); 
    
    //matrix_print(matrix_init(df.registros,3,dY));

    matrix gradiente_salida;
    gradiente_salida=matrix_product(matrix_init(1,df.registros,dL),matrix_init(df.registros,3,dY));//Primer gradiente
    //matrix_print(gradiente_salida);
    matrix O21_dw;
    O21_dw=matrix_init(df.registros,4, salidas_optimizacion);

    matrix gradiente_neurona1_capaoculta,gradiente_neurona2_capaoculta;
    gradiente_neurona1_capaoculta=matrix_product(escalar(matrix_init(1,df.registros,dL),salida.pesos[0]),O21_dw);//<--Gradiente de la neurona oculta 1

    gradiente_neurona2_capaoculta=matrix_product(escalar(matrix_init(1,df.registros,dL),salida.pesos[1]),O21_dw);//<---Gradiente de la neurona oculta 2
    

    /* <---Tengo que usar esa matriz y mutiplicarla 
    por la derivada de la suma de la primera neurona con respecto a las salidas de la primera capa*/
    matrix deltaNeuronaOculta1,deltaNeuronaOculta2;
    deltaNeuronaOculta1=escalar(matrix_init(1,df.registros,dL),salida.pesos[0]);
    deltaNeuronaOculta2=escalar(matrix_init(1,df.registros,dL),salida.pesos[1]);

    //matrix_print(matrix_sum(escalar(deltaNeuronaOculta1,ocult_A.pesos[0]),escalar(deltaNeuronaOculta2,ocult_B.pesos[0])));
    
    //printf("filas=%d,colum=%d",matrix_sum(escalar(deltaNeuronaOculta1,ocult_A.pesos[0]),escalar(deltaNeuronaOculta2,ocult_B.pesos[0])).filas,matrix_sum(escalar(deltaNeuronaOculta1,ocult_A.pesos[0]),escalar(deltaNeuronaOculta2,ocult_B.pesos[0])).columnas);

    float* data;
    data=malloc(df.registros*5*sizeof(float));
    for(i=0;i<df.registros;i++)
    {
        data[i*5+0]=vector[i][0];
        data[i*5+1]=vector[i][1];
        data[i*5+2]=vector[i][2];
        data[i*5+3]=vector[i][3];
        data[i*5+4]=(float)1;
    }
    

    matrix grad_entrada_1,grad_entrada_2,grad_entrada_3;
    grad_entrada_1=matrix_product(matrix_sum(escalar(deltaNeuronaOculta1,ocult_A.pesos[0]),escalar(deltaNeuronaOculta2,ocult_B.pesos[0])),matrix_init(df.registros,5,data));

    grad_entrada_2=matrix_product(matrix_sum(escalar(deltaNeuronaOculta1,ocult_A.pesos[1]),escalar(deltaNeuronaOculta2,ocult_B.pesos[1])),matrix_init(df.registros,5,data));

    grad_entrada_3=matrix_product(matrix_sum(escalar(deltaNeuronaOculta1,ocult_A.pesos[2]),escalar(deltaNeuronaOculta2,ocult_B.pesos[2])),matrix_init(df.registros,5,data));

    //printf("\nGradiente de capa de salida\n");
    //matrix_print(gradiente_salida);
    salida.pesos[0]-=gradiente_salida.data[0][0];
    salida.pesos[1]-=gradiente_salida.data[0][1];
    salida.pesos[2]-=gradiente_salida.data[0][2];



    //printf("\nGradiente 1 de capa oculta\n");
    //matrix_print(gradiente_neurona1_capaoculta);
    ocult_A.pesos[0]-=gradiente_neurona1_capaoculta.data[0][0];
    ocult_A.pesos[1]-=gradiente_neurona1_capaoculta.data[0][1];
    ocult_A.pesos[2]-=gradiente_neurona1_capaoculta.data[0][2];
    ocult_A.pesos[3]-=gradiente_neurona1_capaoculta.data[0][3];
    

    //printf("\nGradiente 2 de capa oculta\n");
    //matrix_print(gradiente_neurona2_capaoculta);
    ocult_B.pesos[0]-=gradiente_neurona2_capaoculta.data[0][0];
    ocult_B.pesos[1]-=gradiente_neurona2_capaoculta.data[0][1];
    ocult_B.pesos[2]-=gradiente_neurona2_capaoculta.data[0][2];
    ocult_B.pesos[3]-=gradiente_neurona2_capaoculta.data[0][3];


    //printf("\nGradiente 1 capa de entrada");
    //matrix_print(grad_entrada_1);
    in_A.pesos[0]-=grad_entrada_1.data[0][0];
    in_A.pesos[1]-=grad_entrada_1.data[0][1];
    in_A.pesos[2]-=grad_entrada_1.data[0][2];
    in_A.pesos[3]-=grad_entrada_1.data[0][3];
    in_A.pesos[4]-=grad_entrada_1.data[0][4];
    

    //printf("\nGradiente 2 capa de entrada");
    //matrix_print(grad_entrada_2);
    in_B.pesos[0]-=grad_entrada_2.data[0][0];
    in_B.pesos[1]-=grad_entrada_2.data[0][1];
    in_B.pesos[2]-=grad_entrada_2.data[0][2];
    in_B.pesos[3]-=grad_entrada_2.data[0][3];
    in_B.pesos[4]-=grad_entrada_2.data[0][4];

    //printf("\nGradiente 3 capa de entrada");
    //matrix_print(grad_entrada_3);
    in_C.pesos[0]-=grad_entrada_3.data[0][0];
    in_C.pesos[1]-=grad_entrada_3.data[0][1];
    in_C.pesos[2]-=grad_entrada_3.data[0][2];
    in_C.pesos[3]-=grad_entrada_3.data[0][3];
    in_C.pesos[4]-=grad_entrada_3.data[0][4];





}


int main()
{
    red();

    
}