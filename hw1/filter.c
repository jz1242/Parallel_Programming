/*******************************************************************************
*
*  Filter a large array based on the values in a second array.
*
********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <omp.h>
#include <math.h>
#include <assert.h>

/* Example filter sizes */
//#define DATA_LEN  512*512*256
#define DATA_LEN  512*512*128
#define FILTER_LEN  512


/* Subtract the `struct timeval' values X and Y,
    storing the result in RESULT.
    Return 1 if the difference is negative, otherwise 0. */
int timeval_subtract (struct timeval * result, struct timeval * x, struct timeval * y)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }
    
  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

/* Function to apply the filter with the filter list in the outside loop */
void serialFilterFirst ( int data_len, unsigned int* input_array, unsigned int* output_array, int filter_len, unsigned int* filter_list )
{
  /* Variables for timing */
  struct timeval ta, tb, tresult;

  /* get initial time */
  gettimeofday ( &ta, NULL );

  /* for all elements in the filter */ 
  for (int y=0; y<filter_len; y++) { 
    /* for all elements in the data */
    for (int x=0; x<data_len; x++) {
       
      if (input_array[x] == filter_list[y]) {
        
        output_array[x] = input_array[x];
      }
    }
  }

  /* get initial time */
  gettimeofday ( &tb, NULL );

  timeval_subtract ( &tresult, &tb, &ta );

  printf ("Serial filter, %lu, %d, 0\n", tresult.tv_sec*1000000 + tresult.tv_usec, filter_len);
}


/* Function to apply the filter with the filter list in the outside loop */
void serialDataFirst ( int data_len, unsigned int* input_array, unsigned int* output_array, int filter_len, unsigned int* filter_list )
{
  /* Variables for timing */
  struct timeval ta, tb, tresult;

  /* get initial time */
  gettimeofday ( &ta, NULL );

  /* for all elements in the data */
  for (int x=0; x<data_len; x++) {
    /* for all elements in the filter */ 
    for (int y=0; y<filter_len; y++) { 
       
      if (input_array[x] == filter_list[y]) {
        
        output_array[x] = input_array[x];
      }
    }
  }

  /* get initial time */
  gettimeofday ( &tb, NULL );

  timeval_subtract ( &tresult, &tb, &ta );

  printf ("Serial data, %lu, %d, 0\n", tresult.tv_sec*1000000 + tresult.tv_usec, filter_len);
}

