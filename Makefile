
build:
	g++ -o sendsample main.cc

all: build

run: build
	./sendsample


clean:
	rm sendsample
	rm *.o


