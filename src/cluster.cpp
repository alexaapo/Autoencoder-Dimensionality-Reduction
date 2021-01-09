#include "../headers/cluster.h"

void Cluster::Clustering()
{
    //Important variables..
    float ratio;
    double clustering_time=0.0;

    //Original array of kmeans centroids...
    int* indexes = kmeansptr->get_centroids(),iters=0;

    //Store previous and current objective value in this array..
    float objectives_values[2];
    objectives_values[0]=100;
    objectives_values[1]=100;

    //Allocate memory for array with silhouette values...
    float* silhouette_array = new float[kmeansptr->get_K()+1];    
    for(int k=0;k<=kmeansptr->get_K();k++)  silhouette_array[k]=0.0;

    //Store centroids with results of kmeans++ initialization...
    for(int i=0;i<kmeansptr->get_K();i++)
    {
        for(int j=0;j<kmeansptr->get_dimensions();j++)
            centroids[i][j] = kmeansptr->get_Images_Array()[indexes[i]][j];
    }

    auto start = chrono::high_resolution_clock::now();
    while(true)
    {
        //Assign with the Lloyd method...
        Lloyd_Assign();

        //Calculate objective value and if ratio is less than epsilon then stop clustering
        objectives_values[1] = Objective_Value();
        ratio = abs(objectives_values[1]-objectives_values[0])/objectives_values[0];
        objectives_values[0] = objectives_values[1];
        // file << "Reduction's rate change of objective function's value:" << ratio << endl << "Cost:" << objectives_values[1] << endl << endl;
        if(ratio<epsilon || iters==20)   break;
        
        //Update centroids with kmedians...
        SlowUpdate();
        // FastUpdate();

        iters++;
    }
    auto end = chrono::high_resolution_clock::now(); 
    clustering_time = chrono::duration_cast<chrono::seconds>(end - start).count();
    file << endl << "Converged with " << iters << " updates!" << endl;

    //Calculate Silhouette values.
    Silhouette(&points,kmeansptr->get_K(),&silhouette_array,kmeansptr);
    
    Print(silhouette_array,(clustering_time+kmeansptr->get_kmeans_time()),objectives_values[1]);

    delete [] silhouette_array;
}

void Cluster::Lloyd_Assign()
{
    int dist1,n1,dist2,n2;

    //For each point of dataset keep 2 nearest centroids and the appropriate distances in our map...
    for(int i=0;i<kmeansptr->get_number_of_images();i++)
    {
        //Store pairs of distance to each centroid from a point of dataset and index of this centroid...
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>> > distances; 
        
        for(int j=0;j<kmeansptr->get_K();j++)
            distances.push(make_pair(ManhattanDistance(kmeansptr->get_Images_Array()[i],centroids[j],kmeansptr->get_dimensions()),j));
        
        //Store distance and index of first nearest centroid...
        dist1 = distances.top().first;
        n1 = distances.top().second;
        
        //Pop up first pair...
        distances.pop();
        
        //Store distance and index of second nearest centroid...
        dist2 = distances.top().first;
        n2 = distances.top().second;

        //Pass these values to class Nearest_Centroids of each point of dataset...
        points[i]->set_dist1(dist1);
        points[i]->set_nearest_centroid1(n1);
        points[i]->set_dist2(dist2);
        points[i]->set_nearest_centroid2(n2);
    }
}

void Cluster::FastUpdate()
{    
    map <int,Nearest_Centroids*>::iterator it;
    int cluster=0,median_index=0;

    //Initialize K*dimensions vectors...
    vector<item>** vectors = new vector<item>*[kmeansptr->get_K()];
    for(int i=0;i<kmeansptr->get_K();i++)   vectors[i] = new vector<item>[kmeansptr->get_dimensions()]; 

    //Fill vectors with features of each image of dataset...
    for(it=points.begin();it!=points.end();it++)    
    {
        cluster = it->second->get_nearest_centroid1();
        for(int z=0;z<kmeansptr->get_dimensions();z++)
            vectors[cluster][z].push_back(kmeansptr->get_Images_Array()[it->first][z]);
    }

    //Sort each vector and choose the appropriate feature (with median index) 
    //in order to have a new one centroid.
    for(int i=0;i<kmeansptr->get_K();i++)   
    {
        for(int z=0;z<kmeansptr->get_dimensions();z++)
        {
            if(vectors[i][z].size()!=0)
            {
                sort(vectors[i][z].begin(),vectors[i][z].end());
                median_index = vectors[i][z].size()/2; 
                centroids[i][z] = vectors[i][z][median_index];
            }
        }
    }
    
    //Deallocate memory for vectors...
    for(int i=0;i<kmeansptr->get_K();i++)   delete [] vectors[i];
    delete [] vectors;   
}

