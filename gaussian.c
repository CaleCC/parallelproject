//Cheng Chen
//Implementation of Gaussian  Elimination
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>

void fillMatrix(int N, double** A,double* b);
void printMatrix(int N, double** A,double* b);
void printSolutionVector(double* x, int N);
void *eliminate(void *threading);

struct  Thread_Data{
  double** A;
  double* b;
  int N;
  int j;
  int numThreads;
} thread_data;

int main(int argc, char const *argv[]) {
  //initialize matrices, vectors, and other Variables
  int N = atoi(argv[1]);
  int numThreads = atoi(argv[2]);
  struct timeval startTime, endElimination;
  //double eliminationTime;
  pthread_t elimination_threads[numThreads];

  //allocate Matrix A
  double** A = (double **)calloc(N,sizeof(double*));
  for (int q = 0; q < N; q++){
    A[q] = (double*) malloc(sizeof(double*));
  }

  double* b = (double*) malloc(sizeof(double)*N);
  ///double* x = (double*) malloc(sizeof(double)*N);
  double record[5];
  for (int test = 0;test < 5; test++){/* code */
    fillMatrix(N,A,b);

    //double A[3][3] = {2.0,1.0,1.0,6.0,2.0,1.0,-2.0,2.0,1.0};

    //double b[3]={0,-1.0,7.0};
    if (N <= 8)
    printMatrix(N, A, b);
    //all threads share
    thread_data.A = A;
    thread_data.b = b;
    thread_data.N = N;
    thread_data.numThreads = numThreads;

    int *index = (int*)calloc(numThreads, sizeof(int*));

    for(int i = 0; i < numThreads; i++){
      index[i] = i;
    }
    gettimeofday(&startTime, NULL);

    for (int k=0; k < N; k++){
      for (int j = k+1; j < N; j++){
        A[k][j] = A[k][j] / A[k][k];
      }
      b[k] = b[k] / A[k][k];
      A[k][k] = 1;
      thread_data.j = k;
      for (int thread_index = 0; thread_index < numThreads; thread_index++){
        pthread_create(&elimination_threads[thread_index], NULL, eliminate, (void*)&index[thread_index]);
      }

      for (int thread_index = 0; thread_index < numThreads; thread_index++){
        pthread_join(elimination_threads[thread_index], NULL);
      }
    }
      //printMatrix(N, A, b);



      gettimeofday(&endElimination, NULL);
      double eliminationTime = ((endElimination.tv_sec - startTime.tv_sec) * 1000000u + endElimination.tv_usec - startTime.tv_usec) / 1.e6;
      record[test] = eliminationTime;
    }
    double sum = 0, mean, standDe = 0;
    for(int i =0; i < 5; i ++){
      sum += record[i];
      printf("eliminationtime %f seconds\n",record[i]);
    }
    mean = sum / 5;

    for (size_t i = 0; i < 5; i++) {
      standDe += pow(record[i] - mean, 2);

    }

    standDe = sqrt(standDe/5);

    printf("average eliminationtime %f seconds\n",mean);
    printf("stand deviation %f\n", standDe);
    if(N < 8){
    printMatrix(N, A, b);
    }

    return 0;

}

void *eliminate(void *threadarg){
  int *thread_index = (int*)threadarg;
  double** A = thread_data.A;
  double* b = thread_data.b;
  int N = thread_data.N;
  int j = thread_data.j;
  int numThreads = thread_data.numThreads;
  int i,q;

  for (i = j+1+ *thread_index; i < N; i += numThreads){
    for(q = j+1; q < N; q ++){
      A[i][q] = A[i][q] - A[i][j]*A[j][q];
    }
    b[i] = b[i]-A[i][j]*b[j];
    A[i][j] = 0;
    //printf("print A, thread%d row=%d  col=%d\n",*thread_index,i,j);
  }
  return NULL;
}

void printMatrix(int N, double** A, double* b){
	if (N <= 8){
		for (int x=0; x<N; x++){
			printf("| ");
			for(int y=0; y<N; y++)
				printf("%7.2f ", A[x][y]);
			printf("| | %7.2f |\n", b[x]);
		}
    printf("---------------------------------------------------\n" );
	}
	else{
		printf("\nPerforming gaussian elimination with a matrix and vector too large to print out.\n");
		printf("If you would like to see output, try again with a matrix of length 8 or less.\n");
	}
}

void fillMatrix(int N, double** A, double* b){
	int i, j;
  // double tmpA[9] = {2.0,1.0,1.0,6.0,2.0,1.0,-2.0,2.0,1.0};
  // double tmpB[3]= {1,-1.0,7.0};
  // int s=0;
  // for (size_t i = 0; i < 3; i++) {
  //     for (int j=0; j<3; j++){
  //       A[i][j]=tmpA[s++];
  //     }
  //     b[i]=tmpB[i];/* code */
  // }
  srand(time(0));
	for (i=0; i<N; i++){
		for (j=0; j<N; j++){
			A[i][j] = (rand()/(double)RAND_MAX*1000+1);
		}
		b[i] = (rand()/(double)RAND_MAX*1000+1);
	}
}
