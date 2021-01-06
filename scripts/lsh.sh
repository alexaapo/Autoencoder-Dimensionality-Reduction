make lsh

time ./search -d data/train/train-images-idx3-ubyte -i data/temptrain-images-idx3-ubyte -q data/test/t10k-images-idx3-ubyte -s data/tempt10k-images-idx3-ubyte -k 4 -L 5 -o output
# time ./search -d data/train/train-images-idx3-ubyte -i data/new/newtrain-images-idx3-ubyte -q data/test/t10k-images-idx3-ubyte -s data/new/newt10k-images-idx3-ubyte -k 4 -L 5 -o output
# time valgrind --leak-check=full ./search -d data/train/train-images-idx3-ubyte -i data/new/newtrain-images-idx3-ubyte -q data/test/t10k-images-idx3-ubyte -s data/new/newt10k-images-idx3-ubyte -k 4 -L 5 -o output