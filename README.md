# Autoencoder dimensionality reduction, EMD-Manhattan metrics comparison and classifier based clustering on MNIST dataset

## Question A - Dimensionality Reduction:

  ### Execution:
         python src/reduce.py -d dataset -q queryset -od output_dataset_file -oq output_query_file

   For example:
         
         python src/reduce.py -d data/train/train-images-idx3-ubyte -q data/test/t10k-images-idx3-ubyte -od data/new/newtrain-images-idx3-ubyte -oq data/new/newt10k-images-idx3-ubyte
         
  ### Few things about this question:
  This programm build an image autocoding NN that includes bottleneck layers. More specifically, it converts a given dataset to a reduced version of it by passing it through a pretrained encoder model and taking the latent vector values produced by it. The final structure of this compression model will be:
      
![index](https://user-images.githubusercontent.com/60033683/110159038-d429d100-7df2-11eb-831c-12ddfd77e4d5.png)
