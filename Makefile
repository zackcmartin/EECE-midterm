output: Midterm.o GPIO.o
	g++ Midterm.o GPIO.o -o output -lpthread

Midterm.o: Midterm.cpp GPIO.h
	g++ -g -Wall -c Midterm.cpp -lpthread

GPIO.o: GPIO.cpp GPIO.h
	g++ -g -Wall -c GPIO.cpp

clean:
	rm Midterm.o GPIO.o output
