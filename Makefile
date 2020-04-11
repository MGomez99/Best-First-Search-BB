  
files= BestFirstSearch.cpp

all: $(files)
	g++ -o BestFirstSearch BestFirstSearch.cpp -std=c++14


clean:
	rm -rf output.txt
	rm -rf BestFirstSearch


run: all
	./BestFirstSearch input.txt output.txt

test: all
	./BestFirstSearch testinput.txt output.txt
	
winclean:
	del BestFirstSearch.exe
	del output.txt