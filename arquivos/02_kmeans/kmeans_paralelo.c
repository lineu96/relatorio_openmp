#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DIM 3

#define N_THREADS 8

#include <omp.h>
#include <time.h>

int main(void) {

    omp_set_num_threads(N_THREADS);

    //float tempo1,tempo2,tempo3;
    //time_t t1,t2,t3,t4;

    //t1 = time(NULL);

	/// declara variáveis
	int i, j, k, n, c;
	double dmin, dx;
	double *x, *mean, *sum;
	int *cluster, *count, color;
	int flips;

	/// le numero de centroides e pontos
	scanf("%d", &k);
	scanf("%d", &n);

	/// aloca vetores (ELES FORAM INICIALIZADOS EM 0?)
	x = (double *)malloc(sizeof(double)*DIM*n); // dimensao 3 x numero de pontos
	mean = (double *)malloc(sizeof(double)*DIM*k); // dimensao 3 x numero de centroides
	sum= (double *)malloc(sizeof(double)*DIM*k); // dimensao 3 x numero de centroides
	cluster = (int *)malloc(sizeof(int)*n); // dimensao numero de pontos
	count = (int *)malloc(sizeof(int)*k); // dimensao numero de centroides

    //t3 = time(NULL);

	/// inicia vetor cluster com zeros (cluster ao qual o ponto pertence)
	// no malloc parecia ja estar incializado
	for (i = 0; i<n; i++){
		cluster[i] = 0;
    }

	/// esse scan vai pra onde? pro vetor media?
	// antes daqui tava zerado depois apareceram varlores. nao entendi os valores
	for (i = 0; i<k; i++)
		scanf("%lf %lf %lf", mean+i*DIM, mean+i*DIM+1, mean+i*DIM+2);

    /// esse scan vai pra onde? pro vetor x?
    // antes daqui tava zerado depois apareceram varlores. nao entendi os valores
	for (i = 0; i<n; i++)
		scanf("%lf %lf %lf", x+i*DIM, x+i*DIM+1, x+i*DIM+2);

    /// flips recebe o numero de pontos
	flips = n;

    //t2 = time(NULL);

    /// enquanto flips for maior que zero

	while (flips>0) {

		flips = 0;

        /// zera count e sum
		for (j = 0; j < k; j++) {
			count[j] = 0;
			for (i = 0; i < DIM; i++)
				sum[j*DIM+i] = 0.0;
		}

		///AQUI É ONDE DEMORA!
        #pragma omp parallel for default(shared) private(dmin) private(color) private(c) private(dx) private(j)
		for (i = 0; i < n; i++) {
			dmin = -1; color = cluster[i];
			for (c = 0; c < k; c++) {
				dx = 0.0;
				for (j = 0; j < DIM; j++)
					dx +=  (x[i*DIM+j] - mean[c*DIM+j])*(x[i*DIM+j] - mean[c*DIM+j]);
				if (dx < dmin || dmin == -1) {
					color = c;
					dmin = dx;
				}
			}
			if (cluster[i] != color) {
				flips++;
				cluster[i] = color;
	      	}
		}

    //t3 = time(NULL);

        ///incrementa o contador e sum
	    for (i = 0; i < n; i++) {
			count[cluster[i]]++;
			for (j = 0; j < DIM; j++)
				sum[cluster[i]*DIM+j] += x[i*DIM+j];
		}


        ///atualiza o vetor de media
		for (i = 0; i < k; i++) {
			for (j = 0; j < DIM; j++) {
				mean[i*DIM+j] = sum[i*DIM+j]/count[i];
  			}
		}
	}

	for (i = 0; i < k; i++) {
		for (j = 0; j < DIM; j++)
			printf("%5.2f ", mean[i*DIM+j]);
		printf("\n");

	}
	#ifdef DEBUG
	for (i = 0; i < n; i++) {
		for (j = 0; j < DIM; j++)
			printf("%5.2f ", x[i*DIM+j]);
		printf("%d\n", cluster[i]);
	}
	#endif
    //t4 = time(NULL);

    //tempo1 = difftime(t2,t1);
    //tempo2 = difftime(t3,t2);
    //tempo3 = difftime(t4,t3);

    //printf("\n\nNumero de threads = %i", N_THREADS);
    //printf("\n\nParte sequencial = %f", tempo1+tempo3);
    //printf("\n\nParte paralela (while) = %f\n\n", tempo2);

	return(0);
}
