# MyScheduler
Projeto para desenvolver a abstração de um Scheduler de Multilevel Priority Queue (MPQ).

- Todos os processos chegam ao mesmo instante ao escalonador;
- 4 níveis de prioriodade na MPQ;
- As prioridades vao de 0 (mais alta) a 3 (mais baixa);
- Para realizar o desempate entre processos com a mesma prioridade, utilize SJF (Shortest Job First);
- (Aging) A cada 10 unidades de tempo (a mesma do CPU-burst), aumente em 1 a prioridade do processo com a menor
prioridade e, dentro desses, com o maior CPU-Burst.

**Entrada:** Cada processo em uma linha, cada linha tem as informações de pid, prioridade e cpu-Burst nessa Ordem.

EX:
```
1 2 1
2 0 120
3 0 190
```
