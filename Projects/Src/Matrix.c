#include "../Inc/Matrix.h"

/**
 * @brief printf the Matrix
 * @param m 
 */
void printf_matrix(Matrix_t *Mat)
{
    printf("%d x %d\r\n",Mat->m,Mat->n);
    for(int i=0; i<Mat->m; i++)
    {
        for(int j=0; j<Mat->n; j++)
        { printf("%.3f ",Mat->matrix[i][j]); }
        printf("\r\n");
    }
    printf("\r\n");
    return;
}

/**
 * @brief Matrix Mulplication
 * @param m1 
 * @param m2 
 * @return Matrix_t* 
 */
Matrix_t *multiply_matrix(Matrix_t *m1, Matrix_t *m2)
{
    if (m1->n != m2->m) {return NULL;}
    Matrix_t *ans;
    ans = (Matrix_t*)malloc(sizeof(Matrix_t));
    ans->m = m1->m;
    ans->n = m2->n;

    for(int i=0; i < m1->m; i++)
    {
        for(int j=0; j < m2->n; j++)
        {
            float sum=0;
            for(int k=0; k < m1->n; k++)
            { sum += m1->matrix[i][k]*m2->matrix[k][j]; }
            ans->matrix[i][j] = sum;
        }
    }
    return ans;
}