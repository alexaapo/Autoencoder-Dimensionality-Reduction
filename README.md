# Autoencoder dimensionality reduction, EMD-Manhattan metrics comparison and classifier based clustering on MNIST dataset </br>


## Question A - Dimensionality Reduction:

  ### Execution:
         python src/reduce.py -d dataset -q queryset -od output_dataset_file -oq output_query_file

   For example:
   
         python src/reduce.py -d data/train/train-images-idx3-ubyte -q data/test/t10k-images-idx3-ubyte -od data/new/newtrain-images-idx3-ubyte -oq data/new/newt10k-images-idx3-ubyte
         
  ### Few things about this query:
  This programm build an image autocoding NN that includes bottleneck layers. More specifically, it converts a given dataset to a reduced version of it by passing it through a pretrained encoder model and taking the latent vector values produced by it. The final structure of this compression model will be:
      
![index](https://user-images.githubusercontent.com/60033683/110159038-d429d100-7df2-11eb-831c-12ddfd77e4d5.png) </br></br></br>

## Question B - LSH ANN in original and reduced space
  
   ### Execution:
          ./scripts/lsh.sh
   ### Few things about this query:
   This program accepts as input a dataset and a queryset both in the original and the reduced dimension versions of them (the output of Question 1). Then makes comparisons between:
   * LSH ANN on original space
   * LSH ANN on reduced space
   * Brute Force NN on original space

All searches are done with Manhattan metric. The results are compared in terms of search time and approximation factors at the original space, i.e. the average Manhattan approximate distance. </br></br></br>

## Question 3 - Comparison of Earth Mover's Distance & Manhattan Distance metrics

   ### Execution:
          python src/emd.py -d input_file_original_space -q query_file_original_space -l1 labels_of_input_dataset -l2 labels_of_query_dataset -o output_file -EMD
          
   For example:
          
          python src/emd.py -d data/train/train-images-idx3-ubyte -q data/test/t10k-images-idx3-ubyte -l1 data/train/train-labels-idx1-ubyte -l2 data/test/t10k-labels-idx1-ubyte -o output -EMD
          
   ### Few things about this query:
   This programm implement the EMD metric that solves a Linear Programming Problem and compare it with the Manhattan Distance. We used the library of [PuLP](https://pypi.org/project/PuLP/), which allowed us to define an LpProblem with all the clusters of a train and test image. 
    
   Also for each image, we calculated the weights and costs of transporting a supply as well as all the different routes that can exist between sub-clusters. After defining all this, we added some constraints, stored the optimized value returned by the objective function in a priority queue for each comparison of a query image with a train image, as we could have the 10 nearest neighbors from each query. At the same time, each time we saved the distances and neighbors we found with Manhattan Distance, in order to compare the success rates. The report include all the results of our experiments and the explanation of them. </br></br></br>
  
## Question 4 - Comparison of Centroid-based clustering on original & reduced space and clustering based in classifier results

   ### Execution:
          ./scripts/cluster.sh
          
  In this script you can comment / un-comment the 2nd and 3rd line respectively, so as to give as input the train dataset, with 60,000 images or the test dataset, with 10,000 images. In most cases we tried the small dataset for time saving. 
  
   ### Few things about this query:
  This programm implemet three different clustering approaches. The Clustering algorithms used k-medians. We use the Clustering procedure on both the original dataset and its reduced version. Finaly, we used the deliverable of the 2nd Project for categorization of the images of the input set and clustering based on it. Each time, we compute the silhouette values, for the final comparison.
  

***Note:*** Regarding questions 2 and 4, there was special treatment of reading the file produced by question A, as now each photo was represented by 10 numbers of size 2 bytes. For this reason a separate [Read_BF2](https://github.com/alexaapo/Autoencoder-Dimensionality-Reduction/blob/73dc367cbbb59331bbd0286c11dcd5b93338b8a3/src/read_binary_file.cpp#L64) function was created which read with the appropriate endianess 2 bytes serially. Finally, all results are temporarily printed to the external output file. 
