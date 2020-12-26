#include "../headers/exhausting.h"

void Nearest_Centroids::set_nearest_centroid1(int a)
{
    nearest_centroid1=a;
}

void Nearest_Centroids::set_dist1(int a)
{
    dist1=a;
}

void Nearest_Centroids::set_nearest_centroid2(int a)
{
    nearest_centroid2=a;
}

void Nearest_Centroids::set_dist2(int a)
{
    dist2=a;
}

int Nearest_Centroids::get_nearest_centroid1()
{
    return nearest_centroid1;
}

int Nearest_Centroids::get_dist1()
{
    return dist1;
}

int Nearest_Centroids::get_nearest_centroid2()
{
    return nearest_centroid2;
}

int Nearest_Centroids::get_dist2()
{
    return dist2;
}

item** kmeans::get_Images_Array()
{
    return Images_Array;
}

int* kmeans::get_centroids()
{
    return centroids;
}

int kmeans::get_K()
{
    return K;
}

int kmeans::get_L()
{
    return L;
}

int kmeans::get_LSH_k()
{
    return LSH_k;
}

int kmeans::get_HC_k()
{
    return HC_k;
}

int kmeans::get_HC_M()
{
    return HC_M;
}

int kmeans::get_probes()
{
    return probes;
}

int kmeans::get_dimensions()
{
    return dimensions;
}

int kmeans::get_number_of_images()
{
    return number_of_images;
}

double kmeans::get_kmeans_time()
{
    return kmeans_time;
}

//Function which reads cluster.conf and stores values.
void kmeans::info_initialization(string configuration_file)
{
    ifstream file(configuration_file);

    if(file.is_open())
    {
        string line, word;
        while(getline(file, line))
        {
            istringstream iss(line);

            while (iss >> word) 
            { 
                if(word == "number_of_clusters:")
                {
                    iss >> word;
                    K = stoi(word);
                }
                else if(word == "number_of_vector_hash_tables:")
                {
                    iss >> word;
                    L = stoi(word);
                }
                else if(word == "number_of_vector_hash_functions:")
                {
                    iss >> word;
                    LSH_k = stoi(word);
                }
                else if(word == "max_number_M_hypercube:")
                {
                    iss >> word;
                    HC_M = stoi(word);
                }
                else if(word == "number_of_hypercube_dimensions:")
                {
                    iss >> word;
                    HC_k = stoi(word);
                }
                else if(word == "number_of_probes:")
                {
                    iss >> word;
                    probes = stoi(word);
                }
            } 
        }
        
    }
    else cout << endl << "Problem" << endl;
}

void kmeans::centroid_initialization()
{
    auto start = chrono::high_resolution_clock::now();

    //Pick uniformly a random first centroid...
    default_random_engine generator1;   
    uniform_int_distribution<int> distribution1(0,number_of_images-1);
    centroids[0] = distribution1(generator1);
    
    //Initialize random generator for our float distribution function...
    default_random_engine generator;  
    
    //For each centroid except first (which we choosed randomly)...
    for(int centroid=0;centroid<K-1;centroid++)
    {
        int min_distance;
        int D_i[number_of_images];
        float P_r[number_of_images];
        for(int i=0;i<number_of_images;i++)
        {
            D_i[i]=0;
            P_r[i]=0;
        }
        priority_queue<int> distances;

        //Calculate minimum distance from distances of each image to a specific centroid and store it to D[i]...
        for(int i=0;i<number_of_images;i++)
        {
            min_distance=pow(2,25);
            for(int j=0;j<=centroid;j++)
            {
                int distance = ManhattanDistance(Images_Array[i], Images_Array[centroids[j]],dimensions);
                if(distance < min_distance)  min_distance = distance;
            }
            D_i[i] = min_distance;
            distances.push(min_distance);
        }
        //Store max_Di...
        int max_Di = distances.top();

        //Calculate P_r[0] cause we'll need it to calculate P_r[1]...
        P_r[0] = pow(((float)(D_i[0])/(float)max_Di),2);

        //Calculate each Pr[r]...
        for(int r=1;r<number_of_images;r++)
            P_r[r] = P_r[r-1] + pow(((float)(D_i[r])/(float)max_Di),2);
    
        //Pick random real number in range (0,Pr[number_of_images-1])
        uniform_real_distribution<float> distribution(float(0),P_r[number_of_images-1]);
        float x = distribution(generator);

        //Find new index of new centroid...
        for(int r=0;r<number_of_images;r++)
        {
            if(P_r[r] >= x)  
            {
                centroids[centroid+1] = r;
                break;
            }
        }
    }
    auto end = chrono::high_resolution_clock::now(); 
    kmeans_time = chrono::duration_cast<chrono::seconds>(end - start).count();
}

void Silhouette(map <int,Nearest_Centroids*>* map_ptr,int K,float** silhouette_array,kmeans* kmeansptr)
{
    //Declaration of important structures,variables...
    int cluster1=0,cluster2=0,images_in_cluster1=0,images_in_cluster2=0;
    float ai=0.0,bi=0.0,average_silhouette = 0.0;
    int images_in_cluster[K];
    item* current_image = NULL;
    
    //Initialize arrays with zeros...
    for(int i=0;i<K;i++)  (*silhouette_array)[i] = 0.0;
    for(int i=0;i<K;i++)  images_in_cluster[i] = 0;

    map <int,Nearest_Centroids*>::iterator it;
    //Iterate whole map so as to calculate silhouette for points of each cluster...
    for(it=map_ptr->begin();it!=map_ptr->end();it++)    
    {
        images_in_cluster1 = 0;
        images_in_cluster2 = 0;
        cluster1 = it->second->get_nearest_centroid1();
        cluster2 = it->second->get_nearest_centroid2();
        current_image = kmeansptr->get_Images_Array()[it->first];

        map <int,Nearest_Centroids*>::iterator it1;
        for(it1=map_ptr->begin();it1!=map_ptr->end();it1++)    
        {
            if(it1->second->get_nearest_centroid1()==cluster1)
            {
                ai += (float)(ManhattanDistance(kmeansptr->get_Images_Array()[it1->first],current_image,kmeansptr->get_dimensions()));
                images_in_cluster1++;
            }   
            if(it1->second->get_nearest_centroid1()==cluster2)
            {
                bi += (float)(ManhattanDistance(kmeansptr->get_Images_Array()[it1->first],current_image,kmeansptr->get_dimensions()));
                images_in_cluster2++;
            }
        }
        
        ai/=(float)images_in_cluster1;
        bi/=(float)images_in_cluster2;

        if(ai < bi)     
            (*silhouette_array)[cluster1] += (1-((float)ai/(float)bi));
        else if(ai > bi)    
            (*silhouette_array)[cluster1] += (((float)bi/(float)ai)-1);
        
        images_in_cluster[cluster1] += 1;
    }
    
    //Calculate mean si of each cluster..
    for(int i=0;i<K;i++) 
    {
        (*silhouette_array)[i]/=(float)images_in_cluster[i];
        average_silhouette+=(*silhouette_array)[i];
    }

    //Store average silhouette value in last pos of array...
    (*silhouette_array)[K] = average_silhouette/(float)K;
}