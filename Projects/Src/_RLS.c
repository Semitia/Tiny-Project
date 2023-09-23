#include"../Inc/RLS.h"
#include<stdio.h>
#include<time.h>

/**
 * @brief  递推最小二乘法初始化
 * @param dim 维度
 * @param theta 参数向量
 * @param P 协方差矩阵
*/
void rlsInit(RLS_t *rls, int dim, MATRIX_TYPE *theta, MATRIX_TYPE *P, MATRIX_TYPE lambda){
    rls->lambda = lambda;
    rls->dim = dim;
    rls->theta = Matrix_gen(rls->dim, 1, theta);
    rls->P = Matrix_gen(rls->dim, rls->dim, P);
    return;
}

/**
 * @brief  根据新输入更新参数
 * @param x 新输入（数组）
 * @param y 新输出
*/
void rlsUpdate(RLS_t *rls, MATRIX_TYPE *x_in, MATRIX_TYPE y) {
    //print theta
    printf("theta:\n");
    M_print(rls->theta);

    Matrix *x = Matrix_gen(rls->dim, 1, x_in);
    MATRIX_TYPE e = y - M_mul(M_T(rls->theta),x)->data[0];         //注意data是一维数组

    printf("error: %f\n", e);

    /*计算增益矩阵*/
    /*                  p(t-1) x(t)
        K(t) = --------------------------------
                lambda + x(t)^T P(t-1) x(t)       */
    Matrix *up = M_mul(rls->P, x);
    Matrix *down = M_mul(M_mul(M_T(x), rls->P), x);
    //Matrix *K = M_numul(M_mul(rls->P, x), 1/(M_mul(M_mul(M_T(x),rls->P), x)->data[0]+rls->lambda));
    Matrix *K = M_numul(up, 1/(down->data[0]+rls->lambda));
    /*更新参数*/
    rls->theta = M_add_sub(1, rls->theta, 1, M_numul(K, e));
    /*更新协方差矩阵*/
    /*            1    --                            --
        P(t) = --------| P(t-1) - K(t) x(t)^T P(t-1)  |
                lambda --                            --  */
    rls->P = M_numul(M_add_sub(1, rls->P, 1, M_mul(M_mul(K, M_T(x)), rls->P)), 1/rls->lambda);
    
    M_free(up);
    M_free(down);
    M_free(x);
    M_free(K);
    return;
}

#define BIAS_SCALE 50
int main() {
    FILE *fp = fopen("../Data/rls.xls", "w");
    if(fp == NULL){
        printf("File open failed!\n");
        return 0;
    }

    RLS_t rls;
    MATRIX_TYPE theta[3] = {0, 0, 0};
    MATRIX_TYPE P[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    rlsInit(&rls, 3, theta, P, 0.99);
    srand(time(NULL)); // 初始化随机数生成器

        for (int i = 0; i < 10; i++) {
        MATRIX_TYPE x_in[3] = {i * i, i, 1}; // 输入向量
        MATRIX_TYPE y = 2 * i * i + 5 * i + 6; // 输出值
        // 添加一些随机噪声
        y += (((double)rand() / RAND_MAX) - 0.5)*BIAS_SCALE;

        fprintf(fp, "%d\t%f\n", i, y);
        rlsUpdate(&rls, x_in, y);
    }

        fclose(fp);
    printf("Estimated parameters: %f %f %f\n", 
        rls.theta->data[0], rls.theta->data[1], rls.theta->data[2]);
    return 0;
}