/* Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_PROCESSES 4

typedef struct{
	int n;
	int posicao;
} threadArgs;

/*Tipo que armazena mutex. Uma variável global, de forma a 
ficar em uma pilha visível para todas as threads        */
pthread_mutex_t trava;

/*Número de threads rodando*/
int workersNum = 0;
pthread_t workers[MAX_PROCESSES];
int primosNum = 0;

/*Variável que verifica se processo está atio ou não*/
int workerstatus[MAX_PROCESSES];

/*Função que verifica se número é primo*/
int isPrime (int num);

/*Função que atualiza travas das threads e número de processos rodando simultaneamente*/
void* worker (void *arg){
	threadArgs *info = (threadArgs *)arg;
	pthread_mutex_lock(&trava); 
	primosNum += isPrime(info -> n); //Verifica se o número é primo (1)  ou não (0) e adiciona à variável
	pthread_mutex_unlock(&trava); //Após o fim da verificação, o mutex é destravado
	workersNum --; //Número de processos diminui, e um novo pode passar a acontecer
	workerstatus[info -> posicao] = 0; //Libera posição em que thread estava sendo rodada
	free(info);
}

int main() {
	threadArgs *saveArgs;
	int input[80];
	int i = 0;
	char c;

	/*Recebe a entrada do usuário até a quebra de linha*/
	do{
		scanf("%d", &input[i++]);
	}
	while((c=getchar()) != '\n');

	int iteration = i;
	i =0;

	while (i<=iteration){
		if(workersNum<MAX_PROCESSES){
			saveArgs = (threadArgs*)malloc(sizeof(threadArgs));
			saveArgs->n = input[i++]; 
			int j = 0;
			while (workerstatus[j] == 1) 
				j ++;
			saveArgs->posicao = j;
			pthread_mutex_lock(&trava);
			workerstatus[j] = 1;
			workersNum ++;
			pthread_mutex_unlock(&trava);
			pthread_create(&(workers[j]), NULL, worker, (void*)saveArgs);
		}
	}
	
	/*Espera todos os processos se encerrarem antes do fim do programa*/
	for (int i = 0; i<MAX_PROCESSES; i++){
		if(workerstatus[i] == 1)
			pthread_join(workers[i], NULL);
	}

	/*Imprime o resultado final*/
	printf("%d\n", primosNum);
	return 0;

}

int isPrime (int num){
        if (num == 0 || num == 1)
                return 0;
        if (num == 2)
                return 1;
        for (int i=2; i<num; i++){
                if (num%i == 0)
                        return 0;
        }
        return 1;
}

