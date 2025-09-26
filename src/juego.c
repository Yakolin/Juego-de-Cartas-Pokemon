#include "juego.h"
#include "lista.h"
#include "tipo.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "adversario.h"
#include "pokemon.h"
#include "ataque.h"

typedef struct jugador {
	lista_t *pokemones;
	int puntos;
	char ataques_disponibles[9][20];
	int indice_ataques;
} jugador_t;

struct juego {
	informacion_pokemon_t *info_pokemon;
	lista_t *pokemones_disponibles;
	jugador_t *jugador_1;
	jugador_t *jugador_2;
	size_t rondas;
};
//Quita un ataque del vector ataques disponible del jugador
void quitar_ataques(char ataques_disponibles[9][20], char *nombre_a_buscar)
{
	for (int i = 0; i < 9; i++) {
		if (strcmp(ataques_disponibles[i], nombre_a_buscar) == 0) {
			strcpy(ataques_disponibles[i], "\0");
			return;
		}
	}
}
//Busca un ataque en el vector de ataques disponiles del jugador, si lo encuentra lo devuelve, sino devuelve NULL
char *buscar_ataque_nombres(char ataques_disponibles[9][20],
			    char *nombre_a_buscar)
{
	for (int i = 0; i < 9; i++) {
		if (strcmp(ataques_disponibles[i], nombre_a_buscar) == 0)
			return nombre_a_buscar;
	}
	return NULL;
}
//Compara el nombre de una structura pokemon con un nombre, si son iguales devuelve 0, sino 1
int comparar_nombre_pokemones(void *struct_poke1, void *nombre_poke2)
{
	pokemon_t *pokemon = struct_poke1;
	if (strcmp(pokemon_nombre(pokemon), (char *)nombre_poke2) == 0)
		return 0;
	return 1;
}
//Inserta un pokemon en la lista enviada por auxiliar
void insertar_en_lista(pokemon_t *pokemon, void *auxiliar)
{
	if (!pokemon)
		return;
	lista_t *lista_pokes = auxiliar;
	if (!lista_insertar(lista_pokes, pokemon))
		return;
	return;
}
/*
	Crea un jugador_t con una lista de pokemones vacia
	En caso de error devuelve NULL
*/
jugador_t *jugador_crear()
{
	jugador_t *nuevo_jugador = calloc(1, sizeof(jugador_t));
	if (!nuevo_jugador)
		return NULL;
	nuevo_jugador->pokemones = lista_crear();
	if (!nuevo_jugador->pokemones) {
		free(nuevo_jugador);
		return NULL;
	}
	return nuevo_jugador;
}
//Destruye un jugador
void jugador_destruir(jugador_t *jugador)
{
	if (jugador->pokemones)
		lista_destruir(jugador->pokemones);
	free(jugador);
}
juego_t *juego_crear()
{
	juego_t *nuevo_juego = calloc(1, sizeof(juego_t));
	if (!nuevo_juego)
		return NULL;
	nuevo_juego->jugador_1 = jugador_crear();
	if (!nuevo_juego->jugador_1) {
		free(nuevo_juego);
		return NULL;
	}
	nuevo_juego->jugador_2 = jugador_crear();
	if (!nuevo_juego->jugador_2) {
		free(nuevo_juego->jugador_1);
		free(nuevo_juego);
		return NULL;
	}
	nuevo_juego->pokemones_disponibles = lista_crear();
	if (!nuevo_juego->pokemones_disponibles) {
		free(nuevo_juego->jugador_1);
		free(nuevo_juego->jugador_2);
		free(nuevo_juego);
		return NULL;
	}
	nuevo_juego->rondas = 9;
	return nuevo_juego;
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo)
{
	informacion_pokemon_t *nueva_info = pokemon_cargar_archivo(archivo);
	if (!nueva_info)
		return ERROR_GENERAL;
	if (pokemon_cantidad(nueva_info) >= 4) {
		juego->info_pokemon = nueva_info;
		return TODO_OK;
	}
	if (pokemon_cantidad(nueva_info) < 4) {
		pokemon_destruir_todo(nueva_info);
		return POKEMON_INSUFICIENTES;
	}
	pokemon_destruir_todo(nueva_info);
	return ERROR_GENERAL;
}

