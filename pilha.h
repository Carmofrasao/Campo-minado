#define MAXI 101

typedef struct
{
	int x, y;
} coordenada;

typedef struct 
{
	int ini; 
	coordenada v[MAXI];
} pilha;

void inicializar_pilha(pilha *p);
/* cria uma pilha vazia*/

int pilha_vazia(pilha *p);
/*retorna 1 se a pilha estavazia e 0 caso contrario*/

int pilha_unitaria(pilha p);
/*retorna 1 se a pilha tem um unico elemento*/

void empilhar(coordenada x, pilha *p);
/*insere x no inicio da pilha*/

coordenada desempilhar(pilha *p);
/*retorna o elemento do inicio da pilha e o remove dela*/

coordenada topo(pilha p);
/*retorna o elemento do inicio da pilha, sem remove-lo*/
