#include "../headers/exhausting.h"

//Search exhaustively for each query distance with each image of dataset so as to find best for LSH.
void ExhaustingNN(LSH* info)
{   
    for(int i=0;i<info->get_Num_of_Queries();i++)
    {
        auto start = chrono::high_resolution_clock::now(); 
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>> > pq; 

        for(int j=0;j<info->get_Num_of_Images();j++)
            pq.push(make_pair(ManhattanDistance(info->get_Queries_Array()[i],info->get_Images_Array()[j],info->get_dimensions()),j));
        
        auto end = chrono::high_resolution_clock::now(); 
        for(int k=0;k<info->get_N();k++)
        {
            info->get_True_Distances()[i][k] = pq.top().first;
            info->get_True_Neighbors()[i][k] = pq.top().second;
            pq.pop();
        }
        info->get_tTrue()[i] = chrono::duration_cast<chrono::microseconds>(end - start).count();  
    }

    // For new space...
    for(int i=0;i<info->get_New_Num_of_Queries();i++)
    {
        auto start = chrono::high_resolution_clock::now(); 
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>> > pq; 

        for(int j=0;j<info->get_New_Num_of_Images();j++)
            pq.push(make_pair(ManhattanDistance(info->get_New_Queries_Array()[i],info->get_New_Images_Array()[j],info->get_New_dimensions()),j));
        
        auto end = chrono::high_resolution_clock::now(); 
        for(int k=0;k<info->get_N();k++)
        {
            info->get_Reduced_Distances()[i][k] = pq.top().first;
            info->get_True_Reduced_Neighbors()[i][k] = pq.top().second;
            pq.pop();
        }
        info->get_tReduced()[i] = chrono::duration_cast<chrono::microseconds>(end - start).count();  
    }
}

//Function which returns manhattan distance of 2 vectors.
item ManhattanDistance(item* x,item* y,int dimensions)
{
    item sum=0;
    for(int i=0;i<dimensions;i++)   sum+=abs(x[i]-y[i]);    
    
    return sum;
}