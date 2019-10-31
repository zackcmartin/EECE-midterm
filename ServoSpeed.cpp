#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "GPIO.h"

using std::cin;
using std::cout;

  /**
* Calulate the pulse given the degree
*
* @param sPosition
* Degree position
*/
int degreeToOnDelay(int sPosition)
{
  return (sPosition * 10) + 600;
}

/**
 * Main method to run the robot arm
 */

int main()
{
  int servoNum;
  int servoPosStart;
  int servoPosEnd;
  int rotationSpeed;
  int servoPulseStart;
  int servoPulseEnd;

  while (true)
  {
    bool invalid = false;
    cout << "Enter a servo number between 1 and 5. \n 1 = Base \n 2 = Bicep \n 3 = Elbow \n 4 = Wrist \n 5 = Gripper \n";
    cin >> servoNum;

    //converts 1-5 to proper pin number
    switch (servoNum)
    {
    case 1:
      servoNum = 13;
      break;
    case 2:
      servoNum = 10;
      break;
    case 3:
      servoNum = 11;
      break;
    case 4:
      servoNum = 12;
      break;
    case 5:
      servoNum = 0;
      break;
    default:
      cout << "Invalid servo number. Must be an integer between 1 and 5. \n \n";
      invalid = true;
      break;
    }

    if (!invalid)
    {
      cout << "Enter the start position (angle) you'd like to start at \n";
      cin >> servoPosStart;
      cout << "Enter the end position (angle) you'd like to end at \n";
      cin >> servoPosEnd;
      if (servoPosStart < 0 || servoPosStart > 180 || servoPosEnd < 0 || servoPosEnd > 180)
      {
        cout << "Invalid servo position. The angle must be an integer between 0 and 180 \n \n";
        invalid = true;
      }
      if (!invalid)
      {
        cout << "Enter the speed (in degrees per second) you'd like the arm to move at \n";
        cin >> rotationSpeed;

        GPIO gpio(servoNum);
        cout << "here";

        //Gets pulse start and end
        servoPulseStart = degreeToOnDelay(servoPosStart);
        servoPulseEnd = degreeToOnDelay(servoPosEnd);

        int totalDegrees = abs(servoPosStart - servoPosEnd);

        int time = totalDegrees / rotationSpeed;

        int num_periods = time / 20000;

        gpio.GenerateVariablePWM(20000, servoPulseStart, servoPulseEnd, num_periods);

        cout << "\n Servo: " << servoNum << "Angle Start: " << servoPosStart << "Angle End: " << servoPosEnd << "Pulse Start (ms): " << servoPulseStart << "Pulse End (ms): " << servoPulseEnd << "\n \n";
      }
    }
  }

  return 0;
}
