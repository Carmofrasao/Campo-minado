/* Anderson Aparecido do Carmo Frasão, GRR 20204069*/
/* para compilar digite o seguinte codigo em seu terminal:
   gcc -o mines mines.c pilha.c */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pilha.h"

#define MAX 9			  /*tamanho maximo da matriz*/
#define MINA -1			  
#define NADA 0			  
#define VITORIA 1		  
#define DERROTA 2		  
#define EM_ANDAMENTO 3		  
#define BORDA -2		  
#define SIMBOLO_MINA "@"	  /*representa uma mina*/
#define SIMBOLO_SEM_MINA_VIZ " "  /*representa quadrado sem mina dentro*/
#define SIMBOLO_ESCONDIDO "* "	  /*representa quandrado nao revelado*/

typedef struct 
{
	int info; 		/*ou tem mina ou o numero de minas vizinhas*/
	int revelado;		/*quando nao imprime o conteudo*/ 
} quadrado;
typedef
	quadrado matriz[MAX+1][MAX+1];

typedef struct
{
	int x, y;		  /*numero de linhas e colunas*/
	int total_minas;	  /*numero total de minas no campo*/
	int falta_abrir;	  /*numero de quadrados que falta abrir*/
	int status;		  /*indica se o jogador ganhou ou perdeu o jogo*/
	matriz m;
} campo_minado;

void revelar_tudo(campo_minado *c)
/* usada quando o jogador perde, para mostrar o campo minado aberto*/
{
	int i, j;
	for (i=1; i<=c->x; i++)
		for(j=1; j<=c->y; j++)
			c->m[i][j].revelado = 1;
}

int eh_borda(int i, int j, campo_minado c)
/*retorna 1 quando uma linha ou coluna eh uma borda*/
{
	if (i==0 || i==c.x+1 || j==0 || j==c.y+1)
		return 1;
	return 0;
}

void zerar_campo(campo_minado *c)
/*prepara o campo: define as bordas, zera o tabuleiro e inicia info revelado*/
{
	int i, j;
	for (i=1; i<=c->x; i++) 
		for (j=1; j<=c->y; j++)
		{
			if (eh_borda(i, j, *c) == 1)
				c->m[i][j].info = BORDA;
			else
				c->m[i][j].info = NADA;
			c->m[i][j].revelado = 0;
		} 
}

void gerar_minas(campo_minado *c)
/*coloca o total de minas definido em posições aleatorias do campo*/
{
	srand(time(NULL));
	int i, x, y;
	for (i=1; i<=c->total_minas; i++)
	{
		do
		{
			x = 1 + (rand() % (c->x));
			y = 1 + (rand() % (c->y));
		}
		while (c->m[x][y].info == MINA || c->m[x][y].info == BORDA); /*garante mina em lugar vazio*/
		c->m[x][y].info = MINA;
	}
}

int num_vizinhos_com_mina (int x, int y, campo_minado *c)
/*para um quadrado valido conta as minas no 8 vizinhos*/
/*so entra aqui quando o proprio quadrado nao tem mina*/
{
	int i, j, cont;
	cont = 0;
	for(i=x-1; i<= x+1; i++)
		for (j=y-1; j<=y+1; j++)
			if(c->m[i][j].info == MINA)
				cont = cont + 1;
	return cont;
}

void contar_vizinhos_com_mina(campo_minado *c)
/*para os quadrados que nao sao minas, conta os vizinhos que sao minas*/
{
	int i, j;
	for(i=1; i<=c->x; i++)
		for(j=1; j<=c->y; j++)
			if(c->m[i][j].info == NADA)
				c->m[i][j].info = num_vizinhos_com_mina(i, j, c);
}

void inicializar_campo(campo_minado *c)
/*chama as funcoes que iniciam a estrutura*/
{
	printf("Defina numero de linhas (entre 1 e %d):\n", MAX);
	scanf("%d", &c->x);
	printf("Defina numero de colunas (entre 1 e %d):\n", MAX);
	scanf("%d", &c->y);
	printf("Defina numero de minas (entre 1 e %d): \n", c->x * c->y);
	scanf("%d", &c->total_minas);
	c->falta_abrir = (c->x) * (c->y) - c->total_minas; /*para teste se ganhou o jogo*/
	c->status = EM_ANDAMENTO; 			   /*para inicializar variavel*/
	zerar_campo(c);	/*coloca zero em todas as posicoes*/
	gerar_minas(c); /*espalha minas aleatoriamente no campo minado*/
	contar_vizinhos_com_mina(c);	/*conta as bombas nos vizinhos e armazena*/
}

