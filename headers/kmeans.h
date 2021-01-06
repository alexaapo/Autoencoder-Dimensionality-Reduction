#ifndef KMEANS_H
#define KMEANS_H

#include "../headers/exhausting.h"

//Forward declaration of Read_BF function...
void Read_BF(item ***, int *, int *, int *, string, int );

class kmeans
{
    private:
        int K,L,LSH_k,dimensions,number_of_images,cols,rows;
        item** Images_Array;
        int* centroids;
        double kmeans_time;

    public:
        kmeans(string input_file,string conf,bool original)
        :L(3),LSH_k(4)
        {
            if(original)    Read_BF(&Images_Array,&number_of_images,&cols,&rows,input_file,1);
            else    Read_BF2(&Images_Array,&number_of_images,&cols,&rows,input_file,1);
            dimensions = cols*rows;
            info_initialization(conf);
            centroids = new int[K];
            centroid_initialization();
        }

        ~kmeans()
        {
            delete [] centroids;
            //Deallocation of memory of Images_Array...
            for(int i=0;i<number_of_images;i++)    delete [] Images_Array[i];
                delete [] Images_Array;
        }

        void info_initialization(string );
        void centroid_initialization();

        int get_K();
        int get_L();
        int get_LSH_k();
        int get_dimensions();
        int get_rows();
        int get_cols();
        int get_number_of_images();
        double get_kmeans_time();
        int* get_centroids();
        item** get_Images_Array();
};

class Nearest_Centroids
{
    private:
        int nearest_centroid1,dist1;
        int nearest_centroid2,dist2;
    
    public:    
        Nearest_Centroids()
        {}
        
        void set_nearest_centroid1(int );
        void set_dist1(int );
        void set_nearest_centroid2(int );
        void set_dist2(int );

        int get_nearest_centroid1();
        int get_dist1();
        int get_nearest_centroid2();
        int get_dist2();           
};

void Silhouette(map <int,Nearest_Centroids*>* ,int,float**,kmeans* );

#endif