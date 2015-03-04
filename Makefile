all: main

main: 
	g++ -Wall -Werror -pedantic src/parser.cpp src/main.cpp -o main

clean:
	rm main

run:
	./main
