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
        
    public:

        Cluster(string input_file_original_space,string input_file_new_space,string output_file_,string clusters_file_,string conf)
        :output_file(output_file_),clusters_file(clusters_file_)
        {   
            file.open(output_file,ios::out);

            //Allocate memory for kmeans pointer (helpful class kmeans).
            kmeansptr = new kmeans(input_file_original_space,conf);
            
            file << "Images:" << kmeansptr->get_number_of_images() << endl << "Dimensions:" << sqrt(kmeansptr->get_dimensions()) << "x" << sqrt(kmeansptr->get_dimensions()) << endl <<  "Κ:" << kmeansptr->get_K() << endl << "ε:" << epsilon << endl;

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
        void RA_LSH_Assign();
        void RA_HC_Assign();
        void FastUpdate();
        void SlowUpdate();
        float Objective_Value();
        void Print(float* ,int);
};

#endif