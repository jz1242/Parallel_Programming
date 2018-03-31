// gameoflife.c
// Name: Jason Zhang
// JHED: jzhan127@jhu.edu

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "mpi.h"

#define DEFAULT_ITERATIONS 64
#define GRID_WIDTH  256
#define DIM  16     // assume a square grid

void update(int grid[], int dim, int start, int end);
int checkNeighbors(const int grid[], int dim, int pos);
int mod (int a, int b);
int main ( int argc, char** argv ) {
    

  int global_grid[ 256 ] = 
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

  // MPI Standard variable
  int num_procs;
  int ID, j;
  int iters = 0;
  int num_iterations;

  // Setup number of iterations
  if (argc == 1) {
    num_iterations = DEFAULT_ITERATIONS;
  }
  else if (argc == 2) {
    num_iterations = atoi(argv[1]);
  }
  else {
    printf("Usage: ./gameoflife <num_iterations>\n");
    exit(1);
  }

  // Messaging variables
  MPI_Status stat;
  // TODO add other variables as necessary

  // MPI Setup
  if ( MPI_Init( &argc, &argv ) != MPI_SUCCESS )
  {
    printf ( "MPI_Init error\n" );
  }

  MPI_Comm_size ( MPI_COMM_WORLD, &num_procs ); // Set the num_procs
  MPI_Comm_rank ( MPI_COMM_WORLD, &ID );

  assert ( DIM % num_procs == 0 );

  // TODO Setup your environment as necessary
  int block_size = DIM*(DIM/num_procs);
  int id_start = block_size*ID;



  
  int id_end = id_start + DIM*(DIM/num_procs  - 1);
  int prev = (ID - 1) % num_procs < 0 ? ((ID - 1) % num_procs) + num_procs : (ID - 1) % num_procs;
  int next = (ID + 1) % num_procs < 0 ? ((ID + 1) % num_procs) + num_procs : (ID + 1) % num_procs; 
  int prev_offset = mod(id_start - DIM, GRID_WIDTH);
  int next_offset = mod(id_end + DIM, GRID_WIDTH);
  //int block_size = DIM*DIM;
  /*if (ID == 0)
      curr_grid = malloc(sizeof(int) * GRID_WIDTH * num_procs);
  */

  for ( iters = 0; iters < num_iterations; iters++ ) {
    // TODO: Add Code here or a function call to you MPI code
        if (ID % 2 == 0) {
            MPI_Ssend(global_grid + id_start, DIM, MPI_INT, prev, 2, MPI_COMM_WORLD);
            MPI_Ssend(global_grid + id_end, DIM, MPI_INT, next, 2, MPI_COMM_WORLD);
            MPI_Recv(global_grid + next_offset, DIM, MPI_INT, next, 2, MPI_COMM_WORLD, &stat);
            MPI_Recv(global_grid + prev_offset, DIM, MPI_INT, prev, 2, MPI_COMM_WORLD, &stat);
        } else {
            MPI_Recv(global_grid + next_offset, DIM, MPI_INT, next, 2, MPI_COMM_WORLD, &stat);
            MPI_Recv(global_grid + prev_offset, DIM, MPI_INT, prev, 2, MPI_COMM_WORLD, &stat);
            MPI_Ssend(global_grid + id_start, DIM, MPI_INT, prev, 2, MPI_COMM_WORLD);
            MPI_Ssend(global_grid + id_end, DIM, MPI_INT, next, 2, MPI_COMM_WORLD);
        }
        update(global_grid, DIM, id_start, id_start + block_size);
    // Output the updated grid state
    if ( ID == 0 ) {
      printf ( "\nIteration %d: final grid:\n", iters );
      for ( j = 0; j < GRID_WIDTH; j++ ) {
        if ( j % DIM == 0 ) {
          printf( "\n" );
        }
        printf ( "%d  ", global_grid[j] );
      }
      printf( "\n" );
    }
  }

  // TODO: Clean up memory
  MPI_Finalize(); // finalize so I can exit
}

void update(int grid[], int dim, int start, int end) {
    int neighbors[GRID_WIDTH];
    for (int i = start; i < end; i++) {
        neighbors[i] = checkNeighbors(grid, dim, i);
    }
    for (int j = start; j < end;  j++) {
        if (grid[j] == 1){
            if(neighbors[j] != 2 && neighbors[j] != 3){
                grid[j] = 0;
            }
        }
        else if (grid[j] == 0){
            if(neighbors[j] == 3){
                grid[j] = 1;
            }
        }

    }
}

int checkNeighbors(const int grid[], int dim, int pos) {
    int col = pos % dim;
    int row = pos / dim;
    int totalNeighbors = 0;
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            int new_row = (row + i) % dim;
            int new_col = (col + j) % dim;
            if(new_row < 0){
                new_row += dim;
            }
            if(new_col < 0){
                new_col += dim;
            }
            int new_pos = new_col + dim*new_row;
            if(new_pos != pos){
                totalNeighbors += grid[new_pos];

            }

        }
    }
    return totalNeighbors;
}
int mod (int a, int b) {
    if(b < 0)
        return mod(a, -b);
    int mod = a % b;
    if(mod < 0)
        mod+=b;
    return mod;
}




