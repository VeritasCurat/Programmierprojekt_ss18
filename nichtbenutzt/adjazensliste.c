/*
 * adjazensliste.c
 *
 *  Created on: 06.06.2018
 *      Author: Johannes
 */

#include "adjazensliste.h"

//void append(struct element **lst, int X, int Y){
//	struct element *neuesElement;
//	/* Zeiger auf die Einfügeposition ermitteln, d.h. bis zum Ende laufen */
//	while( *lst != NULL )
//	{
//		lst = &(*lst)->next;
//	}
//	neuesElement = malloc(sizeof(*neuesElement)); /* erzeuge ein neues Element
//	*/
//	neuesElement->X = X;
//	neuesElement->Y = Y;
//
//	neuesElement->next = NULL; /* Wichtig für das Erkennen des Listenendes
//	*/
//	*lst = neuesElement;
//}
//
//
//int search(struct element **lst, struct element *gesElement){
//	/* Zeiger auf die Einfügeposition ermitteln, d.h. bis zum Ende laufen */
//	while(1)
//	{
//		if(*lst == gesElement)return 0;
//		lst = &(*lst)->next;
//	}
//	return 1;
//}
//
//int search_nachbar(struct element **lst, struct element *gesElement){
//	/* Zeiger auf die Einfügeposition ermitteln, d.h. bis zum Ende laufen */
//	while(1)
//	{
//		if(*lst == gesElement)return 0;
//		lst = &(*lst)->nachbar;
//	}
//	return 1;
//}
//
//int delete(struct element **lst, struct element *gesElement){
//	/* Zeiger auf die Einfügeposition ermitteln, d.h. bis zum Ende laufen */
//	while(1)
//	{
//		if(&(*lst)->next == gesElement){
//			free((*lst)->next);
//			(*lst)->next = (*lst)->next->next;
//		}
//		lst = &(*lst)->next;
//	}
//	return 1;
//}
//
//int delete_nachbar(struct element **lst, struct element *gesElement){
//	/* Zeiger auf die Einfügeposition ermitteln, d.h. bis zum Ende laufen */
//	while(1)
//	{
//		if(&(*lst)->nachbar == gesElement){
//			free((*lst)->nachbar);
//			(*lst)->nachbar = (*lst)->nachbar->nachbar;
//		}
//		lst = &(*lst)->nachbar;
//	}
//	return 1;
//}
//
//void printliste(const struct element *e){
//	for( ; e != NULL ; e = e->next ){
//		printf("X: %d, Y: %d\n", e->X, e->Y);
//	}
//}
//
//int main(){
//	struct element *Liste;
//	Liste = NULL; /* init. die Liste mit NULL = leere Liste */
//	append(&Liste, 1,1); /* füge neues Element in die Liste ein */
//	append(&Liste, 2,3); /* füge neues Element in die Liste ein */
//	append(&Liste, 3,3); /* füge neues Element in die Liste ein */
//	printliste(Liste); /* zeige alle Elemente der Liste an */
//return 0;
//}

