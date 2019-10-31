#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "GPIO.h"
#include <thread>

using std::cin;
using std::cout;
using std::thread;

GPIO base(13);
GPIO bicep(10);
GPIO elbow(11);
GPIO wrist(12);
GPIO gripper(0);

int servoPulseStart;
int servoPulseEnd;

int degreeToOnDelay(int sPosition)
{
    return (sPosition * 10) + 600;
}

int main()
{
    standStraight();
}

//Moves to a specified location
void movePos(GPIO servo, int degree, int numPeriods)
{
    int pulse = degreeToOnDelay(degree);

    servo.GeneratePWM(20000, pulse, numPeriods);
}

//Moves to a location at a specified speed
void moveSpeed(GPIO servo, int startPos, int endPos, int rotationSpeed)
{
    servoPulseStart = degreeToOnDelay(startPos);
    servoPulseEnd = degreeToOnDelay(endPos);

    int totalDegrees = abs(startPos - endPos);

    int time = totalDegrees / rotationSpeed;

    int num_periods = time * 50;

    servo.GenerateVariablePWM(20000, servoPulseStart, servoPulseEnd, num_periods);
}

//Puts the entire robot arm at 90 degrees
void standStraight()
{
    thread baseThreadP(movePos, base, 90, 400);
    thread bicepThreadP(movePos, base, 75, 400);
    thread elbowThreadP(movePos, base, 80, 400);
    thread wristThreadP(movePos, base, 73, 400);
    thread gripperThreadP(movePos, base, 90, 400);

    baseThreadP.join();
    bicepThreadP.join();
    elbowThreadP.join();
    wristThreadP.join();
    gripperThreadP.join();
}