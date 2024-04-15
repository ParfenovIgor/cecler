all:
	g++ main.cpp lexer.cpp -o cecler

run:
	./cecler

clean:
	rm cecler