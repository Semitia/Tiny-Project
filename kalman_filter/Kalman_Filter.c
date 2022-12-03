#include<time.h>
#include "../inc/matrix.h"

/*模型构建：

*/
#define num 100
double my_f[INV][INV]={{1,1},{0,1}}, my_h[2][INV]={{0,1},{0,1}}, my_r[2][2]={{10000,0},{0,10000}};
double my_q[INV][INV]={{100,0},{0,100}}, my_p[INV][INV]={{1000,0},{0,1000}}, my_x[INV][INV]={{0},{10}};
Matrix_t _X, X, F, H, _P, P, Q, R, K;
FILE *fp = NULL;
int count = 0;
double pre_angle1 = 0, pre_angle2 = 0, pre_angle3;



void filter(Matrix_t Z)
{
    count++;
    pre_angle1 += Z.matrix[0][0];
    pre_angle2 += Z.matrix[1][0];
    pre_angle3 = (pre_angle1 + pre_angle2)/2;

    _X = mul_matrix(F,X);
    _P = add_matrix(mul_matrix(mul_matrix(F, P), tran_matrix(F)), Q);
    K  = mul_matrix(mul_matrix(_P,tran_matrix(H)), inv_matrix(add_matrix(mul_matrix(mul_matrix(H,_P),tran_matrix(H)),R)));
    X  = add_matrix(_X,mul_matrix(K,sub_matrix(Z,mul_matrix(H,_X) ) ) );
    P  = mul_matrix(sub_matrix(get_I(2),mul_matrix(K,H)),_P); 

    fprintf(fp,"%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",count, pre_angle1, pre_angle2, pre_angle3, X.matrix[0][0], X.matrix[1][0], Z.matrix[0][0], Z.matrix[1][0]);
    return;
}


int main()
{
    /*
    test_inv();
    */
    Matrix_t input;
    fp = fopen("kalman_filter.xls","w");
    K  = matrix_init2(2,2);
    _P = matrix_init2(INV,INV);
    F  = matrix_init(INV,INV,my_f);
    Q  = matrix_init(INV,INV,my_q);
    R  = matrix_init(2,2,my_r);
    H  = matrix_init(2,INV,my_h);
    P  = matrix_init(INV,INV,my_p);
    X  = matrix_init(INV,1,my_x);

    input.m = 2, input.n = 1;
    srand((unsigned)time( NULL ) );    
    //fprintf(fp,"Number\tP\tR\tK\tX\tZ\n");
    fprintf(fp,"Number\tpre_angle1\tpre_angle2\tpre_angle3\tangle\tgyro\tZ1\tZ2\n");
    for(int i=0; i<num; i++)
    {
        input.matrix[0][0] = (double)rand()/1638 ;//+ ((rand()%200)-100);
        input.matrix[1][0] = (double)rand()/1638 ;//d+ ((rand()%200)-100);
        filter(input);
    }


    fclose(fp);
    printf("finished");
    system("pause");
    return 0;
}


