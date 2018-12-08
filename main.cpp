#include <iostream>
#include <mpi/mpi.h>
#include <experimental/filesystem>
#include <sys/stat.h>
#include "Map.h"
#include "Combiner.h"
#include "Reduce.h"

/** copy pasted function from stack  overflow **/
std::string getStdoutFromCommand(std::string &cmd) {
    std::string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd+=" 2>&1";

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != nullptr) data.append(buffer);
        pclose(stream);
    }
    return data;
}


int main() {



    // Initialize the MPI environment
    MPI_Init(nullptr, nullptr);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


    MPI_Request request = MPI_REQUEST_NULL;
    MPI_Status status;
    /**any processor would do the job choose 0 for the 0 **/
    if (world_rank==0){

        std::cout << "my rank is " <<world_rank <<std::endl;
        /**
        * Delete files in intermediate_map and in reduce_result
        * with the command system("exec rm -r balblablabla *")
        * **/

        system("rm -r intermidate_map/*");
        system("rm -r reduce_result/*");
        std::cout << "deleted all files in intermediate_map/ "<<std::endl;


        for (int k = 1; k <6 ; ++k) {
            std::string filename="files/text_"+std::to_string(k);
            std::ifstream in(filename,std::ios::in);
            std::string line;
            int i=0;
            while (std::getline(in,line)){
                MPI_Isend(line.c_str(),(int)line.length(),MPI_CHAR,(i% (world_size-1))+1,0,MPI_COMM_WORLD,&request);
                i++;
            }
            in.close();
        }
        std::string a="FIN";
        for (int j = 1; j < world_size ; ++j) {
            MPI_Send(a.c_str(),(int)a.length(),MPI_CHAR,j,0,MPI_COMM_WORLD);
        }

    }

    else  {
        Map map;

        bool quit=false;
        while (!quit) {
            MPI_Probe(0,0,MPI_COMM_WORLD,&status);
            int count;
            MPI_Get_count(&status,MPI_CHAR,&count);
            char buf [count];
            MPI_Recv(&buf,count,MPI_CHAR,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            std::string s = buf;


            if (s=="FIN"){
                quit=true;
            }
            else {
                /**
                 * for each word in line **/
                auto *p=map.generateKeyValueList(s.substr(0,(unsigned long)count));
                for (auto &a: *p){
                    map.emitIntermediate(
                            a
                            );
                }

            }
        }




    }

    auto *filesList= new std::vector<std::string>();

    MPI_Wait(&request,&status);

    if (world_rank==0){

        std::cout << "my rank is " << world_rank << std::endl;
        std::string cmd="cd intermidate_map && ls -1";

        std::stringstream ls(getStdoutFromCommand(cmd));
        std::string item;
        char delimeter='\n';

        while (std::getline(ls, item, delimeter))
        {
            filesList->push_back(item);

        }

        {
            int a=2;
            MPI_Bcast(&a, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }

        for (size_t k = 0; k < filesList->size(); k++){
            MPI_Send(filesList->at(k).c_str(),(int)filesList->at(k).length(),MPI_CHAR,(int)(k%3)+1,0,MPI_COMM_WORLD);
        }
        std::string c="FIN";
        for (int j = 1; j < world_size ; ++j) {
            MPI_Send(c.c_str(),(int)c.length(),MPI_CHAR,j,0,MPI_COMM_WORLD);

        }


    }
    else {
        {
            /** blocks at Bcast waiting for a. Prints value read by MASTER **/
            int a;
            MPI_Bcast(&a, 1, MPI_INT,0, MPI_COMM_WORLD);
        }
        bool quit=false;
        while (!quit)
        {
            // std::cout << "my rank is " << world_rank << std::endl;

            MPI_Probe(0,0,MPI_COMM_WORLD,&status);
            int count;
            MPI_Get_count(&status,MPI_CHAR,&count);

            /** BUGGY BUG couldn't init the buffer to count :/ had to use 256 to make it work **/
            char buffer[256];
            MPI_Recv(&buffer,256,MPI_CHAR,0,0,MPI_COMM_WORLD,&status);
            std::string s = buffer;

            MPI_Wait(&request,&status);
            if (s.substr(0,(size_t)count)=="FIN"){
                quit=true;
            }
            else {
                Combiner combiner;
                auto *a=combiner.combineResult(s.substr(0,(size_t)count));

                for (const auto &it : *a) {
                    Reduce reduce;
                    std::cout << it.first <<","<< reduce.emitIntermediate(it.first,*(it.second)) <<std::endl;
                    std::ofstream out("reduce_result/reduce_"+std::to_string(world_rank),std::ios::out | std::ios::app);
                    out << it.first <<","<< reduce.emitIntermediate(it.first,*(it.second)) <<std::endl;

                    out.close();
                }


            }
        }


    }

    if (world_rank==0){
        system("rm -r intermidate_map/*");
    }

    // Finalize the MPI environment.
    MPI_Finalize();

}