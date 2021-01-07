from pulp import * 
from math import *
import numpy as np

def Load_Mnist_Images(train_images_path):
    with open(train_images_path, 'rb') as file:
        magic_num, num_of_images, rows, cols = struct.unpack(">IIII", file.read(16))
        image_data = array("B", file.read())        
    images = []
    for i in range(num_of_images):
        images.append([0] * rows * cols)
    for i in range(num_of_images):
        img = np.array(image_data[i * rows * cols:(i + 1) * rows * cols])
        img = img.reshape(28, 28)
        images[i][:] = img            
    print("Number of images: ",num_of_images)
    print("Rows of images: ",rows)
    print("Cols of images: ",cols)
    return (images,rows,cols)

def Calculate_Weights(data, size_of_cluster, rows, cols):
    dictionary = dict()
    weight = 0
    name_of_cluster = 0
    centroids = []

    for row in range(0,rows,math.sqrt(size_of_cluster)):
        for col in range(0,cols,math.sqrt(size_of_cluster)):
            for row_cluster in range(row, row + math.sqrt(size_of_cluster)):
                for col_cluster in range(col, col + math.sqrt(size_of_cluster)):
                    weight += data[row_cluster][col_cluster]
            dictionary[str(name_of_cluster)] = weight
            name_of_cluster += 1
            weight = 0

    return (dictionary, centroids)

def main(argv):
    if(len(sys.argv) != 9):
        print("Sorry, the input must be in this form: reduce.py  –d  <dataset>  -q  <queryset>  -od  <output_dataset_file>  -oq  <output_query_file>>")
        exit(1)
    else:
        if(str(sys.argv[1]) != '-d'):
            print("Sorry, the input must be in this form: reduce.py  –d  <dataset>  -q  <queryset>  -od  <output_dataset_file>  -oq  <output_query_file>")
            exit(1)
        else:
            train_file = str(sys.argv[2])

        if(str(sys.argv[3]) != '-q'):
            print("Sorry, the input must be in this form: reduce.py  –d  <dataset>  -q  <queryset>  -od  <output_dataset_file>  -oq  <output_query_file>")
            exit(1)
        else:
            test_file = str(sys.argv[4])

        if(str(sys.argv[5]) != '-od'):
            print("Sorry, the input must be in this form: reduce.py  –d  <dataset>  -q  <queryset>  -od  <output_dataset_file>  -oq  <output_query_file>")
            exit(1)
        else:
            train_labels = str(sys.argv[6])

        if(str(sys.argv[7]) != '-oq'):
            print("Sorry, the input must be in this form: reduce.py  –d  <dataset>  -q  <queryset>  -od  <output_dataset_file>  -oq  <output_query_file>")
            exit(1)
        else:
            test_labels = str(sys.argv[8])

        if(str(sys.argv[9]) != '-o'):
            print("Sorry, the input must be in this form: reduce.py  –d  <dataset>  -q  <queryset>  -od  <output_dataset_file>  -oq  <output_query_file>")
            exit(1)
        else:
            output_file = str(sys.argv[10])

        if(str(sys.argv[11]) != '-EMD'):
            print("Sorry, the input must be in this form: reduce.py  –d  <dataset>  -q  <queryset>  -od  <output_dataset_file>  -oq  <output_query_file>")
            exit(1)

    train_images, rows, cols = Load_Mnist_Images(train_file)
    test_images, rows, cols = Load_Mnist_Images(test_file)

    size_of_cluster = 49
    num_of_clusters = (rows*cols)/size_of_cluster

    for i in range(test_images.shape[0]):
        demand, query_centroids = Calculate_Weights(test_images[i],size_of_cluster,rows,cols)

        for j in range(train_images.shape[0]):
            supply, train_centroids = Calculate_Weights(train_images[j],size_of_cluster,rows,cols)

            costs = [[]]
            for a in query_centroids:
                cost = []
                for b in train_centroids:
                    dist = np.linalg.norm(a-b)
                    cost.append(dist)
                costs.append(cost)
            


        

if __name__ == "__main__":
    main(sys.argv[0:])