lista_t *juego_listar_pokemon(juego_t *juego)
{
	if (!juego)
		return NULL;
	if (!juego->info_pokemon || !juego->pokemones_disponibles)
		return NULL;
	con_cada_pokemon(juego->info_pokemon, insertar_en_lista,
			 juego->pokemones_disponibles);
	return juego->pokemones_disponibles;
}
/*
	Llena el vector ataques_disponibles del jugador
*/
void llenar_jugador_ataques(const struct ataque *ataque, void *j_v)
{
	jugador_t *jugador = j_v;
	strcpy(jugador->ataques_disponibles[jugador->indice_ataques],
	       ataque->nombre);
	jugador->indice_ataques++;
}
JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
				       const char *nombre1, const char *nombre2,
				       const char *nombre3)
{
	if (!juego)
		return ERROR_GENERAL;
	if (strcmp(nombre1, nombre2) == 0 || strcmp(nombre1, nombre3) == 0 ||
	    strcmp(nombre2, nombre3) == 0)
		return POKEMON_REPETIDO;
	pokemon_t *pokemones[3];

	const char *nombre_actual = nombre1;

	for (int i = 0; i < 3; i++) {
		pokemones[i] = lista_buscar_elemento(
			juego->pokemones_disponibles, comparar_nombre_pokemones,
			(void *)nombre_actual);
		if (!pokemones[i])
			return POKEMON_INEXISTENTE;
		nombre_actual = nombre2;
		if (i == 1)
			nombre_actual = nombre3;
	}
	jugador_t *jugador_principal = juego->jugador_1;
	jugador_t *jugador_secundario = juego->jugador_2;
	nombre_actual = nombre1;
	if (jugador == JUGADOR2) {
		jugador_principal = juego->jugador_2;
		jugador_secundario = juego->jugador_1;
	}
	for (int i = 0; i < 3; i++) {
		if (!lista_insertar(jugador_principal->pokemones, pokemones[i]))
			return ERROR_GENERAL;
		con_cada_ataque(pokemones[i], llenar_jugador_ataques,
				jugador_principal);
		nombre_actual = nombre2;
		if (i == 1) {
			nombre_actual = nombre3;
			jugador_principal = jugador_secundario;
		}
	}
	return TODO_OK;
}
//Calcula el valor de los puntos causados por el resultado del ataque, en algun error devolvera 0
int calcular_puntos(RESULTADO_ATAQUE resultado_ataque,
		    const struct ataque *ataque)
{
	if (!ataque || resultado_ataque == ATAQUE_ERROR)
		return 0;
	if (resultado_ataque == ATAQUE_EFECTIVO) {
		return (int)(ataque->poder) * 3;
	}
	if (resultado_ataque == ATAQUE_INEFECTIVO) {
		int resultado = (int)(ataque->poder) / 2;
		if ((int)(ataque->poder) % 2 != 0) {
			resultado += 1;
		}
		return resultado;
	}
	if (resultado_ataque == ATAQUE_REGULAR) {
		return (int)(ataque->poder);
	}
	return 0;
}

