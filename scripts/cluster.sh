make cluster 

# time ./cluster -i data/train-images-idx3-ubyte -c data/cluster.conf -o output -complete no -m Classic
# time valgrind --leak-check=full ./cluster -i data/train-images-idx3-ubyte -c data/cluster.conf -o output -complete no -m Classic

time ./cluster -i data/train-images-idx3-ubyte -c data/cluster.conf -o output -complete no -m LSH
# time valgrind --leak-check=full ./cluster -i data/train-images-idx3-ubyte -c data/cluster.conf -o output -complete no -m LSH

# time ./cluster -i data/train-images-idx3-ubyte -c data/cluster.conf -o output -complete no -m Hypercube
# time valgrind --leak-check=full ./cluster -i data/train-images-idx3-ubyte -c data/cluster.conf -o output -complete no -m Hypercube