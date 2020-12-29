#include "../headers/exhausting.h"

item ReverseInt(item i)
{
    unsigned char ch1, ch2, ch3, ch4;
    ch1=i&255;
    ch2=(i>>8)&255;
    ch3=(i>>16)&255;
    ch4=(i>>24)&255;
    return((item)ch1<<24)+((item)ch2<<16)+((item)ch3<<8)+ch4;
}

void Read_BF(item*** Array,int* number_of_images, int* n_cols, int* n_rows, string input_file,int a)
{
    ifstream file(input_file,ios::binary);
    
    if(file.is_open())
    {
        int magic_number=0,num_of_images=0,rows=0,cols=0;

        //Read magic number...
        file.read((char*)&magic_number,sizeof(magic_number));
        magic_number = ReverseInt(magic_number);

        //Read number of images...
        file.read((char*)&num_of_images,sizeof(num_of_images));
        num_of_images = ReverseInt(num_of_images);
        num_of_images/=a;

        //Read rows...
        file.read((char*)&rows,sizeof(rows));
        rows = ReverseInt(rows);
        
        //Read columns...
        file.read((char*)&cols,sizeof(cols));
        cols = ReverseInt(cols);

        //Allocation of memory for Images_Array...
        (*Array) = new item*[num_of_images];
        
        //Store important values (so as to pass them to main.cpp)
        *number_of_images = num_of_images;
        *n_rows = rows;
        *n_cols = cols;

        for(int i=0;i<num_of_images;i++)
        {
            (*Array)[i] = new item[(rows*cols)+1];

            for(int z=0;z<rows*cols;z++)
            {
                //Read each integer of binary file and store him into our array.
                unsigned char temp=0;
                file.read((char*)&temp,sizeof(temp));
                (*Array)[i][z] = (item)temp;
            }
            
            //In last position store index...
            (*Array)[i][rows*cols] = (item)i;
        }
    }
}

void Read_BF2(item*** Array,int* number_of_images, int* n_cols, int* n_rows, string input_file,int a)
{
    ifstream file(input_file,ios::binary);
    
    if(file.is_open())
    {
        int magic_number=0,num_of_images=0,rows=0,cols=0;

        //Read magic number...
        file.read((char*)&magic_number,sizeof(magic_number));
        magic_number = ReverseInt(magic_number);

        //Read number of images...
        file.read((char*)&num_of_images,sizeof(num_of_images));
        num_of_images = ReverseInt(num_of_images);
        num_of_images/=a;

        //Read rows...
        file.read((char*)&rows,sizeof(rows));
        rows = ReverseInt(rows);
        
        //Read columns...
        file.read((char*)&cols,sizeof(cols));
        cols = ReverseInt(cols);

        //Allocation of memory for Images_Array...
        (*Array) = new item*[num_of_images];
        
        //Store important values (so as to pass them to main.cpp)
        *number_of_images = num_of_images;
        *n_rows = rows;
        *n_cols = cols;

        for(int i=0;i<num_of_images;i++)
        {
            (*Array)[i] = new item[(rows*cols)+1];

            for(int z=0;z<rows*cols;z++)
            {
                //Read each integer of binary file and store him into our array.
                unsigned short int temp=0,mask=0;
                file.read((char*)&temp,sizeof(temp));
                mask=temp;
                // mask = temp = T-H
                temp = temp << 8;  
                //temp = H-0
                mask = mask >> 8;
                //mask = 0-T
                temp = temp | mask;      
                //temp = H-T     
                (*Array)[i][z] = (item)temp;
            }
            
            //In last position store index...
            (*Array)[i][rows*cols] = (item)i;
        }
    }
}