void Cluster::SlowUpdate()
{
    map <int,Nearest_Centroids*>::iterator it;
    int cluster=0,median_index=0;

    vector <item> vec;

    for(int i=0;i<kmeansptr->get_K();i++)   
    {
        for(int z=0;z<kmeansptr->get_dimensions();z++)
        {
            vec.clear();

            for(it=points.begin();it!=points.end();it++)    
            {
                cluster = it->second->get_nearest_centroid1();
                if(cluster==i)
                    vec.push_back(kmeansptr->get_Images_Array()[it->first][z]);
            }

            if(vec.size()!=0)
            {
                sort(vec.begin(),vec.end());
                median_index = vec.size()/2; 
                centroids[i][z] = vec[median_index];
            }
        }
    } 
}

float Cluster::Objective_Value()
{
    float avg_sum=0.0;
    int K=kmeansptr->get_K(),cluster=0,sums[K];
    for(int i=0;i<kmeansptr->get_K();i++)   sums[i]=0;

    map <int,Nearest_Centroids*>::iterator it;

    for(it=points.begin();it!=points.end();it++)    
    {
        cluster = it->second->get_nearest_centroid1();
        sums[cluster] += ManhattanDistance(kmeansptr->get_Images_Array()[it->first],centroids[cluster],kmeansptr->get_dimensions());
    }

    for(int i=0;i<K;i++)    avg_sum+=sums[i];

    return avg_sum;
}

void Cluster::Print(float* silhouette_array,int time,float last_obj_value)
{
    //Declaration of important structures,variables...
    int cluster=0,K=kmeansptr->get_K();
    map <int,Nearest_Centroids*>::iterator it;
    int images_in_cluster[K];
    for(int i=0;i<kmeansptr->get_K();i++)   images_in_cluster[i]=0;

    //Iterate whole map so as to calculate silhouette for points of each cluster...
    for(it=points.begin();it!=points.end();it++)    
    {
        cluster = it->second->get_nearest_centroid1();
        images_in_cluster[cluster]++;
    }

    if(original)
        file << endl << "ORIGINAL SPACE" << endl;
    else
        file << endl << "NEW SPACE" << endl;
    
    for(int i=0;i<K;i++)   
    {
        file << "CLUSTER-" << i << "  {" << images_in_cluster[i] << ", [ ";
        for(int z=0;z<kmeansptr->get_dimensions();z++)  file << centroids[i][z] << " "; 
        file << "]}" << endl << endl;
    }
    
    file << "clustering_time: " << time << "s" << endl << endl << "Silhouette: [ ";

    for(int i=0;i<=K;i++)   
    {
        file << fixed << setprecision(3) << silhouette_array[i];
        if(i<K) file << ", ";
        else    file << " ]";
    }
    
    file << endl << "Value of Objective Function: " << last_obj_value << endl << endl;
}

