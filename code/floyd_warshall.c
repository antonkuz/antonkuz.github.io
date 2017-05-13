
/*
 *The code to perform the floyd marshall algorithm parallely using 
 *openMPI by dividing the distance matrix into blocks and allowing
 *individual workers to be responsible for a block each. Some parts
 *of the code related to communicating among nodes were taken from
 *stackoverflow
 */
 
#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#define MASTER 0
#define SLAVE 1
#define KILLED 2
#define INF 1000000
 
 
int main(int argc, char *argv[]) {
    int n1,n2,n3,n,my_rank,num_procs,chunk,overflow=0,*d_graph;
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    FILE *fp;
    /* master code */
    if (my_rank == MASTER) {
      double t1,t2;
      int disable=0,killed,result[3]; 
      fp= fopen("1kgraph.txt","r");
      fscanf(fp,"%d",&n);
      d_graph = (int*)malloc((n*n) * sizeof(int));
      for(int i=0; i < n; i++){
        for(int j=0; j < n; j++){
            d_graph[(i*n)+j] = 0;
          }
        }  
      while(fscanf(fp,"%d%d%d",&n1,&n2,&n3)!=EOF){   
              if(n1<n&&n2<n)            
               d_graph[(n1*n)+n2]=n3;      
            }  
      for(int i=0; i < n; i++){
          for(int j=0; j < n; j++){
            if (d_graph[(i*n)+j] == 0 && i != j) {
            d_graph[(i*n)+j] = INF;
          }
        }  
      }
      MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
      overflow= n%(num_procs-1);
      chunk= (n-overflow)/(num_procs-1);

       t1 = MPI_Wtime();
          for(int i=1;i<num_procs;i++)
         MPI_Send(&d_graph,n*n,MPI_INT,i,killed,MPI_COMM_WORLD);
      do {
         MPI_Recv(&result,3,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
         if (status.MPI_TAG == KILLED)
            killed++;
         else
            if (d_graph[(result[1]*n)+result[2]]>result[0])
                d_graph[(result[1]*n)+result[2]]=result[0];
      } while (killed < num_procs-1);
       t2 = MPI_Wtime();
      printf("computation time %f \n",t2-t1);
    }
    else{
        
        int msg[3];
        MPI_Recv(&n,1,MPI_INT,MASTER,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        MPI_Recv(&d_graph,n*n,MPI_INT,MASTER,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        if(my_rank+1!=num_procs)
            overflow=0;//Only the the last process needs to take care of the overflow.
        for (int k = chunk*(my_rank-1); k < chunk*(my_rank-1)+chunk+overflow; ++k){
            for (int i = 0; i < n; ++i){
                for (int j = 0; j < n; ++j){
                    if ((d_graph[(i*n)+k] * d_graph[(k*n)+j] != 0) && (i != j))
                        if ((d_graph[(i*n)+k] + d_graph[(k*n)+j] < d_graph[(i*n)+j]) || (d_graph[(i*n)+j] == 0)){
                            d_graph[(i*n)+j] = d_graph[(i*n)+k] + d_graph[(k*n)+j];
                            msg[0]=d_graph[(i*n)+j];
                            msg[1]=i;
                            msg[2]=j;
                            MPI_Send(&msg,3,MPI_INT,MASTER,0,MPI_COMM_WORLD);
                         }
                }
            }
        }
        MPI_Send(0,0,MPI_INT,MASTER,KILLED,MPI_COMM_WORLD);
    }
    MPI_Finalize();

 
    return 0;
}