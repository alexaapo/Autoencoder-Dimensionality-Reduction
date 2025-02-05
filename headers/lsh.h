#ifndef LSH_H
#define LSH_H

#include "../headers/exhausting.h" 

//Forward declaration
class Bucket;

class LSH
{
    private:
        int Num_of_Images,Num_of_Queries,New_Num_of_Images,New_Num_of_Queries,k,L,R,dimensions,New_dimensions,W,N,m,M,HashTableSize;
        int* modulars;
        double *tLSH,*tTrue,*tReduced,dist_AF,red_dist_AF,time_error;
        int **s_i,**True_Distances,**Reduced_Distances,**True_Neighbors,**True_Reduced_Neighbors,**LSH_nns,**LSH_Distances;
        item **Images_Array,**Queries_Array,**New_Images_Array,**New_Queries_Array;
        Bucket*** Hash_Tables;
        string input_file_original_space,input_file_new_space,query_file_original_space,query_file_new_space,output_file;
        fstream file;
    
    public:
        LSH(string input_file_orig,string input_file_new,string query_file_orig,string query_file_new,string output_file_,int L_,int k_)
        :input_file_original_space(input_file_orig),input_file_new_space(input_file_new),query_file_original_space(query_file_orig),query_file_new_space(query_file_new),output_file(output_file_),L(L_),N(1),k(k_),dist_AF(0.0),red_dist_AF(0.0),time_error(0.0)
        {}

        ~LSH()
        {
            Deallocation_of_Memory();
        }

        void Approximate_LSH();
        void Approximate_Range_Search(int);
        void InitLSH();
        void Deallocation_of_Memory();
        
        int get_dimensions();
        int get_New_dimensions();
        int get_M();
        int get_k();
        int get_L();
        int get_N();
        int get_W();
        int get_HashTableSize();
        int get_Num_of_Images();
        int get_New_Num_of_Images();
        int get_Num_of_Queries();
        int get_New_Num_of_Queries();
        int* get_modulars();
        int** get_s_i();
        double* get_tTrue();
        double* get_tReduced();
        int** get_True_Distances();
        int** get_True_Neighbors();
        int** get_Reduced_Distances();
        int** get_True_Reduced_Neighbors();
        item** get_Images_Array();
        item** get_New_Images_Array();
        item** get_Queries_Array();
        item** get_New_Queries_Array();
        Bucket*** get_Hash_Tables();
};

#endif