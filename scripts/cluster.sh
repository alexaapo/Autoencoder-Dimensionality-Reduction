make cluster 
time ./cluster -d data/test/t10k-images-idx3-ubyte -i data/new/newt10k-images-idx3-ubyte -n data/clusterfile.txt -c data/cluster.conf -o output # Input: 10.000 images
# time ./cluster -d data/train/train-images-idx3-ubyte -i data/new/newtrain-images-idx3-ubyte -n data/clusterfile.txt -c data/cluster.conf -o output # Input: 60.000 images