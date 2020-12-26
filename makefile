cluster: build/read_binary_file.o build/bucket.o build/exhausting.o build/lsh.o build/kmeans.o build/cluster.o build/main.o
	g++ -O3 build/main.o build/read_binary_file.o build/bucket.o build/exhausting.o build/lsh.o build/hypercube.o build/kmeans.o build/cluster.o -o cluster

lsh: build/main.o build/read_binary_file.o build/bucket.o build/exhausting.o build/lsh.o build/kmeans.o build/cluster.o 
	g++ -O3 build/main.o build/read_binary_file.o build/bucket.o build/exhausting.o build/lsh.o build/kmeans.o build/cluster.o -o search

build/main.o: src/main.cpp
	g++ -O3 -c src/main.cpp -o build/main.o

build/read_binary_file.o: src/read_binary_file.cpp 
	g++ -O3 -c src/read_binary_file.cpp -o build/read_binary_file.o

build/bucket.o: src/bucket.cpp 
	g++ -O3 -c src/bucket.cpp -o build/bucket.o

build/exhausting.o: src/exhausting.cpp 
	g++ -O3 -c src/exhausting.cpp -o build/exhausting.o

build/lsh.o: src/lsh.cpp 
	g++ -O3 -c src/lsh.cpp -o build/lsh.o

build/kmeans.o: src/kmeans.cpp 
	g++ -O3 -c src/kmeans.cpp -o build/kmeans.o

build/cluster.o: src/cluster.cpp 
	g++ -O3 -c src/cluster.cpp -o build/cluster.o

clean:
	rm lsh cluster build/*.o