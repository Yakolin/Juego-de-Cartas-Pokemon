#include "lista.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	size_t cantidad_elementos;
	nodo_t *nodo_inicio;
	nodo_t *nodo_final;
};

struct lista_iterador {
	lista_t *lista;
	nodo_t *actual;
};
//Devuelve el nodo de la posicio pedida. En caso de error devuelve NULL
nodo_t *nodo_en_posicion(nodo_t *nodo_actual, size_t posicion)
{
	size_t contador = 0;
	while (nodo_actual != NULL) {
		if (contador == posicion)
			return nodo_actual;
		nodo_actual = nodo_actual->siguiente;
		contador++;
	}
	return NULL;
}

//Devuelve un nodo_t* con el elemento cargado o NULL si fallo al guardarlo en memoria
nodo_t *crear_nuevo_nodo(void *elemento)
{
	nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));
	if (!nuevo_nodo)
		return NULL;
	nuevo_nodo->elemento = elemento;
	return nuevo_nodo;
}
lista_t *lista_crear()
{
	lista_t *nueva_lista = calloc(1, sizeof(lista_t));
	if (!nueva_lista)
		return NULL;
	return nueva_lista;
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (lista == NULL)
		return NULL;
	nodo_t *nuevo_nodo = crear_nuevo_nodo(elemento);
	if (nuevo_nodo == NULL)
		return NULL;
	if (lista->cantidad_elementos == 0)
		lista->nodo_inicio = nuevo_nodo;
	else {
		lista->nodo_final->siguiente = nuevo_nodo;
	}
	lista->nodo_final = nuevo_nodo;
	lista->cantidad_elementos++;
	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (lista == NULL)
		return NULL;
	if (posicion >= lista->cantidad_elementos) {
		return lista_insertar(lista, elemento);
	}
	nodo_t *nuevo_nodo = crear_nuevo_nodo(elemento);
	if (nuevo_nodo == NULL)
		return NULL;

	nodo_t *nodo_actual = lista->nodo_inicio;
	if (posicion == 0) {
		lista->nodo_inicio = nuevo_nodo;
		nuevo_nodo->siguiente = nodo_actual;
		lista->cantidad_elementos++;
		return lista;
	}
	nodo_actual = nodo_en_posicion(nodo_actual, posicion - 1);
	if (!nodo_actual) {
		free(nuevo_nodo);
		return NULL;
	}
	nuevo_nodo->siguiente = nodo_actual->siguiente;
	nodo_actual->siguiente = nuevo_nodo;
	lista->cantidad_elementos++;
	return lista;
}
void *lista_quitar(lista_t *lista)
{
	if (lista == NULL || lista->nodo_inicio == NULL)
		return NULL;

	void *elemento_de_nodo_eliminado = NULL;

	if (lista->nodo_inicio->siguiente == NULL) {
		elemento_de_nodo_eliminado = lista->nodo_inicio->elemento;
		free(lista->nodo_inicio);
		lista->nodo_inicio = NULL;
		lista->nodo_final = NULL;
		lista->cantidad_elementos--;
		return elemento_de_nodo_eliminado;
	}

	nodo_t *nodo_actual = lista->nodo_inicio;
	nodo_actual = nodo_en_posicion(nodo_actual, lista_tamanio(lista) - 2);
	if (!nodo_actual)
		return NULL;
	elemento_de_nodo_eliminado = lista->nodo_final->elemento;
	nodo_actual->siguiente = NULL;
	free(lista->nodo_final);
	lista->nodo_final = nodo_actual;
	lista->cantidad_elementos--;
	return elemento_de_nodo_eliminado;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (lista == NULL)
		return NULL;
	void *elemento_de_nodo_eliminado = NULL;
	nodo_t *nodo_actual = lista->nodo_inicio;
	nodo_t *nodo_a_eliminar = NULL;

	if (posicion >= lista_tamanio(lista))
		return lista_quitar(lista);

	if (posicion == 0) {
		elemento_de_nodo_eliminado = lista->nodo_inicio->elemento;
		nodo_actual = lista->nodo_inicio->siguiente;
		free(lista->nodo_inicio);
		lista->nodo_inicio = nodo_actual;
		lista->cantidad_elementos--;
		return elemento_de_nodo_eliminado;
	}
	nodo_actual = nodo_en_posicion(nodo_actual, posicion - 1);
	if (!nodo_actual)
		return NULL;
	nodo_a_eliminar = nodo_actual->siguiente;
	nodo_actual->siguiente = nodo_a_eliminar->siguiente;
	elemento_de_nodo_eliminado = nodo_a_eliminar->elemento;
	free(nodo_a_eliminar);
	lista->cantidad_elementos--;
	return elemento_de_nodo_eliminado;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (lista == NULL) {
		return NULL;
	}
	if (posicion > lista_tamanio(lista))
		return NULL;
	size_t contador = 0;
	nodo_t *nodo_actual = lista->nodo_inicio;
	while (nodo_actual != NULL) {
		if (contador == posicion) {
			return nodo_actual->elemento;
		}
		nodo_actual = nodo_actual->siguiente;
		contador++;
	}
	return NULL;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (lista == NULL || comparador == NULL)
		return NULL;
	nodo_t *nodo_actual = lista->nodo_inicio;
	while (nodo_actual != NULL) {
		if (comparador(nodo_actual->elemento, contexto) == 0) {
			return nodo_actual->elemento;
		}
		nodo_actual = nodo_actual->siguiente;
	}
	return NULL;
}

