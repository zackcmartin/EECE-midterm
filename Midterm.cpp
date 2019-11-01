#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "GPIO.h"
#include <pthread.h>

using namespace std;


GPIO base(13);
GPIO bicep(10);
GPIO elbow(11);
GPIO wrist(12);
GPIO gripper(0);

int NUM_THREADS = 5;
GPIO gpioList[5] = {base, bicep, elbow, wrist, gripper};
int standStraightPositions[5] = {90, 75, 80, 73, 160};
int movePositions[5] = {0, 0, 80, 73, 160};

struct threadDataPos {
	GPIO &gpio;
    int numPeriods;
	int degree;
};

int servoPulseStart;
int servoPulseEnd;

int degreeToOnDelay(int sPosition)
{
    return (sPosition * 10) + 600;
}


void *standStraight(void *threadStruct){
    struct threadDataPos *getData;

    getData = (struct threadDataPos *) threadStruct;
    GPIO gpio = getData->gpio;
    int numPeriods = getData->numPeriods;
    int degree = getData->degree;

    int pulse = degreeToOnDelay(degree);

    gpio.GeneratePWM(20000, pulse, numPeriods);
    return NULL;
}


int main()
{
    pthread_t threads[NUM_THREADS];
    pthread_t threads2[NUM_THREADS];
   int rc;
   int i;
   
   for( i = 0; i < NUM_THREADS; i++ ) {
      cout << "main() : creating thread, " << i << endl;
      threadDataPos threadData = {gpioList[i], 400, standStraightPositions[i]};
      rc = pthread_create(&threads[i], NULL, standStraight, (void *)&threadData);


      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }
   
   for( i = 0; i < NUM_THREADS; i++) {
       pthread_join(threads[i], NULL);
   }

    for( i = 0; i < NUM_THREADS; i++ ) {
      cout << "main() : creating thread, " << i << endl;
      threadDataPos threadData = {gpioList[i], 400, movePositions[i]};
      rc = pthread_create(&threads2[i], NULL, standStraight, (void *)&threadData);


      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }



}