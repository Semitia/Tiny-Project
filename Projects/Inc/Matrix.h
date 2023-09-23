#ifndef __MATRIX_H
#define __MATRIX_H

#include<stddef.h>
#define Matrix_MX 5

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

void printf_matrix(Matrix_t *Mat);

#endif 