/* Function to apply the filter with the filter list in the outside loop */
void parallelFilterFirst ( int data_len, unsigned int* input_array, unsigned int* output_array, int filter_len, unsigned int* filter_list )
{
    /* Variables for timing */
  struct timeval ta, tb, tresult;

  /* get initial time */
  gettimeofday ( &ta, NULL );
  #pragma omp parallel for //schedule(dynamic, 4)
  /* for all elements in the filter */ 
  for (int y=0; y<filter_len; y++) { 
    /* for all elements in the data */
    for (int x=0; x<data_len; x++) {
       
      if (input_array[x] == filter_list[y]) {
        
        output_array[x] = input_array[x];
      }
    }
    /*for (int x=0; x<data_len; x+=2) {
       
      if (input_array[x] == filter_list[y]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x + 1] == filter_list[y]) {
        
        output_array[x + 1] = input_array[x + 1];
      }
    }
    for (int x=0; x<data_len; x+=4) {
       
      if (input_array[x] == filter_list[y]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x + 1] == filter_list[y]) {
        
        output_array[x + 1] = input_array[x + 1];
      }
      if (input_array[x + 2] == filter_list[y]) {
        
        output_array[x + 2] = input_array[x + 2];
      }
      if (input_array[x + 3] == filter_list[y]) {
        
        output_array[x + 3] = input_array[x + 3];
      }
    }
    for (int x=0; x<data_len; x+=8) {
       
      if (input_array[x] == filter_list[y]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x + 1] == filter_list[y]) {
        
        output_array[x + 1] = input_array[x + 1];
      }
      if (input_array[x + 2] == filter_list[y]) {
        
        output_array[x + 2] = input_array[x + 2];
      }
      if (input_array[x + 3] == filter_list[y]) {
        
        output_array[x + 3] = input_array[x + 3];
      }
      if (input_array[x + 4] == filter_list[y]) {
        
        output_array[x + 4] = input_array[x + 4];
      }
      if (input_array[x + 5] == filter_list[y]) {
        
        output_array[x + 5] = input_array[x + 5];
      }
      if (input_array[x + 6] == filter_list[y]) {
        
        output_array[x + 6] = input_array[x + 6];
      }
      if (input_array[x + 7] == filter_list[y]) {
        
        output_array[x + 7] = input_array[x + 7];
      }
    }
    for (int x=0; x<data_len; x+=16) {
       
      if (input_array[x] == filter_list[y]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x + 1] == filter_list[y]) {
        
        output_array[x + 1] = input_array[x + 1];
      }
      if (input_array[x + 2] == filter_list[y]) {
        
        output_array[x + 2] = input_array[x + 2];
      }
      if (input_array[x + 3] == filter_list[y]) {
        
        output_array[x + 3] = input_array[x + 3];
      }
      if (input_array[x + 4] == filter_list[y]) {
        
        output_array[x + 4] = input_array[x + 4];
      }
      if (input_array[x + 5] == filter_list[y]) {
        
        output_array[x + 5] = input_array[x + 5];
      }
      if (input_array[x + 6] == filter_list[y]) {
        
        output_array[x + 6] = input_array[x + 6];
      }
      if (input_array[x + 7] == filter_list[y]) {
        
        output_array[x + 7] = input_array[x + 7];
      }
      if (input_array[x + 8] == filter_list[y]) {
        
        output_array[x + 8] = input_array[x + 8];
      }
      if (input_array[x + 9] == filter_list[y]) {
        
        output_array[x + 9] = input_array[x + 9];
      }
      if (input_array[x + 10] == filter_list[y]) {
        
        output_array[x + 10] = input_array[x + 10];
      }
      if (input_array[x + 11] == filter_list[y]) {
        
        output_array[x + 11] = input_array[x + 11];
      }
      if (input_array[x + 12] == filter_list[y]) {
        
        output_array[x + 12] = input_array[x + 12];
      }
      if (input_array[x + 13] == filter_list[y]) {
        
        output_array[x + 13] = input_array[x + 13];
      }
      if (input_array[x + 14] == filter_list[y]) {
        
        output_array[x + 14] = input_array[x + 14];
      }
      if (input_array[x + 15] == filter_list[y]) {
        
        output_array[x + 15] = input_array[x + 15];
      }
    }*/
  }

  /* get initial time */
  gettimeofday ( &tb, NULL );

  timeval_subtract ( &tresult, &tb, &ta );

  printf ("Parallel filter, %lu, %d, %d\n", tresult.tv_sec*1000000 + tresult.tv_usec, filter_len, omp_get_max_threads());
}


