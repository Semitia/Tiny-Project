#include"../Inc/RLS.h"
#include<stdio.h>
#include<time.h>

#define BIAS_SCALE 50
int main() {
    FILE *fp = fopen("../Data/rls.csv", "w");
    if(fp == NULL){
        printf("File open failed!\n");
        return 0;
    }
    
    fprintf(fp, "i\ty\n");
    for (int i = 0; i < 10; i++) {
        MATRIX_TYPE x_in[3] = {i * i, i, 1}; // 输入向量
        MATRIX_TYPE y = 2 * i * i + 5 * i + 6; // 输出值
        // 添加一些随机噪声
        y += (((double)rand() / RAND_MAX) - 0.5)*BIAS_SCALE;

        fprintf(fp, "%d\t%f\n", i, y);
    }

    fclose(fp);
    return 0;
}