all: main

main: 
	g++ -Wall -Werror -pedantic src/parser.cpp src/main.cpp -o main

clean:
	rm main data.txt

tests:
	python test/test_fret.py

run:
	./main
