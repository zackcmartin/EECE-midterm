#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include "GPIO.cpp"
using std::cin;
using std::cout;


int degreeToOnDelay(int sPosition){
    return (sPosition * 10) + 600;
}

int main()
{
    int servoNum;
    int servoPos;
    int servoPulse;

    while (true)
    {
        bool invalid = false;
        cout << "Enter a servo number between 1 and 5. \n 1 = Base \n 2 = Bicep \n 3 = Elbow \n 4 = Wrist \n 5 = Gripper \n";
        cin >> servoNum;
        if (servoNum < 1 || servoNum > 5)
        {
            cout << "Invalid servo number. Must be an integer between 1 and 5. \n \n";
            invalid = true;
        }
        if (!invalid)
        {
            cout << "Enter the position (angle) you'd like for this servo (between 0° and 180°) \n";
            cin >> servoPos;
            if (servoPos < 0 || servoPos > 180)
            {
                cout << "Invalid servo position. The angle must be an integer between 0 and 180 \n \n";
                invalid = true;
            }
            if(!invalid){
                GPIO gpio(servoNum);

                servoPulse = degreeToOnDelay(servoPos);

                gpio.GeneratePWM(20000, servoPulse, 400);

                cout << "Servo: " << servoNum << "Angle: " << servoPos << "Pulse (ms): " << servoPulse << "\n \n";
            }
        }
    }

    return 0;
}