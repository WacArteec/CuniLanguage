CC = g++

FLAGS = -c

all: hello
	language

hello: Objectives/main.o  Objectives/myassert.o Objectives/tree.o Objectives/lexical.o Objectives/syntactic.o Objectives/reader.o Objectives/translator.o
	$(CC) Objectives/main.o  Objectives/myassert.o Objectives/tree.o Objectives/lexical.o Objectives/syntactic.o Objectives/reader.o Objectives/translator.o -o language

Objectives/myassert.o: myassert/myassert.cpp myassert/myassert.h
	$(CC) $(FLAGS) myassert/myassert.cpp -o Objectives/myassert.o

Objectives/tree.o: tree/tree.cpp tree/tree.h
	$(CC) $(FLAGS) tree/tree.cpp -o Objectives/tree.o

Objectives/syntactic.o: Parser/syntactic.cpp Parser/syntactic.h
	$(CC) $(FLAGS) Parser/syntactic.cpp -o Objectives/syntactic.o

Objectives/lexical.o: Parser/lexical.cpp Parser/lexical.h
	$(CC) $(FLAGS) Parser/lexical.cpp -o Objectives/lexical.o

Objectives/reader.o: reader/reader.cpp reader/reader.h
	$(CC) $(FLAGS) reader/reader.cpp -o Objectives/reader.o

Objectives/translator.o: Translator/translator.cpp Translator/translator.h
	$(CC) $(FLAGS) Translator/translator.cpp -o Objectives/translator.o

Objectives/main.o: main.cpp
	$(CC) $(FLAGS) main.cpp -o Objectives/main.o