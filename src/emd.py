from pulp import *
import math
import numpy as np
import struct
import sys
from array import array
from queue import PriorityQueue

#Function that loads the MNIST images 
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
    
    return images,rows,cols

#Function that loads the Labels of images
def Load_MNIST_Labels(labels_path):
    labels = []
    with open(labels_path, 'rb') as file:
        magic, size = struct.unpack(">II", file.read(8))
        labels = array("B", file.read()) 
    return labels

#Function that calculate the weights of an image and finds the centroids of each cluster 
#Weight of cluster: Sum of values of pixels' values of a cluster
def Calculate_Weights(data, size_of_cluster, rows, cols):
    dictionary = dict()
    weight = 0
    name_of_cluster = 0
    centroids = []
    adj = int(math.sqrt(size_of_cluster)/2)

    for row in range(0,rows,int(math.sqrt(size_of_cluster))):
        for col in range(0,cols,int(math.sqrt(size_of_cluster))):
            centroids.append((row+adj,col+adj))
            for row_cluster in range(row, row + int(math.sqrt(size_of_cluster))):
                for col_cluster in range(col, col + int(math.sqrt(size_of_cluster))):
                    weight += data[row_cluster][col_cluster]
            dictionary[str(name_of_cluster)] = weight
            name_of_cluster += 1
            weight = 0

    return dictionary, centroids

#Function that calculate the Manhattan distance
def ManhattanDistance(x,y,rows,cols):
    sum=0.0
    for i in range(rows):
        for j in range(cols):
            sum+=abs(x[i][j]-y[i][j])  
    
    return sum

