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

struct TREE{

	int codigo_cliente; // Chave de cada cliente
	long long int saldo; // Saldo atual de cada cliente
	int qtd_op; // Quantidade de operações realizadas por cada cliente
	int altura_no; // Altura do nó atual
	int nivel_no; // Nível do nó atual (Utilizado somente para a raiz da Árvore e seu "pai")
	arvore *fi_esq; // Ponteiro para filho esquerdo do nó
	arvore *fi_dir; // Ponteiro para filho direito do nó
	arvore *pai; // Ponteiro para pai do nó
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// FUNÇÕES DE CRIAÇÃO DE NÓS ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

arvore* avl_create(){ //Cria a Árvore
	arvore *AVL;
	AVL = malloc(sizeof(arvore));
	if(AVL==NULL)
		return 0;
	AVL->codigo_cliente = 0;
	AVL->saldo = 0;
	AVL->qtd_op = 0;
	AVL->altura_no = 0;
	AVL->nivel_no = 0;
	AVL->fi_esq = NULL;
	AVL->fi_dir = NULL;
	AVL->pai = NULL;
	return AVL;
}

void avl_push(arvore **AVL, arvore *avl, int cod_client, int operation, long long int value){ // Insere nó e Rebalanceia (se necessário) a Árvore 
	if(*AVL!=NULL && (*AVL)->altura_no==0)
	{
		arvore *tmp;
		tmp = malloc(sizeof(arvore));
		if(tmp==NULL)
			return;
		tmp->codigo_cliente = cod_client;
		if(operation==0) tmp->saldo = value;
		else tmp->saldo = -value;
		tmp->qtd_op = 1;
		tmp->altura_no = 1;
		tmp->nivel_no = 1;
		tmp->fi_esq = NULL;
		tmp->fi_dir = NULL;
		tmp->pai = *AVL;
		*AVL = tmp;
		return;
	}
	else if(*AVL!=NULL && (*AVL)->nivel_no==1 && ((*AVL)->fi_esq == NULL && (*AVL)->fi_dir == NULL))
	{
		if(cod_client == (*AVL)->codigo_cliente)
		{
			(*AVL)->qtd_op++;
			if(operation==0) (*AVL)->saldo += value;
			else (*AVL)->saldo -= value;
		}
		else if(cod_client < (*AVL)->codigo_cliente)
		{
			arvore *tmp;
			tmp = malloc(sizeof(arvore));
				if(tmp==NULL) return;
			tmp->codigo_cliente = cod_client;
			if(operation==0) tmp->saldo = value;
			else tmp->saldo = -value;
			tmp->qtd_op = 1;
			tmp->altura_no = 1;
			tmp->nivel_no = 2;
			tmp->fi_esq = NULL;
			tmp->fi_dir = NULL;
			tmp->pai = (*AVL);
			avl_parent_fix(AVL);
			(*AVL)->fi_esq = tmp;
			avl_parent_fix(AVL);
		}
		else if(cod_client > (*AVL)->codigo_cliente)
		{
			arvore *tmp;
			tmp = malloc(sizeof(arvore));
				if(tmp==NULL) return;
			tmp->codigo_cliente = cod_client;
			if(operation==0) tmp->saldo = value;
			else tmp->saldo = -value;
			tmp->qtd_op = 1;
			tmp->altura_no = 1;
			tmp->nivel_no = 2;
			tmp->fi_esq = NULL;
			tmp->fi_dir = NULL;
			tmp->pai = (*AVL);
			avl_parent_fix(AVL);
			(*AVL)->fi_dir = tmp;	
			avl_parent_fix(AVL);
		}
		avl_parent_fix(AVL);
		(*AVL)->altura_no = maior(avl_height_node((*AVL)->fi_esq), avl_height_node((*AVL)->fi_dir)) + 1; 
		return;
	}
	else if(*AVL==NULL)
	{
		arvore *tmp;
		tmp = malloc(sizeof(arvore));
			if(tmp==NULL) return;
		tmp->codigo_cliente = cod_client;
		if(operation==0) tmp->saldo = value;
		else tmp->saldo = -value;
		tmp->qtd_op = 1;
		tmp->altura_no = 1;
		tmp->nivel_no = 2;
		tmp->fi_esq = NULL;
		tmp->fi_dir = NULL;
		tmp->pai = (*AVL);
		*AVL = tmp;
		return;		
	}
	arvore *papai;
	papai = avl;
	arvore *aux;
	aux = *AVL;
	(*AVL)->altura_no = maior(avl_height_node((*AVL)->fi_esq), avl_height_node((*AVL)->fi_dir)) + 1;
	if(cod_client < aux->codigo_cliente)
	{
		avl_push(&((*AVL)->fi_esq), papai, cod_client, operation, value);
		if(avl_fab((*AVL))>=2)
		{
			if(cod_client < (*AVL)->fi_esq->codigo_cliente)
				avl_rotation_LL(AVL);
			else
				avl_rotation_LR(AVL);
		}
	
	}
	else if(cod_client > aux->codigo_cliente)
	{
		avl_push(&((*AVL)->fi_dir), papai, cod_client, operation, value);
	
		if(avl_fab((*AVL))>=2)
		{
			if(cod_client > (*AVL)->fi_dir->codigo_cliente)
				avl_rotation_RR(AVL);
			else
				avl_rotation_RL(AVL);
		}
	}
	else
	{
		if(operation == 0)
		{
			(*AVL)->saldo+=value;
			(*AVL)->qtd_op++;
		}
		else
		{
			(*AVL)->saldo-=value;
			(*AVL)->qtd_op++;
		}
		return;
	}
	avl_parent_fix(AVL);
	(aux)->altura_no = maior(avl_height_node(aux->fi_esq), avl_height_node(aux->fi_dir)) + 1;
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// FUNÇÕES DE REMOÇÃO DE NÓS ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void avl_remove(arvore **AVL, int cod_client){ // Remove nó e Rebalanceia (se necessário) a Árvore
	if((*AVL)==NULL) return;
	if((*AVL)!=NULL && (*AVL)->nivel_no == 0)
		return;

	if(cod_client < (*AVL)->codigo_cliente)
	{
		avl_remove(&((*AVL)->fi_esq), cod_client);
		if(*AVL!=NULL) (*AVL)->altura_no = maior(avl_height_node((*AVL)->fi_esq), avl_height_node((*AVL)->fi_dir)) + 1;
		avl_parent_fix(AVL);
		if(avl_fab(*AVL) >= 2)
		{
			if(avl_height_node((*AVL)->fi_dir->fi_esq) <= avl_height_node((*AVL)->fi_dir->fi_dir))
			{
				avl_rotation_RR(AVL);
				avl_parent_fix(AVL);				
			}
			else
			{
				avl_rotation_RL(AVL);
				avl_parent_fix(AVL);
			}
		}
		
	}
	avl_parent_fix(AVL);
	if(cod_client > (*AVL)->codigo_cliente)
	{
		avl_remove(&((*AVL)->fi_dir), cod_client);
		if(*AVL!=NULL) (*AVL)->altura_no = maior(avl_height_node((*AVL)->fi_esq), avl_height_node((*AVL)->fi_dir)) + 1;
		avl_parent_fix(AVL);
		if(avl_fab(*AVL) >= 2)
		{
			if(avl_height_node((*AVL)->fi_esq->fi_dir) <= avl_height_node((*AVL)->fi_esq->fi_esq))
			{
				avl_rotation_LL(AVL);
				avl_parent_fix(AVL);
			}
			else
			{
				avl_rotation_LR(AVL);
				avl_parent_fix(AVL);
			}
		}
		
	}
	avl_parent_fix(AVL);
	if(cod_client == (*AVL)->codigo_cliente)
	{
		if( (*AVL)->fi_esq == NULL || (*AVL)->fi_dir == NULL )
		{
			arvore *aux;
			aux = (*AVL);
			if((*AVL)->fi_esq != NULL)
			{
				(*AVL)->fi_esq->pai = (*AVL)->pai;
				*AVL = (*AVL)->fi_esq;
				avl_parent_fix(AVL);
			}
			else
			{
				if((*AVL)->fi_dir != NULL) (*AVL)->fi_dir->pai = aux->pai;	
				*AVL = (*AVL)->fi_dir;
			}
			free(aux);	
			avl_parent_fix(AVL);
			if(*AVL!=NULL) (*AVL)->altura_no = maior(avl_height_node((*AVL)->fi_esq), avl_height_node((*AVL)->fi_dir)) + 1;
		}
		else
		{
			arvore *tmp = avl_search_lower((*AVL)->fi_dir);
			(*AVL)->codigo_cliente = tmp->codigo_cliente;
			(*AVL)->qtd_op = tmp->qtd_op;
			(*AVL)->saldo = tmp->saldo;
			avl_remove(&((*AVL)->fi_dir), (*AVL)->codigo_cliente);
			if(*AVL!=NULL) (*AVL)->altura_no = maior(avl_height_node((*AVL)->fi_esq), avl_height_node((*AVL)->fi_dir)) + 1;
			avl_parent_fix(AVL);
			if(avl_fab(*AVL) >= 2)
			{
				if(avl_height_node((*AVL)->fi_esq->fi_dir) <= avl_height_node((*AVL)->fi_esq->fi_esq))
					avl_rotation_LL(AVL);
				else
					avl_rotation_LR(AVL);
			}
			if(*AVL!=NULL) (*AVL)->altura_no = maior(avl_height_node((*AVL)->fi_esq), avl_height_node((*AVL)->fi_dir)) + 1;
		}
		avl_parent_fix(AVL);
		return;
	}
	avl_parent_fix(AVL);
	if(*AVL!=NULL) (*AVL)->altura_no = maior(avl_height_node((*AVL)->fi_esq), avl_height_node((*AVL)->fi_dir)) + 1;	
}


void removeFIRST(arvore **AVL){ // Remove Ponteiro que aponta para a raiz da Árvore
	free((*AVL)->pai);
}

void freeNode(arvore **AVL){ // Libera nó
	free(*AVL);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// FUNÇÕES DE ROTAÇÃO ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void avl_rotation_LL (arvore **AVL){ // Rotação Simples à Direita (a.k.a Left Left Rotation)
	arvore *tmp;
	tmp = (*AVL)->fi_esq;
	(*AVL)->fi_esq = tmp->fi_dir;
	tmp->fi_dir = (*AVL);
	tmp->pai=(*AVL)->pai;
	(*AVL)->altura_no = maior( avl_height_node((*AVL)->fi_esq), avl_height_node((*AVL)->fi_dir)) + 1;
	tmp->altura_no = maior( avl_height_node((tmp)->fi_esq), (*AVL)->altura_no) + 1;

	*AVL = tmp;

}

void avl_rotation_RR (arvore **AVL){ // Rotação Simples à Esquerda (a.k.a Right Right Rotation)
	arvore *tmp;
	tmp = (*AVL)->fi_dir;
	(*AVL)->fi_dir = tmp->fi_esq;
	tmp->fi_esq = (*AVL);
	tmp->pai = (*AVL)->pai;
	(*AVL)->altura_no = maior( avl_height_node((*AVL)->fi_esq), avl_height_node((*AVL)->fi_dir)) + 1;
	tmp->altura_no = maior( avl_height_node((tmp)->fi_dir), (*AVL)->altura_no ) + 1;

	*AVL = tmp;
	
}

void avl_rotation_LR (arvore **AVL){ // Rotação Dupla à Direita (a.k.a Left Right Rotation)
	avl_rotation_RR(&(*AVL)->fi_esq);
	avl_rotation_LL(AVL);	
}

void avl_rotation_RL (arvore **AVL){ // Rotação Dupla à Esquerda (a.k.a Right Left Rotation)
	avl_rotation_LL(&(*AVL)->fi_dir);
	avl_rotation_RR(AVL);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// FUNÇÕES DE BUSCA DE NÓS //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

int avl_search_node(arvore *AVL, long long int value){ // Procura nó e retorna resultado  
	if(AVL->altura_no==0)
		return 0;
	
	arvore *tmp = AVL;
	while(tmp != NULL)
	{
		if(value == tmp->codigo_cliente)
			return 1;
		
		if(value > tmp->codigo_cliente)
			tmp = tmp->fi_dir;
		else
			tmp = tmp->fi_esq;
	}
	return 0;
}

int avl_height_node(arvore *AVL){ // Altura do nó
	if(AVL == NULL || AVL->altura_no == 0) return 0;
	return AVL->altura_no;
}

int avl_fab(arvore *AVL){ // Fator de Balanceamento do nó
	return labs(avl_height_node(AVL->fi_esq) - avl_height_node(AVL->fi_dir));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// FUNÇÕES GET ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

int getHeight(arvore *AVL){ // Retorna altura do nó
	if(AVL == NULL || AVL->altura_no==0) return 0;
	return AVL->altura_no;
}

int getCode(arvore *AVL){ // Retorna codigo_cliente do nó
	if(AVL == NULL || AVL->altura_no==0) return 0;
	return AVL->codigo_cliente;
}

int getLevel(arvore *AVL){ // Retorna nível do nó [Usado Somente em nó RAIZ da árvore e seu "pai"]
	if(AVL == NULL || AVL->altura_no==0) return 0;
	return AVL->nivel_no;
}

int getOp(arvore *AVL){ // Retorna quantidade de operações do nó
	if(AVL == NULL || AVL->altura_no==0) return 0;
	return AVL->qtd_op;
}

long long int getSaldo(arvore *AVL){ // Retorna saldo do nó
	if(AVL == NULL || AVL->altura_no==0) return 0;
	return AVL->saldo;
}

int getLowerCode(arvore *AVL){ // Retorna codigo_cliente do menor nó
	arvore *n1 = AVL;
	arvore *n2 = AVL->fi_esq;
	while(n2!=NULL)
	{
		n1 = n2;
		n2 = n2->fi_esq;
	}
	return n1->codigo_cliente;
}

int getClients(arvore **AVL){ // Retorna quantidade de clientes
	if((*AVL)==NULL || (*AVL)->altura_no==0) return 0;
	int cont = 1;

	if((*AVL)->fi_esq != NULL) cont += getClients(&(*AVL)->fi_esq);
	if((*AVL)->fi_dir != NULL) cont += getClients(&(*AVL)->fi_dir);
	
	return cont;
}

arvore *getEsq(arvore *AVL){ // Retorna nó à esquerda (fi_esq)
	if(AVL == NULL || AVL->altura_no==0) return 0;
	return AVL->fi_esq;
}

arvore *getDir(arvore *AVL){ // Retorna nó à direita (fi_dir)
	if(AVL == NULL || AVL->altura_no==0) return 0;
		return AVL->fi_dir;
}

arvore* getFather(arvore *AVL){ // Retorna nó pai
	if(AVL == NULL || AVL->altura_no==0) return 0;
	return AVL->pai;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// FUNÇÕES AUXILIARES ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

int maior(int a, int b){ // Retorna maior valor
	if (a>b)
		return a;
	return b;
}

void avl_parent_fix(arvore **AVL){ // Atualiza os nós pais dos elementos da Árvore
	if(*AVL==NULL)
		return;

	if((*AVL)->fi_esq != NULL)
		(*AVL)->fi_esq->pai = *AVL;
	avl_parent_fix(&((*AVL)->fi_esq));
	if((*AVL)->fi_dir != NULL)
		(*AVL)->fi_dir->pai = *AVL;
	avl_parent_fix(&((*AVL)->fi_dir));
}

arvore* avl_search_lower(arvore *AVL){ // Retorna menor nó
	arvore *n1 = AVL;
	arvore *n2 = AVL->fi_esq;
	while(n2!=NULL)
	{
		n1 = n2;
		n2 = n2->fi_esq;
	}
	return n1;
}

