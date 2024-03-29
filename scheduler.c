#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 4

int counter = 0;

struct node 
{
   int pid;
   int cpuBurst;
   int totalCpuBurst;
   int waitingTime;
   struct node *next;
};

struct node **head;
struct node *headF;

// Apresenta o conteudo nas filas de prioridade
void printQueue(int priority) 
{
   struct node *ptr = head[priority];
   printf("PROCESSOS DE PRIORIDADE %d: \n[\n", priority);
	
   
   while(ptr != NULL) 
   {
      printf("   pid = %d, cpuBurst = %d)\n",ptr->pid, ptr->cpuBurst);
      ptr = ptr->next;
   }
	
   printf("]\n");
}

// Apresenta o resultado final
void printFinishedProccessList() 
{
   struct node *ptr = headF;
   printf("RESULTADO DO ESCALONAMENTO: \n[\n");
	
   
   while(ptr != NULL) 
   {
      printf("   pid = %d, totalCpuBurst = %d, waitingTime = %d)\n", ptr->pid, ptr->totalCpuBurst, ptr->waitingTime);
      ptr = ptr->next;
   }
	
   printf("]\n");
}

void printQueues() 
{
   for (int i = 0; i < N; i++)
      printQueue(i);
}

// Seleciona o processo com menor cpuBurst na Prioridade - Shortest Job First
struct node* getProccessToCompute(int priority) 
{
   printf("ESCOLHER PROCESSO COM MENOR CPUBURST NA PRIORIDADE %d\n", priority);
   struct node *ptr = head[priority];
   struct node *processSelected = ptr;
   
   while(ptr != NULL) 
   {
      ptr = ptr->next;
      if (ptr != NULL && ptr->cpuBurst < processSelected->cpuBurst)
         processSelected = ptr;
   }
   return processSelected;
}

// Seleciona o processo com maior cpuBurst na Prioridade
struct node* getProccessToPromote(int priority) 
{
   printf("ESCOLHER PROCESSO COM MAIOR CPUBURST NA PRIORIDADE %d\n", priority);
   struct node *ptr = head[priority];
   struct node *processSelected = ptr;
   
   while(ptr != NULL) 
   {
      ptr = ptr->next;
      if (ptr != NULL && ptr->cpuBurst > processSelected->cpuBurst)
         processSelected = ptr;
   }
   return processSelected;
}

// Insere em uma fila de prioridae
void insertFirst(int pid, int priority, int cpuBurst, int totalCpuBurst) 
{

   printf("INSERINDO O PROCESSO %d NA FILA DE PRIORIDADE %d\n", pid, priority);
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   link->pid = pid;
   link->cpuBurst = cpuBurst;
   link->totalCpuBurst = totalCpuBurst;
	
   link->next = head[priority];
	
   head[priority] = link;
}

// Insere o processo na lista de processos finalizados
void insertFirstFinished(int pid, int totalCpuBurst) 
{

   printf("INSERINDO O PROCESSO %d NA LISTA DE PROCESSOS FINALIZADOS\n", pid);
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   link->pid = pid;
   link->totalCpuBurst = totalCpuBurst;
   link->waitingTime = counter - totalCpuBurst;
	
   link->next = headF;
	
   headF = link;
}

// Remove um processo da fila especificada
void removeFromQueue(int pid, int priority) 
{
   printf("REMOVENDO O PROCESSO %d DA FILA DE PRIORIDADE %d\n", pid, priority);
   struct node *ptr = head[priority];
   if (ptr != NULL && ptr->pid == pid) 
   {
   	head[priority] = ptr->next;
   	free(ptr);
   	printf("REMOVIDO O PROCESSO %d DA FILA DE PRIORIDADE %d\n", pid, priority);
   	return;
   }
   	
   while(ptr != NULL) 
   {
      struct node *nextLink = ptr->next;
      if (nextLink->pid == pid) 
      {
      	ptr->next = nextLink->next;
      	free(nextLink);
      	printf("REMOVIDO O PROCESSO %d DA FILA DE PRIORIDADE %d\n", pid, priority);
      	return;
      }
      ptr = ptr->next;
   }
   printf("PROCESSO %d DA FILA DE PRIORIDADE %d\n NÃO ENCONTRADO", pid, priority);
}