/* Function to apply the filter with the filter list in the outside loop */
void parallelDataFirst ( int data_len, unsigned int* input_array, unsigned int* output_array, int filter_len, unsigned int* filter_list )
{
    /* Variables for timing */
  struct timeval ta, tb, tresult;

  /* get initial time */
  gettimeofday ( &ta, NULL );
  #pragma omp parallel for //schedule(dynamic, 4)
  /* for all elements in the data */
  for (int x=0; x<data_len; x++) {
    /* for all elements in the filter */ 
    for (int y=0; y<filter_len; y++) { 

      if (input_array[x] == filter_list[y]) {
        output_array[x] = input_array[x];
      }
    }
  /*  for (int y=0; y<filter_len; y+=2) {
       
      if (input_array[x] == filter_list[y]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 1]) {
        
        output_array[x] = input_array[x];
      }
    }
    for (int y=0; y<filter_len; y+=4) {

      if (input_array[x] == filter_list[y]) {
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 1]) {
    
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 2]) {
     
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 3]) {
        
        output_array[x] = input_array[x];
      }
    }
    for (int y=0; y<filter_len; y+=8) {
      
      if (input_array[x] == filter_list[y]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 1]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 2]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 3]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 4]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 5]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 6]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 7]) {
        
        output_array[x] = input_array[x];
      }
    }
    /*for (int y=0; y<filter_len; y+=16) {
       
      if (input_array[x] == filter_list[y]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 1]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 2]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 3]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 4]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 5]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 6]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 7]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 8]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 9]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 10]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 11]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 12]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 13]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 14]) {
        
        output_array[x] = input_array[x];
      }
      if (input_array[x] == filter_list[y + 15]) {
        
        output_array[x] = input_array[x];
      }
    }*/
  }

  /* get initial time */
  gettimeofday ( &tb, NULL );

  timeval_subtract ( &tresult, &tb, &ta );

  printf ("Parallel data, %lu, %d, %d\n", tresult.tv_sec*1000000 + tresult.tv_usec, filter_len, omp_get_max_threads());
}


void checkData ( unsigned int * serialarray, unsigned int * parallelarray )
{
  for (int i=0; i<DATA_LEN; i++)
  {
    if (serialarray[i] != parallelarray[i])
    {
      printf("Data check failed offset %d\n", i );
      return;
    }
  }
}


int main( int argc, char** argv )
{
  /* loop variables */
  int x,y;

  /* Create matrixes */
  unsigned int * input_array;
  unsigned int * serial_array;
  unsigned int * output_array;
  unsigned int * filter_list;

  /* Initialize the data. Values don't matter much. */
  posix_memalign ( (void**)&input_array, 4096,  DATA_LEN * sizeof(unsigned int));
//  input_array = (unsigned int*) posix_memalign ( DATA_LEN * sizeof(unsigned int), 4096);
  for (x=0; x<DATA_LEN; x++)
  {
    input_array[x] = x % 2048;
  }

  /* start with an empty *all zeros* output array */
  posix_memalign ( (void**)&output_array, 4096, DATA_LEN * sizeof(unsigned int));
  memset ( output_array, 0, DATA_LEN );
  posix_memalign ( (void**)&serial_array, 4096, DATA_LEN * sizeof(unsigned int));
  memset ( serial_array, 0, DATA_LEN );

  /* Initialize the filter. Values don't matter much. */
  filter_list = (unsigned int*) malloc ( FILTER_LEN * sizeof(unsigned int));
  for (y=0; y<FILTER_LEN; y++)
  {
    filter_list[y] = y;
  }

  /* Execute at a variety of filter lengths */
  /*for ( int filter_len =1; filter_len<=FILTER_LEN; filter_len*=2) 
  {
    serialDataFirst ( DATA_LEN, input_array, serial_array, filter_len, filter_list );
    memset ( output_array, 0, DATA_LEN );

    serialFilterFirst ( DATA_LEN, input_array, output_array, filter_len, filter_list );
    checkData ( serial_array, output_array );
    memset ( output_array, 0, DATA_LEN );

  }*/
  /* part 2*/
  int filter_len = 512;
  for ( int i = 1; i <=16 ; i*=2) 
  {
    omp_set_num_threads (i);
    serialDataFirst ( DATA_LEN, input_array, serial_array, filter_len, filter_list );
    memset ( output_array, 0, DATA_LEN );
    
    serialFilterFirst ( DATA_LEN, input_array, output_array, filter_len, filter_list );
    checkData ( serial_array, output_array );
    memset ( output_array, 0, DATA_LEN );


    parallelFilterFirst ( DATA_LEN, input_array, output_array, filter_len, filter_list );
    checkData ( serial_array, output_array );
    memset ( output_array, 0, DATA_LEN );

    parallelDataFirst ( DATA_LEN, input_array, output_array, filter_len, filter_list );
    checkData ( serial_array, output_array );
    memset ( output_array, 0, DATA_LEN );
  }
  
}

