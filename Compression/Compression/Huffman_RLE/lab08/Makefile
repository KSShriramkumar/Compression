COMPILE = g++ -std=c++20
all:
rle:
	rm -r comps decomps
	mkdir comps decomps
	$(COMPILE) -g main.cpp -o main
	./main inputs/part1.txt 1
	diff inputs/part1.txt decomps/d1.txt
huffman:
	rm -r comps decomps
	mkdir comps decomps
	$(COMPILE) -g main.cpp -o main
	./main inputs/part2.txt 2
	diff inputs/part2.txt decomps/d2.txt
clean:
	rm -rf main
	rm -rf *~
