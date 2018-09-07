//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////*MATA40 - ESTRUTURA DE DADOS E ALGORITMOS I - 2017.2*////////////////////////////////
////////////////////////////////*ALUNO: DANILO DE ANDRADE PELETEIRO*//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./lib/avl.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// FUNÇÕES PARA IMPRESSÃO ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void avl_print_search(arvore *AVL, int x); // Imprime resultado ao procurar um Nó específico
void avl_travel_cres(arvore *AVL); // Imprime nós em ordem crescente
void avl_travel_dec(arvore *AVL); // Imprime nós em ordem decrescente
void avl_travel_level(arvore *AVL, int curr, int level); // Imprime nós de mesmo nível
void avl_travel_preord(arvore *AVL);
void avl_travel_posord(arvore *AVL);
void avl_end(arvore **AVL, int flag); // Imprime nós em ordem crescente e os remove
void altura_avl(arvore *AVL); // Imprime altura da árvore
void finaliza(arvore **AVL, int flag); // Finaliza relatório

///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// FUNÇÕES PARA LEITURA  ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void insere_no(arvore **AVL); // Lê dados e insere nó
void consulta_no(arvore *AVL); // Lê dados e consulta nó
void remove_no(arvore **AVL, int *flag); // Lê dados e remove nó
void lista_nos(arvore *AVL); // Lê dados e lista nós em ordem específica (cresc ou decresc)
void lista_nos_nivel(arvore *AVL); // Lê dados e lista nós em determinado nível


///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////  ARQUIVO PRINCIPAL  //////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

int main(){
	
	arvore *AVL;
	AVL = avl_create();
	char op;
	int flag = 0;

	while(1)
	{
		scanf(" %c", &op); // Escolhe operação a ser realizada
		switch(op)
		{
			case 'i': // Insere nó
				insere_no(&AVL);
				break;

			case 'c': // Consulta nó
				consulta_no(AVL);
				break;

			case 'r': // Remove nó
				remove_no(&AVL, &flag);
				break;

			case 'p': // Lista nós em ordem crescente e decrescente
				lista_nos(AVL);
				break;

			case 'n': // Lista nós de acordo com o nível
				lista_nos_nivel(AVL);
				break;

			case 'h': // Informa a altura da árvore
				altura_avl(AVL);
				break;

			case 'f': // Imprime e remove os nós. Finaliza o programa posteriormente
				finaliza(&AVL, flag);
				exit(0);
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// FUNÇÕES PARA IMPRESSÃO ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void avl_print_search(arvore *AVL, int x){ // Imprime resultado ao procurar um Nó específico

	if(AVL!=NULL && avl_search_node(AVL, x))
		printf("existe no com chave: %d\n", x);
	else
		printf("nao existe no com chave: %d\n", x);
}

void avl_travel_cres(arvore *AVL){ // Imprime nós em ordem crescente
	if(AVL==NULL || getLevel(AVL)==0)
		return;
	avl_travel_cres(getEsq(AVL));
	printf("%d %d %lld\n", getCode(AVL), getOp(AVL), getSaldo(AVL));
	avl_travel_cres(getDir(AVL));
}

void avl_travel_dec(arvore *AVL){ // Imprime nós em ordem decrescente
	if(AVL==NULL || getLevel(AVL)==0)
		return;
	avl_travel_dec(getDir(AVL));
	printf("%d %d %lld\n", getCode(AVL), getOp(AVL), getSaldo(AVL));
	avl_travel_dec(getEsq(AVL));
}

void avl_travel_preord(arvore *AVL){
	if(AVL==NULL || getLevel(AVL)==0) return;
	
	printf("%d %d %lld\n", getCode(AVL), getOp(AVL), getSaldo(AVL));
	avl_travel_preord(getEsq(AVL));
	avl_travel_preord(getDir(AVL));	
}

void avl_travel_posord(arvore *AVL){
	if(AVL==NULL || getLevel(AVL)==0) return;
	
	avl_travel_posord(getEsq(AVL));
	avl_travel_posord(getDir(AVL));
	printf("%d %d %lld\n", getCode(AVL), getOp(AVL), getSaldo(AVL));
}

void avl_travel_level(arvore *AVL, int curr, int level){ // Imprime nós de mesmo nível
	if(AVL==NULL || getClients(&AVL)==0)
		return;
	if(curr==level)
		printf("%d\n", getCode(AVL));
	else if(curr < level)
	{
		avl_travel_level(getEsq(AVL), curr+1, level);
		avl_travel_level(getDir(AVL), curr+1, level);
	}
}

void avl_end(arvore **AVL, int flag){ // Imprime nós em ordem de remoção de raiz e os remove posteriormente
	if(flag==0 && getLevel(*AVL)==0)
	{
		freeNode(AVL);
		return;
	}
	if(flag==0) removeFIRST(AVL);
	if((*AVL)==NULL) return;
	while(getLevel(*AVL) != 0)
	{
		printf("%d %d %lld\n", getCode(*AVL), getOp(*AVL), getSaldo(*AVL));
		avl_remove(AVL, getCode(*AVL));
	}
}

void altura_avl(arvore *AVL){ // Imprime altura da árvore
	printf("%d\n", getHeight(AVL));
}

void finaliza(arvore **AVL, int flag){  // Finaliza relatório
	printf("-+- Inicio relatorio -+-\n%d\n", getClients(AVL));
	if(AVL!=NULL || *AVL!=NULL) avl_end(AVL, flag);
	printf("-+- Fim relatorio -+-\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// FUNÇÕES PARA LEITURA  ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void insere_no(arvore **AVL){ // Lê dados e insere nó
	int cod,tipo;
	long long int valor;
	scanf(" %d %d %lld", &cod, &tipo, &valor);
	avl_push(AVL, *AVL, cod, tipo, valor);
}

void consulta_no(arvore *AVL){ // Lê dados e consulta nó
	int x;
	scanf(" %d", &x);
	avl_print_search(AVL, x);
}

void remove_no(arvore **AVL, int *flag){ // Lê dados e remove nó
	int x;
	scanf(" %d", &x);
	if(*AVL==NULL) return;
	if(avl_search_node(*AVL, x))
	{
		if(getClients(AVL)==1)
		{
			removeFIRST(AVL);
			*flag = 1;
		}
		avl_remove(AVL, x);
	}
}

void lista_nos(arvore *AVL){ // Lê dados e lista nós em ordem específica (cresc ou decresc)
	char ordem;
	scanf(" %c", &ordem);
	if(ordem=='c')
		avl_travel_cres(AVL);
	else if(ordem=='d')
		avl_travel_dec(AVL);
	else if(ordem=='r')
		avl_travel_preord(AVL);
	else
		avl_travel_posord(AVL);
}

void lista_nos_nivel(arvore *AVL){ // Lê dados e lista nós em determinado nível
	int x,z=1;
	scanf(" %d", &x);
	avl_travel_level(AVL, z, x);
}

