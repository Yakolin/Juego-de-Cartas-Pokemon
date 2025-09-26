#include "lista.h"
#include "hash.h"
#include "juego.h"
#include "adversario.h"
#include "pokemon.h"
#include "ataque.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

struct adversario {
	lista_t *todos_los_pokemones;
	lista_t *pokemones_seleccionados;
	char ataques_disponibles[9][20];
	int indice_ataques;
	jugada_t jugada_anterior_propia;
};
typedef struct almacenador_nombres {
	char nombres[3][23];
	int indice;
	adversario_t *adversario;
} nombres_t;

//Quita un ataque del vector ataques disponible del jugador
void quitar_ataque(char ataques_disponibles[9][20], char *nombre_a_buscar)
{
	for (int i = 0; i < 9; i++) {
		if (strcmp(ataques_disponibles[i], nombre_a_buscar) == 0) {
			strcpy(ataques_disponibles[i], "\0");
			return;
		}
	}
}
//Busca un ataque en el vector de ataques disponiles del jugador, si lo encuentra lo devuelve, sino devuelve NULL
char *buscar_ataque_nombre(char ataques_disponibles[9][20],
			   char *nombre_a_buscar)
{
	for (int i = 0; i < 9; i++) {
		if (strcmp(ataques_disponibles[i], nombre_a_buscar) == 0)
			return nombre_a_buscar;
	}
	return NULL;
}
//Compara el nombre de una structura pokemon con un nombre, si son iguales devuelve 0, sino 1
int comparar_nombre_pokes(void *struct_poke1, void *nombre_poke2)
{
	pokemon_t *pokemon = struct_poke1;
	if (strcmp(pokemon_nombre(pokemon), (char *)nombre_poke2) == 0)
		return 0;
	return 1;
}

adversario_t *adversario_crear(lista_t *pokemon)
{
	if (!pokemon)
		return NULL;
	adversario_t *nuevo_adversario = calloc(1, sizeof(adversario_t));
	if (!nuevo_adversario)
		return NULL;
	nuevo_adversario->pokemones_seleccionados = lista_crear();
	if (!nuevo_adversario->pokemones_seleccionados) {
		free(nuevo_adversario);
		return NULL;
	}
	nuevo_adversario->todos_los_pokemones = pokemon;
	return nuevo_adversario;
}
//Recibe un vector de enteros y devuelve en cada posicion un numero aleatorio con el valor medio
void conseguir_tres_numeros_aleatorios(int vector[3], int valor_medio)
{
	for (int i = 0; i < 3; i++) {
		vector[i] = rand() % valor_medio;
		for (int j = 0; j < i; j++)
			if (vector[j] == vector[i])
				i--;
	}
}
//Llena el vector ataques disponibles del adversario
void llenar_adversario_ataques(const struct ataque *ataque, void *a_v)
{
	adversario_t *adversario = a_v;
	strcpy(adversario->ataques_disponibles[adversario->indice_ataques],
	       ataque->nombre);
	adversario->indice_ataques++;
}
bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1,
				    char **nombre2, char **nombre3)
{
	srand((unsigned int)time(NULL));
	if (!adversario)
		return false;
	int indices[3];
	conseguir_tres_numeros_aleatorios(
		indices, (int)lista_tamanio(adversario->todos_los_pokemones));
	pokemon_t *pokemon_actual;
	for (int i = 0; i < 2; i++) {
		pokemon_actual = lista_elemento_en_posicion(
			adversario->todos_los_pokemones, (size_t)indices[i]);
		if (!pokemon_actual)
			return false;
		if (!lista_insertar(adversario->pokemones_seleccionados,
				    pokemon_actual))
			return false;
		con_cada_ataque(pokemon_actual, llenar_adversario_ataques,
				adversario);
		if (i == 0)
			*nombre1 = (char *)pokemon_nombre(pokemon_actual);
		else
			*nombre2 = (char *)pokemon_nombre(pokemon_actual);
	}
	pokemon_actual = lista_elemento_en_posicion(
		adversario->todos_los_pokemones, (size_t)indices[2]);
	if (!pokemon_actual)
		return false;
	*nombre3 = (char *)pokemon_nombre(pokemon_actual);
	return true;
}
bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1,
				     char *nombre2, char *nombre3)
{
	if (!adversario)
		return false;
	pokemon_t *pokemon_actual;
	pokemon_actual = lista_buscar_elemento(adversario->todos_los_pokemones,
					       comparar_nombre_pokes,
					       (void *)nombre3);
	if (!pokemon_actual)
		return false;
	if (!lista_insertar(adversario->pokemones_seleccionados,
			    pokemon_actual))
		return false;
	con_cada_ataque(pokemon_actual, llenar_adversario_ataques, adversario);
	return true;
}
/*
	Llena un vector de strs con el nombre de cada uno de los ataques de 1 pokemon
*/
void llenar_vector_con_nombres_ataque(const struct ataque *ataque, void *aux)
{
	nombres_t *nombres_aux = aux;
	if (buscar_ataque_nombre(nombres_aux->adversario->ataques_disponibles,
				 (char *)ataque->nombre)) {
		strcpy(nombres_aux->nombres[nombres_aux->indice],
		       ataque->nombre);
		nombres_aux->indice++;
	}
}
//Busca un ataque disponible en una estructura nombres_t.
const struct ataque *buscar_ataque_disponible(pokemon_t *pokemon,
					      nombres_t nombres_ataques)
{
	int indice_ataque = rand() % nombres_ataques.indice;
	const struct ataque *ataque = pokemon_buscar_ataque(
		pokemon, nombres_ataques.nombres[indice_ataque]);
	return ataque;
}
jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	jugada_t j = { .ataque = "", .pokemon = "" };
	nombres_t nombres_ataques;
	nombres_ataques.adversario = adversario;
	pokemon_t *pokemon;
	const struct ataque *ataque = NULL;
	nombres_ataques.indice = 0;
	int indice_pokemon = 0;
	bool pokemon_ataque_correcto = false;

	while (!pokemon_ataque_correcto) {
		indice_pokemon = rand() % 3;
		pokemon = lista_elemento_en_posicion(
			adversario->pokemones_seleccionados,
			(size_t)indice_pokemon);
		con_cada_ataque(pokemon, llenar_vector_con_nombres_ataque,
				&nombres_ataques);
		if (nombres_ataques.indice != 0)
			ataque = buscar_ataque_disponible(pokemon,
							  nombres_ataques);
		if (ataque)
			pokemon_ataque_correcto = true;
	}
	strcpy(j.pokemon, pokemon_nombre(pokemon));
	strcpy(j.ataque, ataque->nombre);
	strcpy(adversario->jugada_anterior_propia.pokemon,
	       pokemon_nombre(pokemon));
	strcpy(adversario->jugada_anterior_propia.ataque, ataque->nombre);
	return j;
}
void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
	if (!a)
		return;
	quitar_ataque(a->ataques_disponibles, a->jugada_anterior_propia.ataque);
}
void adversario_destruir(adversario_t *adversario)
{
	if (!adversario)
		return;
	if (adversario->pokemones_seleccionados)
		lista_destruir(adversario->pokemones_seleccionados);
	free(adversario);
}
