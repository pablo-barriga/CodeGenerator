P3.out : Project3.o LexicalAnalyzer.o SyntacticalAnalyzer.o CodeGenerator.o
	g++ -g -o P3.out Project3.o LexicalAnalyzer.o SyntacticalAnalyzer.o CodeGenerator.o

Project3.o : Project3.cpp SyntacticalAnalyzer.h
	g++ -g -c Project3.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp

CodeGenerator.o : CodeGenerator.cpp CodeGenerator.h LexicalAnalyzer.h
	g++ -g -c CodeGenerator.cpp

clean : 
	rm [SPC]*.o P3.out *.gch

submit : Project3.cpp SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp CodeGenerator.h CodeGenerator.cpp makefile README.txt
	rm -rf barrigaP3
	mkdir barrigaP3
	cp Project3.cpp barrigaP3
	cp SyntacticalAnalyzer.h barrigaP3
	cp SyntacticalAnalyzer.cpp barrigaP3
	cp CodeGenerator.h barrigaP3
	cp CodeGenerator.cpp barrigaP3
	cp makefile barrigaP3
	cp README.txt barrigaP3
	tar cfvz barrigaP3.tgz barrigaP3
	cp barrigaP3.tgz ~tiawatts/cs460drop
