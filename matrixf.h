
typedef struct{
float **data;
int filas;
int columnas;
}matrix;


matrix matrix_init(int filas, int columnas, float *M);

void matrix_print(matrix M);

matrix matrix_sum(matrix A, matrix B);

matrix escalar(matrix A, float escalar);

matrix matrix_product(matrix A, matrix B);

matrix matrix_traspuesta(matrix A);