#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define TamanhoMatriz 5000
#define NumThreads 2

float matrizA[TamanhoMatriz][TamanhoMatriz];
float matrizB[TamanhoMatriz][TamanhoMatriz];
float resultado[TamanhoMatriz][TamanhoMatriz];

typedef struct {
  int start;
  int end;
} ThreadArgs;

void MultiplicarMatrizPosicional(int start, int end) {
  for (int i = start; i < end; i++) {
    for (int j = 0; j < TamanhoMatriz; j++) {
      resultado[i][j] = matrizA[i][j] * matrizB[i][j];
    }
  }
}

void MultiplicarMatrizMatricial(int start, int end) {
  for (int i = start; i < end; i++) {
    for (int j = 0; j < TamanhoMatriz; j++) {
      float temp = 0.0f;
      for (int k = 0; k < TamanhoMatriz; k++) {
        temp += matrizA[i][k] * matrizB[k][j];
      }
      resultado[i][j] = temp;
    }
  }
}

void ZerarMatriz() {
  for (int i = 0; i < TamanhoMatriz; i++) {
    for (int j = 0; j < TamanhoMatriz; j++) {
      resultado[i][j] = 0.0f;
    }
  }
}

void PreencherMatrizAleatoria() {
  for (int i = 0; i < TamanhoMatriz; i++) {
    for (int j = 0; j < TamanhoMatriz; j++) {
      matrizA[i][j] = (float)(rand() % 100);
      matrizB[i][j] = (float)(rand() % 100);
    }
  }
  
}

void *ThreadMultiplication(void *args) {
  ThreadArgs *threadArgs = (ThreadArgs *)args;
  MultiplicarMatrizPosicional(threadArgs->start, threadArgs->end);
  return NULL;
}

void *ThreadMultiplicationMatricial(void *args) {
  ThreadArgs *threadArgs = (ThreadArgs *)args;
  MultiplicarMatrizMatricial(threadArgs->start, threadArgs->end);
  return NULL;
}

int main() {
  ZerarMatriz();
  PreencherMatrizAleatoria();

  struct timeval start, end;
  long SeqPosicional, SeqMatricial, ParPosicional, ParMatricial;

  gettimeofday(&start, NULL);
  MultiplicarMatrizPosicional(0, TamanhoMatriz);
  gettimeofday(&end, NULL);
  SeqPosicional =
      (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

  ZerarMatriz();

  gettimeofday(&start, NULL);
  MultiplicarMatrizMatricial(0, TamanhoMatriz);
  gettimeofday(&end, NULL);
  SeqMatricial =
      (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

  ZerarMatriz();

  pthread_t threads[NumThreads];
  ThreadArgs threadArgs[NumThreads];
  int linhasPorThread = TamanhoMatriz / NumThreads;

  gettimeofday(&start, NULL);
  for (int i = 0; i < NumThreads; i++) {
    threadArgs[i].start = i * linhasPorThread;
    threadArgs[i].end =
        (i == NumThreads - 1) ? TamanhoMatriz : (i + 1) * linhasPorThread;
    pthread_create(&threads[i], NULL, ThreadMultiplication, &threadArgs[i]);
  }

  for (int i = 0; i < NumThreads; i++) {
    pthread_join(threads[i], NULL);
  }
  gettimeofday(&end, NULL);
  ParPosicional =
      (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

  ZerarMatriz();

  pthread_t threadsMatricial[NumThreads];
  gettimeofday(&start, NULL);
  for (int i = 0; i < NumThreads; i++) {
    threadArgs[i].start = i * linhasPorThread;
    threadArgs[i].end =
        (i == NumThreads - 1) ? TamanhoMatriz : (i + 1) * linhasPorThread;
    pthread_create(&threadsMatricial[i], NULL, ThreadMultiplicationMatricial,
                   &threadArgs[i]);
  }

  for (int i = 0; i < NumThreads; i++) {
    pthread_join(threadsMatricial[i], NULL);
  }
  gettimeofday(&end, NULL);
  ParMatricial =
      (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

  printf("Multiplicação Posicional:\n");
  printf("  Tempo de processamento sequencial: %ld ms\n", SeqPosicional);
  printf("  Tempo de processamento multithread: %ld ms\n", ParPosicional);
  printf("Multiplicação Matricial:\n");
  printf("  Tempo de processamento sequencial: %ld ms\n", SeqMatricial);
  printf("  Tempo de processamento multithread: %ld ms\n", ParMatricial);

  return 0;
}
