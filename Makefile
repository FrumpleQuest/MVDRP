compile:
	g++ MVDRP.cpp -o MVDRP.o

run:
	./MVDRP.o instance $i.txt $k $c $r 
	python Path_Visualizer.py

clean:
	rm *.o
	rm Visualized_Path.png
	rm path.txt
	rm output.txt

