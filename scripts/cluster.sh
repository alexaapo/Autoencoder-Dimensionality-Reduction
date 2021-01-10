make cluster 

# time ./cluster -d data/train/train-images-idx3-ubyte -i data/new/newtrain-images-idx3-ubyte -n data/clusterfile.txt -c data/cluster.conf -o output
time ./cluster -d data/test/t10k-images-idx3-ubyte -i data/new/newt10k-images-idx3-ubyte -n data/clusterfile.txt -c data/cluster.conf -o output
# time valgrind --leak-check=full ./cluster –d <input file original space> -i <input file new space> -n <classes from NN as clusters file> –c <configuration file> -o <output file>