void imprimir_campo (campo_minado c)
{
	int i, j;
	system("clear");
	printf ("  ");
	for (i = 1; i <= c.x; i++)
		printf ("%d ",i);
	printf ("\n");
	printf (" +");
	for (i = 1; i <= 2 * c.x; i++)
		printf ("-");
	printf ("\n");
	for (i = 1; i <= c.x; i++)
	{
		printf ("%d| " ,i);
		for (j = 1; j <= c.y; j++)
			if (c.m[i][j].revelado == 0)
				printf (SIMBOLO_ESCONDIDO);
			else
			{
				if (c.m[i][j].info == MINA)
					printf (SIMBOLO_MINA);
				else
					if (c.m[i][j].info == NADA)
						printf (SIMBOLO_SEM_MINA_VIZ);
					else
						printf ("%d ",c.m[i][j].info);
			}
		printf ("\n");
	}
}

void ler_jogada(int *x, int *y, campo_minado *c)
/*le uma jogada, nao testa consistencia, mas testa se eh quadrado novo.*/
{
	do
	{
		printf("Escolha a linha para jogar: ");
		scanf("%i", x);
		printf("Escolha a coluna para jogar: ");
		scanf("%i", y);
	}
	while (c->m[*x][*y].revelado != 0); /*nao pode jogar no mesmo lugar novamente*/
}

int abrir_vizinhos_sem_mina(campo_minado *c, pilha *p)
/*usa a pilha de coordenadas para abrir todos os vizinhos que nao tem mina*/
/*retorna o numero de quadros abertos*/
{
	coordenada quadrado, vizinho;
	int i, j, cont;
	cont = 1;
	while (pilha_vazia(p) != 1)
	{
		quadrado = desempilhar(p);
		for (i=quadrado.x-1; i>= quadrado.x+1; i++)
			for (j=quadrado.y-1; j>= quadrado.y+1; j++)
				if ((c->m[i][j].revelado != 1) && (c->m[i][j].info == NADA) && (eh_borda(i, j, *c) != 1))
				{
					vizinho.x = i;
					vizinho.y = j;
					empilhar (vizinho, p);
					c->m[i][j].revelado = 1;
					cont++;
				}
	}
	return cont;
}

void executar_jogada(int x, int y, campo_minado *c)
/*dada a coordenada (x, y) concretiza a jogada*/
{
	coordenada coord;
	int cont;
	pilha p;
	if (c->m[x][y].info == MINA) /*achou mina...*/
	{
		c->status = DERROTA;
		revelar_tudo(c); /*para revelado todo o campo no final*/
	}
	else /*nao eh mina*/
	{
		cont = 1;
		c->m[x][y].revelado = 1;
		if (c->m[x][y].info == NADA) /*quadrado sem vizinho com mina*/
		{
			coord.x = x;
			coord.y = y;
			inicializar_pilha(&p);
			empilhar(coord, &p);
			cont = abrir_vizinhos_sem_mina(c, &p);
		}
		c->falta_abrir = c->falta_abrir - cont;
		if (c->falta_abrir == 0) /*ganhou*/
			c->status = VITORIA;
	}
}

int ganhou(campo_minado c)
/*ganha quando nao falta abrir nenhum quadrado sem mina*/
{
	if (c.status == VITORIA)
		return 1;
	return 0;
}

int perdeu(campo_minado c)
/*c->status = DERROTA quando clicou em uma mina*/
{
	if (c.status == DERROTA)
		return 1;
	return 0;
}


int main()
{
	campo_minado c;
	int x, y;
	inicializar_campo(&c);
	imprimir_campo(c);
	do
	{
		ler_jogada(&x, &y, &c);
		executar_jogada(x, y, &c);
		imprimir_campo(c);
	}
	while(ganhou(c) != 1 && perdeu(c) != 1);
	if (ganhou(c) == 1)
		printf("Parabens, voce ganhou!!!\n");
	else
		printf("Voce perdeu...\n");

}