void Cluster::classes_as_clusters()
{
    map<int, int> new_points; 
    int label = 0,counter = 0;
    // Read clusters file...
    string line;
    ifstream myfile(clusters_file);
    if (myfile.is_open())
    {
        while(getline(myfile,line))
        {
            cout <<"READING LINE " << label << endl;
            
            istringstream ss(line);        
            string word; 
            counter = 0;
            while (ss >> word) 
            {
                stringstream to_number(word); 
                int image = 0;
                to_number >> image;
                // cout << image << endl;
                if(counter>3)   new_points[image] = label;
            }
            label++;
        }
        myfile.close();
    }
    
    map<int, int>::iterator it;

    for(it=new_points.begin();it!=new_points.end(); it++)
    {
        cout << it->first << " " << it->second << endl;
    }
    
    return;
    // Assign images to predicted clusters...
    int dist1,n1,dist2,n2;

    //For each point of dataset keep 2 nearest centroids and the appropriate distances in our map...
    for(int i=0;i<kmeansptr->get_number_of_images();i++)
    {
        //Store pairs of distance to each centroid from a point of dataset and index of this centroid...
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>> > distances; 
        
        for(int j=0;j<kmeansptr->get_K();j++)
            distances.push(make_pair(ManhattanDistance(kmeansptr->get_Images_Array()[i],centroids[j],kmeansptr->get_dimensions()),j));
        
        //Store distance and index of first nearest centroid...
        dist1 = distances.top().first;
        n1 = distances.top().second;
        
        //Pop up first pair...
        distances.pop();
        
        //Store distance and index of second nearest centroid...
        dist2 = distances.top().first;
        n2 = distances.top().second;

        //Pass these values to class Nearest_Centroids of each point of dataset...
        points[i]->set_dist1(dist1);
        points[i]->set_nearest_centroid1(n1);
        points[i]->set_dist2(dist2);
        points[i]->set_nearest_centroid2(n2);
    }

    /////////// Lets find median centroid fo each cluster...////////////////
    map <int,Nearest_Centroids*>::iterator it;
    int cluster=0,median_index=0;

    vector <item> vec;

    for(int i=0;i<kmeansptr->get_K();i++)   
    {
        for(int z=0;z<kmeansptr->get_dimensions();z++)
        {
            vec.clear();

            for(it=points.begin();it!=points.end();it++)    
            {
                cluster = it->second->get_nearest_centroid1();
                if(cluster==i)
                    vec.push_back(kmeansptr->get_Images_Array()[it->first][z]);
            }

            if(vec.size()!=0)
            {
                sort(vec.begin(),vec.end());
                median_index = vec.size()/2; 
                centroids[i][z] = vec[median_index];
            }
        }
    } 

    ////////////// Silhouette ////////////////
    // //Declaration of important structures,variables...
    // int cluster1=0,cluster2=0,images_in_cluster1=0,images_in_cluster2=0;
    // float ai=0.0,bi=0.0,average_silhouette = 0.0;
    // int images_in_cluster[K];
    // item* current_image = NULL;
    
    // //Initialize arrays with zeros...
    // for(int i=0;i<K;i++)  (*silhouette_array)[i] = 0.0;
    // for(int i=0;i<K;i++)  images_in_cluster[i] = 0;

    // map <int,Nearest_Centroids*>::iterator it;
    // //Iterate whole map so as to calculate silhouette for points of each cluster...
    // for(it=map_ptr->begin();it!=map_ptr->end();it++)    
    // {
    //     images_in_cluster1 = 0;
    //     images_in_cluster2 = 0;
    //     cluster1 = it->second->get_nearest_centroid1();
    //     cluster2 = it->second->get_nearest_centroid2();
    //     current_image = kmeansptr->get_Images_Array()[it->first];

    //     map <int,Nearest_Centroids*>::iterator it1;
    //     for(it1=map_ptr->begin();it1!=map_ptr->end();it1++)    
    //     {
    //         if(it1->second->get_nearest_centroid1()==cluster1)
    //         {
    //             ai += (float)(ManhattanDistance(kmeansptr->get_Images_Array()[it1->first],current_image,kmeansptr->get_dimensions()));
    //             images_in_cluster1++;
    //         }   
    //         if(it1->second->get_nearest_centroid1()==cluster2)
    //         {
    //             bi += (float)(ManhattanDistance(kmeansptr->get_Images_Array()[it1->first],current_image,kmeansptr->get_dimensions()));
    //             images_in_cluster2++;
    //         }
    //     }
        
    //     ai/=(float)images_in_cluster1;
    //     bi/=(float)images_in_cluster2;

    //     if(ai < bi)     
    //         (*silhouette_array)[cluster1] += (1-((float)ai/(float)bi));
    //     else if(ai > bi)    
    //         (*silhouette_array)[cluster1] += (((float)bi/(float)ai)-1);
        
    //     images_in_cluster[cluster1] += 1;
    // }
    
    // //Calculate mean si of each cluster..
    // for(int i=0;i<K;i++) 
    // {
    //     (*silhouette_array)[i]/=(float)images_in_cluster[i];
    //     average_silhouette+=(*silhouette_array)[i];
    // }

    // //Store average silhouette value in last pos of array...
    // (*silhouette_array)[K] = average_silhouette/(float)K;

    //////////// Objective Value /////////////////
    // float avg_sum=0.0;
    // int K=kmeansptr->get_K(),cluster=0,sums[K];
    // for(int i=0;i<kmeansptr->get_K();i++)   sums[i]=0;

    // map <int,Nearest_Centroids*>::iterator it;

    // for(it=points.begin();it!=points.end();it++)    
    // {
    //     cluster = it->second->get_nearest_centroid1();
    //     sums[cluster] += ManhattanDistance(kmeansptr->get_Images_Array()[it->first],centroids[cluster],kmeansptr->get_dimensions());
    // }

    // for(int i=0;i<K;i++)    avg_sum+=sums[i];
}