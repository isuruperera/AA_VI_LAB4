#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

const int S1 = 32768;
const int S2 = 262144;
const int B1 = 64;
const int B2 = 64;
const int test_iterations = 10;
const long sub_iterations_a = 100000; // iterations for 6553600000 mem accesses
const long sub_iterations_b = 6400000; // iterations for 6553600000 mem accesses
const long sub_iterations_c = 800000; // iterations for 6553600000 mem accesses
const long mem_access_cycles = 6553600000;

void subroutine_a(char *buffer) {
    for(int i=0;i<2*S1;i++){
        buffer[i]++;
    }
}

void subroutine_b(char *buffer) {
    for(int i=0;i<2*S1;i+=B1){
        buffer[i]++;
    }
}

void subroutine_c(char *buffer) {
    for(int i=0;i<2*S2;i+=B2){
        buffer[i]++;
    }
}

void test_a(char *buffer, long *results){
    for(int i=0;i<test_iterations;i++){
        // init buffer with random values
        for(int j=0;j<2*S1;j++){
            buffer[j] = (char) random();
        }

        // measure time
        long stop, start;
        start = clock();
        for(int j=0;j<sub_iterations_a;j++){
            subroutine_a(buffer);
        }
        stop = clock();
        results[i] = stop - start;
    }
}

void test_b(char *buffer, long *results){
    for(int i=0;i<test_iterations;i++){
        // init buffer with random values
        for(int j=0;j<2*S1;j++){
            buffer[j] = (char) random();
        }

        // measure time
        long stop, start;
        start = clock();
        for(int j=0;j<sub_iterations_b;j++){
            subroutine_b(buffer);
        }
        stop = clock();
        results[i] = stop - start;
    }
}

void test_c(char *buffer, long *results){
    for(int i=0;i<test_iterations;i++){
        // init buffer with random values
        for(int j=0;j<2*S2;j++){
            buffer[j] = (char) random();
        }

        // measure time
        long stop, start;
        start = clock();
        for(int j=0;j<sub_iterations_c;j++){
            subroutine_c(buffer);
        }
        stop = clock();
        results[i] = stop - start;
    }
}

int main() {
    char buffer_a[2*S1];
    long results_a[test_iterations];
    test_a(buffer_a,results_a);
    char buffer_b[2*S1];
    long results_b[test_iterations];
    test_b(buffer_b,results_b);
    char buffer_c[2*S2];
    long results_c[test_iterations];
    test_c(buffer_c,results_c);

    // calculate averages
    double avg_time_a = 0.0;
    double avg_time_b = 0.0;
    double avg_time_c = 0.0;
    for(int i=0;i<test_iterations;i++){
        avg_time_a+=((double)results_a[i])/CLOCKS_PER_SEC;
        avg_time_b+=((double)results_b[i])/CLOCKS_PER_SEC;
        avg_time_c+=((double)results_c[i])/CLOCKS_PER_SEC;
    }
    avg_time_a = (avg_time_a/test_iterations);
    avg_time_b = (avg_time_b/test_iterations);
    avg_time_c = (avg_time_c/test_iterations);

    double byte_access_time_a = avg_time_a/mem_access_cycles;
    double byte_access_time_b = avg_time_b/mem_access_cycles;
    double byte_access_time_c = avg_time_c/mem_access_cycles;

    double bandwidth_a = (mem_access_cycles*1024*1024)/avg_time_a;
    double bandwidth_b = (mem_access_cycles*1024*1024)/avg_time_b;
    double bandwidth_c = (mem_access_cycles*1024*1024)/avg_time_c;

    printf("Number of Memory Access Cycles: %ld \n",mem_access_cycles);
    printf("Number of Test iterations: %d \n",test_iterations);

    printf("AVG Time Subroutine A: %f sec\n",avg_time_a);
    printf("AVG Time Subroutine B: %f sec\n",avg_time_b);
    printf("AVG Time Subroutine C: %f sec\n",avg_time_c);

    printf("Bandwidth Subroutine A: %f MB/s\n",bandwidth_a);
    printf("Bandwidth Subroutine B: %f MB/s\n",bandwidth_b);
    printf("Bandwidth Subroutine C: %f MB/s\n",bandwidth_c);

}


