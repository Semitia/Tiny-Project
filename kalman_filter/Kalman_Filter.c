#include<time.h>
#include "../inc/matrix.h"

/*模型构建：

*/
#define num 100
double my_f[1][1],my_h[1][1],my_r[1][1],my_q[1][1];
Matrix_t _X, X, F, H, _P, P, Q, R;

void filter(Matrix_t Z)
{
    _X = multiply_matrix(F,X);
    _P = add_matrix(multiply_matrix(multiply_matrix(F, P), tran_matrix(F)), Q);
    
}

int main()
{


    X.matrix[1][1] = 0;
    srand((unsigned)time( NULL ) );     
    for(int i=0; i<num; i++)
    {
        double input[1][1] = i*i + (rand()%100);
        filter(matrix_init(1,1,input));

    }
    return 0;
}


