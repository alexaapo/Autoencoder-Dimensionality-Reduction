make lsh

time ./lsh -d data/train-images-idx3-ubyte -q data/t10k-images-idx3-ubyte -o output -k 4 -L 5 -N 1 -R 10000
# time valgrind --leak-check=full ./lsh -d data/train-images-idx3-ubyte -q data/t10k-images-idx3-ubyte -o output -k 4 -L 5 -N 1 -R 10000