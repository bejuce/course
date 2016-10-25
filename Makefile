FLAGS=-c -g
run: main.o data.o matrix.o msg.o algoritm.o compare.o
	g++ main.o data.o matrix.o msg.o algoritm.o compare.o -O3

gen: test.o data.o matrix.o msg.o algoritm.o compare.o
	g++ test.o msg.o data.o matrix.o algoritm.o compare.o -g -o gen

analize: main2.o data.o matrix.o msg.o algoritm.o compare.o
	g++ main2.o data.o matrix.o msg.o algoritm.o compare.o -O3 -o analize

main2.o: main2.cpp headers.h
	g++ main2.cpp $(FLAGS)

main.o: main.cpp headers.h
	g++ main.cpp $(FLAGS)

data.o: data.cpp headers.h
	g++ data.cpp $(FLAGS)

matrix.o: matrix.cpp headers.h
	g++ matrix.cpp $(FLAGS)

msg.o: msg.cpp headers.h
	g++ msg.cpp $(FLAGS)

algoritm.o: algoritm.cpp headers.h
	g++ algoritm.cpp $(FLAGS)

compare.o: compare.cpp headers.h
	g++ compare.cpp $(FLAGS)

test.o: test.cpp headers.h
	g++ test.cpp $(FLAGS)

clean: 
	rm -rf *.o a.out analize gen
