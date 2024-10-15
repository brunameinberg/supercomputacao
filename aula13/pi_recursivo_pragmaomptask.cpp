#include <omp.h>
#include <iostream>
#include <iomanip>
static long num_steps = 1024l*1024*1024*2;

#define MIN_BLK  1024*1024*1024

double sum = 0;

void pi_r(long Nstart, long Nfinish, double step) {
    if (Nfinish - Nstart < MIN_BLK) {
        for (long i = Nstart; i < Nfinish; i++) {
            double x = (i+0.5)*step;
            #pragma omp atomic
            sum += 4.0/(1.0 + x*x);
        }
    } else {
        long iblk = (Nfinish - Nstart)/2;
        #pragma omp task
        pi_r(Nstart, Nstart + iblk, step);
        #pragma omp task
        pi_r(Nstart + iblk, Nfinish, step);
        #pragma omp taskwait
    }
}

int main () {
    long i;
    double step, pi;
    double init_time, final_time;
    step = 1.0/(double) num_steps;

    init_time = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        pi_r(0, num_steps, step);
    }
    pi = step * sum;
    final_time = omp_get_wtime() - init_time;

    std::cout << "MIN_BLK" << MIN_BLK << std::endl;

    std::cout << "for " << num_steps << " steps pi = " << std::setprecision(15) << pi << " in " << final_time << " secs\n";
}
