#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define numVal 1000000
double realPiValue = 3.141592653589793238462643383279;
double calculatedTotalPiValue, maxRange;
int threadCount;

void delay(int number_of_seconds) {
    int milli_seconds = 1000 * number_of_seconds;

    clock_t start_time = clock();

    while (clock() < start_time + milli_seconds);
}

void *threadFunc(void *parg) {
    int thread_id = *((int *) parg);
    int end, start;
    double thread_sum = 0;

    start = (thread_id * (int) maxRange);
    end = (thread_id + 1) * (int) maxRange;

    if (thread_id + 1 == threadCount) {
        end = numVal;
    }

    for (int x = start; x < end; x++) {
        thread_sum += 4 * (pow(-1, x) / (2 * x + 1));
    }

    calculatedTotalPiValue += thread_sum;
}

void piNumberSimulation() {
    int thread_range=threadCount;

    for(int q = 1; q <= thread_range; q++) {
        threadCount = q;
        maxRange = (double) numVal / (double) threadCount;

        if(maxRange > floor(maxRange)) {
            maxRange = floor(maxRange);
        }

        struct timeval begin, end;
        gettimeofday(&begin, 0);

        pthread_t tid[threadCount];
        for(int i = 0; i < threadCount; i++) {
            int *thread_id;
            thread_id = (int *) malloc(sizeof (int));
            *thread_id = i;
            pthread_create(&tid[i], NULL, threadFunc, (void *) thread_id);
        }

        for(int j = 0; j < threadCount; j++) {
            pthread_join(tid[j], NULL);
        }
        gettimeofday(&end, 0);

        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double elapsed = seconds + microseconds*1e-6;

        printf("\nStats for %d thread: \n", q);
        printf("Calculated value: %.10f\nReal value: %.10f\nApproximation error: %.10f\nComputation time: %.8f second\n", calculatedTotalPiValue, realPiValue, realPiValue-calculatedTotalPiValue, elapsed);
        calculatedTotalPiValue = 0;
    }
}

void multiThreadingPiNumberSimulation(int threadNumber) {
    printf("\nSystem running with: %d thread\n", threadNumber);
    threadCount = threadNumber;
    piNumberSimulation();
    delay(2);
}

int main() {
    multiThreadingPiNumberSimulation(1);
    multiThreadingPiNumberSimulation(2);
    multiThreadingPiNumberSimulation(4);
    multiThreadingPiNumberSimulation(8);
    multiThreadingPiNumberSimulation(16);
    multiThreadingPiNumberSimulation(32);
    multiThreadingPiNumberSimulation(64);
    return 0;
}