// Verifica se tem processo com essa prioridade
int isEmpty(int priority) 
{
   return head[priority] == NULL;
}

// Inicia as filas
void initPriorityQueus() 
{
   head = (struct node**) malloc(sizeof(struct node*) * N);
   for (int i = 0; i < N; i++)
      head[i] = NULL;
}

// Inicia a lista de processos finalizados
void initFinishedProcessList() 
{
   headF = (struct node*) malloc(sizeof(struct node));
   headF = NULL;
}

// Retorna maior prioridade com processos ou -1 caso nao haja processos
int hasProcess() 
{
   for (int i = 0; i < N; i++) 
   {
   	printf("CHECANDO SE TEM PROCESSOS NA PRIORIDADE %d\n", i);
   	if (!isEmpty(i)) return i;
   }
   printf("PROCESSOS TERMINADOS\n");
   return -1;
}

// Computa algum processo na dada fila
void computeOnQueue(int priority)
 {
   struct node *currentProccess = getProccessToCompute(priority);
   printf("PROCESSO %d SELECIONA - CPU BURST %d -> %d\n", currentProccess->pid, currentProccess->cpuBurst, currentProccess->cpuBurst - 1);
   currentProccess->cpuBurst -= 1;
   if (currentProccess->cpuBurst <= 0) 
   {
      printf("PROCESSO %d FINALIZADO\n", currentProccess->pid);
      insertFirstFinished(currentProccess->pid, currentProccess->totalCpuBurst);
      removeFromQueue(currentProccess->pid, priority);
   }
}


// Funcao de aging dos processos
void aging() 
{
   for (int i = N - 1; i > 0; i--) {
      printf("CHECANDO SE TEM PROCESSOS NA PRIORIDADE %d\n", i);
      if (!isEmpty(i)) {
         printf("EXISTE PROCESSO NA PRIORIDADE %d\n", i);
         struct node *proccess = getProccessToPromote(i);
         printf("PROCESSO %d SERÁ PROMOVIDO\n", proccess->pid);
         insertFirst(proccess->pid, i - 1, proccess->cpuBurst, proccess->totalCpuBurst);
         removeFromQueue(proccess->pid, i);
         return;
      }
   }
   printf("NENHUM PROCESSO FOI PROMOVIDO\n");
}

int main() 
{

   initPriorityQueus();
   initFinishedProcessList();
   int pid;
   int priority;
   int cpuBurst;
   while(scanf("%d %d %d", &pid, &priority, &cpuBurst) != EOF)
   {
        if (0 <= priority && priority < N)
   	   insertFirst(pid, priority, cpuBurst, cpuBurst);
   	else
   	   printf("PROCESSO INVALIDO PRIORIDADE DEVE ESTAR ENTRE 0 E %d\n", N);
   }
   printf("---------------------------------------------------------------------------\n");
   printf("PROCESSOS RECEBIDO:\n");
   printQueues();
   printf("---------------------------------------------------------------------------\n");
   
   int currentPriority = hasProcess();
   while (currentPriority != -1) 
   {
      printf("PRIORIDADE ATUAL %d\n", currentPriority);
      counter++;
      computeOnQueue(currentPriority);
      if (counter % 10 == 0) 
      {
      	 printf("---------------------------------------------------------------------------\n");
         printf("ENVELHECENDO OS PROCESSOS - UNIDADES DE TEMPO DECORRIDAS %d\n", counter);
         printf("PROCESSOS ANTES DO AGING:\n");
         printQueues();
         aging();
         printf("PROCESSOS DEPOIS DO AGING:\n");
         printQueues();
         printf("---------------------------------------------------------------------------\n");
      }
      currentPriority = hasProcess();
   }
   printf("---------------------------------------------------------------------------\n");
   printFinishedProccessList();
   
   return 0;
}