def main(argv):
    if(len(sys.argv) != 12):
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

        if(str(sys.argv[5]) != '-l1'):
            print("Sorry, the input must be in this form: reduce.py  –d  <dataset>  -q  <queryset>  -od  <output_dataset_file>  -oq  <output_query_file>")
            exit(1)
        else:
            train_labels = str(sys.argv[6])

        if(str(sys.argv[7]) != '-l2'):
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

    # Load the images and their labes 
    train_images, rows, cols = Load_Mnist_Images(train_file)
    test_images, rows, cols = Load_Mnist_Images(test_file)

    labels_of_train = Load_MNIST_Labels(train_labels)
    labels_of_test = Load_MNIST_Labels(test_labels)

    # Rescale the data with the maximum pixel value of them
    train_images = train_images / np.max(train_images)
    train_images = train_images.reshape(train_images.shape[0], train_images.shape[1], train_images.shape[2])

    test_images = test_images / np.max(test_images)
    test_images = test_images.reshape(test_images.shape[0], test_images.shape[1], test_images.shape[2])

    # Rescale the data to be in space of [0, 25500]
    train_images = ((train_images-np.min(train_images))/(np.max(train_images)-np.min(train_images)))*255
    test_images = ((test_images-np.min(test_images))/(np.max(test_images)-np.min(test_images)))*255

    # Take a subset of data
    train_images = train_images[0:10]
    test_images = test_images[0:2]

    # size_of_cluster = 49 # 7x7
    size_of_cluster = 16 # 4x4
    num_of_clusters = (rows*cols)/size_of_cluster

    # print(type(test_images))
    # print(len(test_images))
    # print(test_images.shape)
    flag = True

    query_labels = []
    scores = []
    manhattan_scores = []
    total_predicted = []
    manhattan_total_predicted = []

    # Compare each query with all train images
    for i in range(test_images.shape[0]):
        demand, query_centroids = Calculate_Weights(test_images[i],size_of_cluster,rows,cols)
        # Find the 10th most closest neighboors
        neighboors = PriorityQueue()
        manhattan_neighboors = PriorityQueue()

        # print(query_centroids)

        # For each train image
        for j in range(train_images.shape[0]):
            supply, train_centroids = Calculate_Weights(train_images[j],size_of_cluster,rows,cols)

            # Calculate only one time the cost of tranposting one unit 
            # Cost of Transportation: Euclidian distance of coordinates of centroids of each cluster
            if flag:
                costs = []
                for a in query_centroids:
                    cost = []
                    for b in train_centroids:
                        dist = math.sqrt( (a[0]-b[0])**2 + (a[1]-b[1])**2 )
                        cost.append(dist)
                    costs.append(cost)

                flag = False
                costs = makeDict([supply.keys(), demand.keys()],costs,0)

            # Creates the prob variable to contain the problem data
            prob = LpProblem("EMD",LpMinimize)

            # Creates a list of tuples containing all the possible routes for transport
            routes = [(w,b) for w in supply.keys() for b in demand.keys()]

            # print(routes)

            # A dictionary called route_vars is created to contain the referenced variables (the routes)
            route_vars = LpVariable.dicts("Route",(supply.keys(),demand.keys()),0,None,LpInteger)

            # The objective function is added to prob first
            prob += lpSum([route_vars[w][b]*costs[w][b] for (w,b) in routes]), "Sum of Transporting Costs"

            # The supply maximum constraints are added to prob for each supply node (train cluster)
            for w in supply.keys():
                prob += lpSum([route_vars[w][b] for b in demand.keys()]) == supply[w], "Sum of Products out of Train Cluster %s"%w

            # The demand minimum constraints are added to prob for each demand node (Query cluster)
            for b in demand.keys():
                prob += lpSum([route_vars[w][b] for w in supply.keys()]) == demand[b], "Sum_of_Products_into_Query_Cluster%s"%b
            
            # The problem data is written to an .lp file
            prob.writeLP("EMD.lp")

            # The problem is solved using PuLP's choice of Solver
            prob.solve(PULP_CBC_CMD(msg=False))

            # # The status of the solution is printed to the screen
            # print("Status:", LpStatus[prob.status])

            # Each of the variables is printed with it's resolved optimum value
            # for v in prob.variables():
                # print(v.name, "=", v.varValue)

            # The optimised objective function value is printed to the screen    
            # print("Total Cost of Transportation = ", value(prob.objective))

            # Save the optimised objective function value and the manhattan distance
            neighboors.put((value(prob.objective), j))
            manhattan_neighboors.put((ManhattanDistance(test_images[i],train_images[j],rows,cols), j))

        query_label = labels_of_test[i]
        # print("query ", query_label)

        neigh = 0
        tr_labels = [] 
        manhattan_tr_labels = [] 

        # Take the 10th most closest neighboors from the Priority Queue
        while (neigh < 10):
            n = neighboors.get()
            index_label = n[1]
            manhattan_n = manhattan_neighboors.get()
            manhattan_index_label = manhattan_n[1]
            tr_labels.append(labels_of_train[index_label])
            manhattan_tr_labels.append(labels_of_train[manhattan_index_label])
            neigh += 1
        
        query_labels.append(query_label)

        # Find the rate of success of EMD
        correct_labels = sum(map(lambda x : x == query_label, tr_labels))
        success_rate = (correct_labels/10)*100
        scores.append(success_rate)
        total_predicted.append(tr_labels)
        
        # Find the rate of success of Manhattan Distance
        manhattan_correct_labels = sum(map(lambda x : x == query_label, manhattan_tr_labels))
        manhattan_success_rate = (manhattan_correct_labels/10)*100
        manhattan_scores.append(manhattan_success_rate)
        manhattan_total_predicted.append(manhattan_tr_labels)

    # Print the results
    for q in range(len(test_images)):
        print("Query label:",query_labels[q])  
        print("Predicted labels: ",total_predicted[q])
        print("Score: ",scores[q],"%")
        print("Manhattan Predicted labels: ",manhattan_total_predicted[q])
        print("Manhattan Score: ",manhattan_scores[q],"%\n")       
    print("Average Correct Search Results EMD: ", sum(scores)/len(scores),"%")
    print("Average Correct Search Results MANHATTAN: ", sum(manhattan_scores)/len(manhattan_scores), "%\n")


if __name__ == "__main__":
    main(sys.argv[0:])