//Analiza la tabla de tipos del juego, devolviendo el resultado del ataque contra el atacado
RESULTADO_ATAQUE
analizar_efectividad_ataque(const struct ataque *ataque_del_jugador,
			    pokemon_t *pokemon_enemigo)
{
	if (!ataque_del_jugador || !pokemon_enemigo)
		return ATAQUE_ERROR;
	if (ataque_del_jugador->tipo == FUEGO &&
	    pokemon_tipo(pokemon_enemigo) == PLANTA)
		return ATAQUE_EFECTIVO;
	if (ataque_del_jugador->tipo == FUEGO &&
	    pokemon_tipo(pokemon_enemigo) == AGUA)
		return ATAQUE_INEFECTIVO;

	if (ataque_del_jugador->tipo == PLANTA &&
	    pokemon_tipo(pokemon_enemigo) == ROCA)
		return ATAQUE_EFECTIVO;
	if (ataque_del_jugador->tipo == PLANTA &&
	    pokemon_tipo(pokemon_enemigo) == FUEGO)
		return ATAQUE_INEFECTIVO;

	if (ataque_del_jugador->tipo == ROCA &&
	    pokemon_tipo(pokemon_enemigo) == ELECTRICO)
		return ATAQUE_EFECTIVO;
	if (ataque_del_jugador->tipo == ROCA &&
	    pokemon_tipo(pokemon_enemigo) == PLANTA)
		return ATAQUE_INEFECTIVO;

	if (ataque_del_jugador->tipo == ELECTRICO &&
	    pokemon_tipo(pokemon_enemigo) == AGUA)
		return ATAQUE_EFECTIVO;
	if (ataque_del_jugador->tipo == ELECTRICO &&
	    pokemon_tipo(pokemon_enemigo) == ROCA)
		return ATAQUE_INEFECTIVO;

	if (ataque_del_jugador->tipo == AGUA &&
	    pokemon_tipo(pokemon_enemigo) == FUEGO)
		return ATAQUE_EFECTIVO;
	if (ataque_del_jugador->tipo == AGUA &&
	    pokemon_tipo(pokemon_enemigo) == ELECTRICO)
		return ATAQUE_INEFECTIVO;

	return ATAQUE_REGULAR;
}
resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
				     jugada_t jugada_jugador2)
{
	resultado_jugada_t resultado;
	resultado.jugador1 = ATAQUE_ERROR;
	resultado.jugador2 = ATAQUE_ERROR;

	pokemon_t *pokemon_jugador1 = lista_buscar_elemento(
		juego->jugador_1->pokemones, comparar_nombre_pokemones,
		jugada_jugador1.pokemon);
	if (!pokemon_jugador1)
		return resultado;
	const struct ataque *ataque_jugador1 =
		pokemon_buscar_ataque(pokemon_jugador1, jugada_jugador1.ataque);
	if (!ataque_jugador1 ||
	    !buscar_ataque_nombres(juego->jugador_1->ataques_disponibles,
				   jugada_jugador1.ataque))
		return resultado;
	pokemon_t *pokemon_jugador2 = lista_buscar_elemento(
		juego->jugador_2->pokemones, comparar_nombre_pokemones,
		jugada_jugador2.pokemon);
	if (!pokemon_jugador2)
		return resultado;
	const struct ataque *ataque_jugador2 =
		pokemon_buscar_ataque(pokemon_jugador2, jugada_jugador2.ataque);
	if (!ataque_jugador2 ||
	    !buscar_ataque_nombres(juego->jugador_2->ataques_disponibles,
				   jugada_jugador2.ataque))
		return resultado;
	resultado.jugador1 =
		analizar_efectividad_ataque(ataque_jugador1, pokemon_jugador2);
	if (!resultado.jugador1)
		return resultado;
	resultado.jugador2 =
		analizar_efectividad_ataque(ataque_jugador2, pokemon_jugador1);
	if (!resultado.jugador2)
		return resultado;
	quitar_ataques(juego->jugador_1->ataques_disponibles,
		       jugada_jugador1.ataque);
	quitar_ataques(juego->jugador_2->ataques_disponibles,
		       jugada_jugador2.ataque);
	juego->jugador_1->puntos +=
		calcular_puntos(resultado.jugador1, ataque_jugador1);
	juego->jugador_2->puntos +=
		calcular_puntos(resultado.jugador2, ataque_jugador2);
	juego->rondas--;
	return resultado;
}
int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador)
{
	if (!juego)
		return 0;
	if (jugador == JUGADOR1)
		return juego->jugador_1->puntos;
	else
		return juego->jugador_2->puntos;
}
bool juego_finalizado(juego_t *juego)
{
	if (!juego)
		return true;
	if (juego->rondas == 0)
		return true;
	return false;
}
void juego_destruir(juego_t *juego)
{
	if (!juego)
		return;
	if (juego->info_pokemon)
		pokemon_destruir_todo(juego->info_pokemon);
	if (juego->pokemones_disponibles)
		lista_destruir(juego->pokemones_disponibles);
	if (juego->jugador_1)
		jugador_destruir(juego->jugador_1);
	if (juego->jugador_2)
		jugador_destruir(juego->jugador_2);
	free(juego);
}
