#include "../headers/cluster.h"

int main(int argc, char** argv)
{
    string answer,input_file_original_space,input_file_new_space, query_file_original_space,query_file_new_space, output_file,configuration_file,clusters_file;
    int k=4,L=5;

    
    if(strcmp(argv[0],"./search") == 0)
    {
        if(argc!=15)   
        {
            cout << endl << "Please give an input with this form: ./search –d <input file original space> -i <input file new space> –q <query file original space> -s <query file new space> –k <int> -L <int> -ο <output file>\n";
            return -1;
        }

        for(int i=1;i<argc;i+=2)
        {
            if(strcmp(argv[i],"-d") == 0)
                input_file_original_space = (argv[i+1]);    
            else if(strcmp(argv[i],"-i") == 0)
                input_file_new_space = (argv[i+1]);                
            else if(strcmp(argv[i],"-q") == 0)
                query_file_original_space = (argv[i+1]);
             else if(strcmp(argv[i],"-s") == 0)
                query_file_new_space = (argv[i+1]);
            else if(strcmp(argv[i],"-o") == 0)
                output_file = (argv[i+1]);
            else if(strcmp(argv[i],"-k") == 0)
                k = atoi(argv[i+1]);
            else if(strcmp(argv[i],"-L") == 0)
                L = atoi(argv[i+1]);
            else
            {
                cout << "Please give an input with this form: ./search –d <input file original space> -i <input file new space> –q <query file original space> -s <query file new space> –k <int> -L <int> -ο <output file>\n";
                return -1;
            }
        }
    }

    else if(strcmp(argv[0],"./cluster") == 0)
    {
        if(argc!=11)    
        {
            cout << endl << "Please give an input with this form: ./cluster –d <input file original space> -i <input file new space> -n <classes from NN as clusters file> –c <configuration file> -o <output file>" <<  endl;
            return -1;
        }

        for(int i=1;i<argc;i+=2)
        {
            if(strcmp(argv[i],"-d") == 0)
                input_file_original_space = (argv[i+1]);      
            else if(strcmp(argv[i],"-i") == 0)
                input_file_new_space = (argv[i+1]);              
            else if(strcmp(argv[i],"-i") == 0)
                clusters_file = (argv[i+1]);              
            else if(strcmp(argv[i],"-c") == 0)
                configuration_file = (argv[i+1]);
            else if(strcmp(argv[i],"-o") == 0)
                output_file = (argv[i+1]);
            else
            {
                cout << endl << "Please give an input with this form: ./cluster –d <input file original space> -i <input file new space> -n <classes from NN as clusters file> –c <configuration file> -o <output file>" <<  endl;
                return -1;
            }
        }
    }
    else
    {
        cout << "Executable isn't valid! Try ./lsh or ./cube or ./cluster !" << endl;
        return -1;
    }   

    while(true)
    {
        LSH* lsh = NULL;
        Cluster* Lloyd = NULL;

        if(strcmp(argv[0],"./lsh") == 0)    
        {
            lsh = new LSH(input_file_original_space,input_file_new_space,query_file_original_space,query_file_new_space,output_file,L,k);    
            lsh->InitLSH();
            lsh->Approximate_LSH();
        }        
        else if(strcmp(argv[0],"./cluster") == 0)   
        {
            Lloyd = new Cluster(input_file_original_space,input_file_new_space,output_file,clusters_file,configuration_file);
            Lloyd->Clustering();
        }
        string answer; 

        cout << endl << "Do you want to question something else? (y/n)" << endl;
        cin >> answer;

        if(strcmp(argv[0],"./lsh") == 0)    delete lsh;       
        else if(strcmp(argv[0],"./cluster") == 0)   delete Lloyd;
        
        if((answer == "no") || (answer == "No") || (answer == "NO") || (answer == "n") || (answer =="N"))
        {
            fstream file;
            file.open(output_file, std::ofstream::out | std::ofstream::trunc);
            file.close();
            break;
        }
        // else
        // {
        //     fstream file;
        //     file.open(output_file, std::ofstream::out | std::ofstream::trunc);
        //     file.close();

        //     cout << "Please give a new query file:\n";
        //     cin >> query_file;
        // } 

    }
    return 0; 
    }