compile:
	g++ MVDRP.cpp -o MVDRP.o

run:
	./MVDRP.o instance $i.txt $k $c $r 

clean:
	rm *.o

