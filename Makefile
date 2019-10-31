output: ServoSpeed.o GPIO.o
	g++ ServoSpeed.o GPIO.o -o output	

ServoSpeed.o: ServoSpeed.cpp GPIO.h
	g++ -g -Wall -c ServoSpeed.cpp

GPIO.o: GPIO.cpp GPIO.h
	g++ -g -Wall -c GPIO.cpp

clean:
	rm ServoSpeed.o GPIO.o output
