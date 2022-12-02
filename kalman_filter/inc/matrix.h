#ifndef __MATRIX_H
#define __MATRIX_H

#include<math.h>
#include<stdio.h>
#include<stdlib.h>

#define Matrix_MX 10


/**
 * @brief 
 * @param m 行数
 * @param n 列数
 * @param matrix 矩阵
 */
typedef struct __Matrix_t{
    int m;
    int n;
    double matrix[Matrix_MX][Matrix_MX];
}Matrix_t;

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
 * @brief 
 * 
 * @param m1 
 * @param m2 
 * @return Matrix_t 
 */
Matrix_t add_matrix(Matrix_t m1, Matrix_t m2)
{
    int i,j;
    for(i=0; i<m1.m; i++)
    {
        for(j=0; j<m1.n; j++)
        { m1.matrix[i][j] += m2.matrix[i][j]; }
    }
    return m1;
}

/**
 * @brief Matrix Mulplication
 * @param m1 
 * @param m2 
 * @return Matrix_t
 */
Matrix_t mul_matrix(Matrix_t m1, Matrix_t m2)
{
    //if (m1->n != m2->m) {return 0;}
    Matrix_t ans;
    ans.m = m1.m;
    ans.n = m2.n;

    for(int i=0; i < m1.m; i++)
    {
        for(int j=0; j < m2.n; j++)
        {
            float sum=0;
            for(int k=0; k < m1.n; k++)
            { sum += m1.matrix[i][k]*m2.matrix[k][j]; }
            ans.matrix[i][j] = sum;
        }
    }
    return ans;
}

Matrix_t inv_matrix(Matrix_t M)
{
    Matrix_t ans;
    return ans;
}

/**
 * @brief 返回转置矩阵
 * 
 * @param M 
 * @return Matrix_t 
 */
Matrix_t tran_matrix(Matrix_t M)
{
    int i,j;
    Matrix_t ans;
    ans.m = M.m, ans.n = M.n;
    for(i=0; i<M.m; i++)
    {
        for(j=0; j<M.n; j++)
        { ans.matrix[i][j] = M.matrix[j][i]; }
    }
    return ans;
}

Matrix_t matrix_init(int m, int n, double **matrix)
{
    int i,j;
    Matrix_t ans;
    ans.m = m;
    ans.n = n;
    for(i=0; i<m; i++)
    {
        for(j=0; j<n; j++)
        { ans.matrix[i][j] = matrix[i][j]; }
    }
    return ans;
}

#endif

