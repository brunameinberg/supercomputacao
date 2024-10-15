#include <vector>
#include <iostream>
#include <unistd.h>
#include <omp.h>


double conta_complexa(int i) {
    return 2 * i;
}

int main() {
    int N = 10000; 
    std::vector<double> vec;
    double init_time = omp_get_wtime();
    for (int i = 0; i < N; i++) 
    #pragma omp critical
    {
        vec.push_back(conta_complexa(i));
    }

    double final_time = omp_get_wtime() - init_time;

    std::cout << "tempo final" << final_time << std::endl;

    for (int i = 0; i < N; i++) {
        std::cout << i << " ";
    }

    return 0;
}