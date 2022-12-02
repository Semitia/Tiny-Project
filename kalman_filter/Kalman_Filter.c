#include<time.h>
#include "../inc/matrix.h"

/*模型构建：

*/
#define num 100
double my_f[1][1],my_h[1][1],my_r[1][1],my_q[1][1],z;
Matrix_t _X, X, F, H, _P, P, Q, R, K;
FILE *fp = NULL;
int count = 0;
void filter(Matrix_t Z)
{
    count++;
    _X = mul_matrix(F,X);
    _P = add_matrix(mul_matrix(mul_matrix(F, P), tran_matrix(F)), Q);
    /**/
    K.matrix[0][0] = _P.matrix[0][0]/(_P.matrix[0][0]+R.matrix[0][0]);
    /**/
    X.matrix[0][0] =_X.matrix[0][0] + K.matrix[0][0]*(Z.matrix[0][0]-_X.matrix[0][0]);
    P.matrix[0][0] = (1-K.matrix[0][0])*_P.matrix[0][0];
    //printf("X:%.2f Z:%.2f \r\n",X.matrix[0][0], Z.matrix[0][0]);
    fprintf(fp,"%d\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\n",count,P.matrix[0][0],R.matrix[0][0],K.matrix[0][0],X.matrix[0][0],Z.matrix[0][0]) ;

    return;
}


int main()
{
    Matrix_t input;
    fp = fopen("kalman_filter.xls","w");

    K.m = 1, K.n = 1;
    F.m = 1, F.n = 1, F.matrix[0][0] = 1;
    Q.m = 1, Q.n = 1, Q.matrix[0][0] = 50000;
    R.m = 1, R.n = 1, R.matrix[0][0] = 500000;
    P.m = 1, P.n = 1, P.matrix[0][0] = 1;    
    X.m = 1, X.n = 1, X.matrix[0][0] = 0;
    input.m = 1, input.n = 1;
    srand((unsigned)time( NULL ) );    
    fprintf(fp,"Number\tP\tR\tK\tX\tZ\n");
    for(int i=0; i<num; i++)
    {
        input.matrix[0][0] =(double) i*100 + (rand()%3000 - 1500);
        //printf("I^2:%d ",i*i);
        filter(input);

    }


    fclose(fp);
    printf("finished");
    system("pause");
    return 0;
}


