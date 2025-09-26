#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "hash.h"

#define FACTOR_CARGA_MAXIMO 0.7

typedef struct nodos_hash {
	struct nodos_hash *siguiente;
	char *clave;
	void *elemento;
} nodos_hash_t;

struct hash {
	struct nodos_hash **tabla;
	size_t capacidad;
	size_t cantidad;
};

unsigned long funcion_hash(unsigned char *clave)
{
	unsigned long valor = 5381;
	int c;

	while ((c = *clave++))
		valor = ((valor << 5) + valor) + (unsigned long)c;

	return valor;
}
unsigned long calcular_indice(const char *clave, size_t capacidad)
{
	return funcion_hash((unsigned char *)clave) % capacidad;
}
bool funcion_insertar(const char *clave, void *valor, void *aux)
{
	return hash_insertar((hash_t *)aux, clave, valor, NULL) ==
	       (hash_t *)aux;
}
hash_t *rehash_hash(hash_t *hash)
{
	hash_t *nuevo_hash = hash_crear(hash->capacidad * 2);
	if (!nuevo_hash)
		return NULL;
	hash_con_cada_clave(hash, funcion_insertar, nuevo_hash);
	hash_t aux = *hash;
	*hash = *nuevo_hash;
	*nuevo_hash = aux;
	hash_destruir(nuevo_hash);
	return hash;
}

nodos_hash_t *crear_nodo(const char *clave, void *elemento)
{
	nodos_hash_t *nodo = (nodos_hash_t *)malloc(sizeof(nodos_hash_t));
	if (!nodo)
		return NULL;
	nodo->siguiente = NULL;
	char *clave_nueva = malloc(strlen(clave) + 1);
	if (!clave_nueva) {
		free(nodo);
		return NULL;
	}
	strcpy(clave_nueva, clave);
	nodo->clave = clave_nueva;
	nodo->elemento = elemento;
	return nodo;
}

hash_t *hash_crear(size_t capacidad)
{
	if (capacidad < 3)
		capacidad = 3;
	hash_t *nueva_tabla_hash = calloc(1, sizeof(hash_t));
	if (!nueva_tabla_hash)
		return NULL;
	nueva_tabla_hash->tabla =
		calloc(capacidad, sizeof(struct nodos_hash *));
	if (!nueva_tabla_hash->tabla) {
		free(nueva_tabla_hash->tabla);
		free(nueva_tabla_hash);
		return NULL;
	}
	nueva_tabla_hash->capacidad = capacidad;
	return nueva_tabla_hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave)
		return NULL;
	double factor_carga = (double)hash->cantidad / (double)hash->capacidad;
	if (factor_carga > FACTOR_CARGA_MAXIMO)
		if (!rehash_hash(hash))
			return NULL;

	unsigned long indice = calcular_indice(clave, hash->capacidad);

	nodos_hash_t *nuevo_nodo = crear_nodo(clave, elemento);
	if (!nuevo_nodo)
		return NULL;
	if (hash->tabla[indice] == NULL) {
		hash->tabla[indice] = nuevo_nodo;
		hash->cantidad++;
		if (anterior)
			*anterior = NULL;
		return hash;
	}
	nodos_hash_t *actual = hash->tabla[indice];
	while (actual) {
		if (strcmp(actual->clave, clave) == 0) {
			if (anterior)
				*anterior = actual->elemento;
			actual->elemento = elemento;
			free(nuevo_nodo->clave);
			free(nuevo_nodo);
			return hash;
		}
		actual = actual->siguiente;
	}
	nuevo_nodo->siguiente = hash->tabla[indice];
	hash->tabla[indice] = nuevo_nodo;
	hash->cantidad++;
	if (anterior)
		*anterior = NULL;
	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;
	unsigned long indice = calcular_indice(clave, hash->capacidad);
	void *elemento_eliminado = NULL;
	nodos_hash_t *actual = hash->tabla[indice];
	nodos_hash_t *anterior = NULL;

	while (actual) {
		if (strcmp(actual->clave, clave) == 0) {
			elemento_eliminado = actual->elemento;
			if (anterior) {
				anterior->siguiente = actual->siguiente;
				free(actual->clave);
				free(actual);
			} else {
				nodos_hash_t *siguiente_actual =
					actual->siguiente;
				free(actual->clave);
				free(actual);
				hash->tabla[indice] = siguiente_actual;
			}
			hash->cantidad--;
			return elemento_eliminado;
		}
		anterior = actual;
		actual = actual->siguiente;
	}
	return elemento_eliminado;
}

//BUSCA EL ELEMENTO CON LA CLAVE PASADA POR PARAMETRO , Si esta devuelve el elemento, sino devuelve NULL
void *hash_buscar_elemento(hash_t *hash, const char *clave)
{
	unsigned long indice = calcular_indice(clave, hash->capacidad);

	if (hash->tabla[indice] == NULL)
		return NULL;
	nodos_hash_t *actual = hash->tabla[indice];
	while (actual) {
		if (strcmp(actual->clave, clave) == 0) {
			return actual->elemento;
		}
		actual = actual->siguiente;
	}
	return NULL;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;
	return hash_buscar_elemento(hash, clave);
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return false;
	if (hash_buscar_elemento(hash, clave))
		return true;
	return false;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;
	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	if (!hash)
		return;
	for (int i = 0; i < hash->capacidad; i++) {
		nodos_hash_t *actual = hash->tabla[i];
		while (actual) {
			nodos_hash_t *siguiente = actual->siguiente;
			free(actual->clave);
			free(actual);
			actual = siguiente;
		}
	}
	free(hash->tabla);
	free(hash);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;

	for (int i = 0; i < hash->capacidad; i++) {
		nodos_hash_t *actual = hash->tabla[i];
		while (actual) {
			nodos_hash_t *siguiente = actual->siguiente;
			if (destructor)
				destructor(actual->elemento);
			free(actual->clave);
			free(actual);
			actual = siguiente;
		}
	}

	free(hash->tabla);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t n = 0;
	if (!hash || !f)
		return n;
	for (int i = 0; i < hash->capacidad; i++) {
		nodos_hash_t *actual = hash->tabla[i];
		while (actual) {
			n++;
			if (!f(actual->clave, actual->elemento, aux))
				return n;

			actual = actual->siguiente;
		}
	}
	return n;
}
