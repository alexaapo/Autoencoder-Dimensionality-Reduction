make cluster 

# time ./cluster –d <input file original space> -i <input file new space> -n <classes from NN as clusters file> –c <configuration file> -o <output file>
time valgrind --leak-check=full ./cluster –d <input file original space> -i <input file new space> -n <classes from NN as clusters file> –c <configuration file> -o <output file>