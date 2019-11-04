#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "GPIO.h"
#include <pthread.h>

using namespace std;

GPIO *base = new GPIO(13);
GPIO *bicep = new GPIO(10);
GPIO *elbow = new GPIO(11);
GPIO *wrist = new GPIO(12);
GPIO *gripper = new GPIO(0);

int NUM_THREADS = 5;
GPIO *gpioList[5] = {base, bicep, elbow, wrist, gripper};

//These are the positions for each move
int standStraightPositions[5] = {90, 75, 80, 73, 90};
int moveBaseRight[5] = {110, 75, 80, 0, 90};
int moveRight[5] = {110, 75, 150, 110, 170};
int gripRight[5] = {110, 75, 150, 110, 0};

//This is the speed for the move
int moveRightSpeed[5] = {30, 30, 30, 30, 30};

//Data for a position move (without speeed)
struct threadDataPos
{
    GPIO *gpio;
    int numPeriods;
    int degree;
};


//Data for a move with speed
struct threadDataSpeed
{
    GPIO *gpio;
    int startPos;
    int endPos;
    int rotationSpeed;
};

int servoPulseStart;
int servoPulseEnd;


//Converts from degrees to pulse
int degreeToOnDelay(int sPosition)
{
    return (sPosition * 10) + 600;
}

void *movePos(void *threadStruct)
{
    struct threadDataPos *getData;

    getData = (struct threadDataPos *)threadStruct;
    GPIO *gpio = getData->gpio;
    int numPeriods = getData->numPeriods;
    int degree = getData->degree;

    int pulse = degreeToOnDelay(degree);

    gpio->GeneratePWM(20000, pulse, numPeriods);
    return NULL;
}

void *moveSpeed(void *threadStruct)
{
    struct threadDataSpeed *getData;

    getData = (struct threadDataSpeed *)threadStruct;
    GPIO *gpio = getData->gpio;
    int startPos = getData->startPos;
    int endPos = getData->endPos;
    int rotationSpeed = getData->rotationSpeed;

    servoPulseStart = degreeToOnDelay(startPos);
    servoPulseEnd = degreeToOnDelay(endPos);

    cout << "START POS" << startPos << endl;

     cout << "END POS" << endPos << endl;


    int num_periods = ((abs(startPos - endPos) / rotationSpeed) * 50);

    if(num_periods == 0){
        num_periods = 1;
    }

    cout << "THIS IS THE NUMBER!!!!" << num_periods << endl;

    // int time = totalDegrees / rotationSpeed;

    // int num_periods = time * 50;

    gpio->GenerateVariablePWM(20000, servoPulseStart, servoPulseEnd, num_periods);
    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS], threads2[NUM_THREADS], threads3[NUM_THREADS], threads4[NUM_THREADS];
    int rc;
    int i;

    //This makes the arm stand up straight for a second (50).
       for( i = 0; i < NUM_THREADS; i++ ) {
          cout << "main() : creating thread, " << i << endl;
          threadDataPos threadData = {gpioList[i], 50, standStraightPositions[i]};
          rc = pthread_create(&threads[i], NULL, movePos, (void *)&threadData);

          if (rc) {
             cout << "Error:unable to create thread," << rc << endl;
             exit(-1);
          }
       }

       for( i = 0; i < NUM_THREADS; i++) {
           pthread_join(threads[i], NULL);
       }

       cout << "passed the joins" << endl;

    //This moves the base of the arm to the starting position to pick up the bottles 
           for( i = 0; i < NUM_THREADS; i++ ) {
          cout << "main() : creating threadTWO, " << i << endl;
          threadDataPos threadData = {gpioList[i], 50, moveBaseRight[i]};
          rc = pthread_create(&threads2[i], NULL, movePos, (void *)&threadData);

          if (rc) {
             cout << "Error:unable to create thread," << rc << endl;
             exit(-1);
          }
       }

         for( i = 0; i < NUM_THREADS; i++) {
           pthread_join(threads2[i], NULL);
       }

    //This is the loop in which I am trying to manipulate the speed of the robot arm using
    //GenerateVariablePMW. It is supposed to move down to pick up the water bottle.
    //This is currently not working correctly
    for (i = 0; i < NUM_THREADS; i++)
    {
        // if (i == 3)
        // {
            cout << "main() : creating threadTHREE, " << i << endl;
            threadDataSpeed threadData = {gpioList[i], moveBaseRight[i], moveRight[i], moveRightSpeed[i]};
            rc = pthread_create(&threads3[i], NULL, moveSpeed, (void *)&threadData);

            if (rc)
            {
                cout << "Error:unable to create thread," << rc << endl;
                exit(-1);
            }
        // }




//This commeneted out code is the code to move the arm to pick up the water bottle without
//manipulating the speed

        // else{
        //     cout << "main() : creating threadTHREE, " << i << endl;
        //     threadDataPos threadData = {gpioList[i], 100, moveRight[i]};
        //     rc = pthread_create(&threads3[i], NULL, movePos, (void *)&threadData);

        //     if (rc)
        //     {
        //         cout << "Error:unable to create thread," << rc << endl;
        //         exit(-1);
        //     }
        // }
    }

//This makes the arm grip onto the cap of the water bottle (currently set for 8 seconds (400))
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads3[i], NULL);
    }

           for( i = 0; i < NUM_THREADS; i++ ) {
          cout << "main() : creating threadFOUR, " << i << endl;
          threadDataPos threadData = {gpioList[i], 400, gripRight[i]};
          rc = pthread_create(&threads4[i], NULL, movePos, (void *)&threadData);

          if (rc) {
             cout << "Error:unable to create thread," << rc << endl;
             exit(-1);
          }
       }

         for( i = 0; i < NUM_THREADS; i++) {
           pthread_join(threads4[i], NULL);
       }
}