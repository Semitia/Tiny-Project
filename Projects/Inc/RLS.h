#ifndef __RLS_H
#define __RLS_H
#include"../Inc/Matrix_Hub_v1.52/matrix.h"

/**
 * @brief  Recursive Least Square
*/
typedef struct __RLS_t{
    int dim;            //维度
    Matrix *theta;      //参数向量
    Matrix *P;          //协方差矩阵
    float lambda;       //遗忘因子

}RLS_t;

void rlsInit(RLS_t *rls, int dim, MATRIX_TYPE *theta, MATRIX_TYPE *P, MATRIX_TYPE lambda);
void rlsUpdate(RLS_t *rls, MATRIX_TYPE *x, MATRIX_TYPE y);

#endif