void *lista_primero(lista_t *lista)
{
	if (lista == NULL || lista->cantidad_elementos == 0)
		return NULL;
	return lista->nodo_inicio->elemento;
}
void *lista_ultimo(lista_t *lista)
{
	if (lista == NULL || lista->cantidad_elementos == 0)
		return NULL;
	return lista->nodo_final->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if (lista == NULL || lista->cantidad_elementos == 0)
		return true;
	return false;
}

size_t lista_tamanio(lista_t *lista)
{
	if (lista == NULL)
		return 0;
	return (size_t)lista->cantidad_elementos;
}

void lista_destruir(lista_t *lista)
{
	if (lista == NULL) {
		return;
	}
	nodo_t *nodo_aux = lista->nodo_inicio;
	nodo_t *nodo_siguiente;

	while (nodo_aux != NULL) {
		nodo_siguiente = nodo_aux->siguiente;
		free(nodo_aux);
		nodo_aux = nodo_siguiente;
	}
	free(lista);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (lista == NULL)
		return;

	nodo_t *nodo_actual = lista->nodo_inicio;
	nodo_t *nodo_siguiente;

	while (nodo_actual != NULL) {
		nodo_siguiente = nodo_actual->siguiente;
		if (funcion != NULL)
			funcion(nodo_actual->elemento);
		free(nodo_actual);
		nodo_actual = nodo_siguiente;
	}
	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (lista == NULL)
		return NULL;

	lista_iterador_t *nuevo_iterador = calloc(1, sizeof(lista_iterador_t));
	if (!nuevo_iterador)
		return NULL;
	nuevo_iterador->actual = lista->nodo_inicio;
	nuevo_iterador->lista = lista;
	return nuevo_iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (iterador == NULL || iterador->lista->cantidad_elementos == 0 ||
	    iterador->actual == NULL)
		return false;
	return true;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!iterador || lista_vacia(iterador->lista) || !iterador->actual)
		return false;
	if (iterador->actual->siguiente == NULL) {
		iterador->actual = iterador->actual->siguiente;
		return false;
	}
	iterador->actual = iterador->actual->siguiente;
	return true;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (iterador == NULL || lista_vacia(iterador->lista) ||
	    iterador->actual == NULL)
		return NULL;
	return iterador->actual->elemento;
}
void lista_iterador_destruir(lista_iterador_t *iterador)
{
	if (iterador == NULL)
		return;
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (lista == NULL || funcion == NULL || lista_vacia(lista))
		return 0;
	nodo_t *nodo_actual = lista->nodo_inicio;
	size_t elementos_iterados = 0;
	bool puedo_continuar = true;
	while (puedo_continuar && nodo_actual != NULL) {
		if (!funcion(nodo_actual->elemento, contexto))
			puedo_continuar = false;
		elementos_iterados++;
		nodo_actual = nodo_actual->siguiente;
	}
	return elementos_iterados;
}
