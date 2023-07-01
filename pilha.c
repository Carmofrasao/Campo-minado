#include<stdio.h>
#include<stdlib.h>
#include"pilha.h"

void inicializar_pilha(pilha *p)
/*inicializa a estrutura.*/
{
	p->ini = 0;
}

int pilha_vazia(pilha *p)
/*retorna 1 se a pilha esta vazia e 0 caso contrario*/
{
	if (p->ini == 0)
		return 1;
	return 0;
}

int pilha_unitaria(pilha p)
/*retorna 1 se a pilha tem um unico elemento e 0 caso contrario*/
{
	if (p.ini == 1)
		return 1;
	return 0;
}

void empilhar(coordenada x, pilha *p)
/*insere x no inicio da pilha*/
{
	p->ini = p->ini + 1;
	p->v[p->ini] = x;
}

coordenada desempilhar(pilha *p)
/*retorna o elemento do inicio da pilha e o remove dela*/
{
	coordenada aux;
	aux = p->v[p->ini];
	p->ini = p->ini - 1;
	return aux;
}

coordenada topo(pilha p)
/*retorna o elemento do inicio da pilha, sem remove-lo*/
{
	return p.v[p.ini];
}
