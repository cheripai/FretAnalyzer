all:
	g++ lib/*.cpp src/*.cpp -o fret


clean:
	rm fret


run:
	./fret
