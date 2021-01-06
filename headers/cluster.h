#ifndef CLUSTER_H
#define CLUSTER_H

#include "../headers/kmeans.h"

class Cluster
{
    private:
        string output_file,clusters_file;
        kmeans* kmeansptr;
        item** centroids;
        map <int,Nearest_Centroids*> points;
        fstream file;
        float epsilon=1e-3;
        bool original;
        
    public:

        Cluster(string input,string output_file_,string clusters_file_,string conf,bool original_)
        :output_file(output_file_),clusters_file(clusters_file_),original(original_)
        {   
            file.open(output_file,ios::app);

            //Allocate memory for kmeans pointer (helpful class kmeans).
            kmeansptr = new kmeans(input,conf,original);
            
            file << "Images:" << kmeansptr->get_number_of_images() << endl << "Dimensions:" << kmeansptr->get_rows() << "x" << kmeansptr->get_cols() << endl <<  "Κ:" << kmeansptr->get_K() << endl << "ε:" << epsilon << endl;

            //Allocate memory for centroids of each cluster (centroids_dimensions=K*image_dimensions).
            centroids = new item*[kmeansptr->get_K()];
            for(int i=0;i<kmeansptr->get_K();i++) centroids[i] = new item[kmeansptr->get_dimensions()];
            for(int i=0;i<kmeansptr->get_K();i++) 
                for(int z=0;z<kmeansptr->get_dimensions();z++) 
                    centroids[i][z]=0;
            
            //Allocate memory for each class Nearest_Centroids (each image has one pointer to an object of Nearest_Centroids).
            for(int i=0;i<kmeansptr->get_number_of_images();i++)
                points[i] = new Nearest_Centroids();
        }

        ~Cluster()
        {
            //Deallocate memory for each pointer to class Nearest_Centroids.
            for(int i=0;i<kmeansptr->get_number_of_images();i++)    delete points[i];
            
            //Deallocate memory for centroids array.
            for(int i=0;i<kmeansptr->get_K();i++) delete [] centroids[i];
            delete [] centroids;

            //Deallocate memory for kmeans pointer.
            delete kmeansptr;
        }
        
        void Clustering();
        void Lloyd_Assign();
        void FastUpdate();
        void SlowUpdate();
        float Objective_Value();
        void Print(float* ,int, float);
};

#endif