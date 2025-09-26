#include <stdlib.h>
#include "pokemon.h"
#include "tipo.h"
#include <stdio.h>
#include "ataque.h"
#include <string.h>
#include <stdbool.h>

#define MAX_NOMBRE 20
#define MAX_ATAQUES 3
#define TIPO_NORMAL 'N'
#define TIPO_FUEGO 'F'
#define TIPO_AGUA 'A'
#define TIPO_PLANTA 'P'
#define TIPO_ELECTRICO 'E'
#define TIPO_ROCA 'R'
struct pokemon {
	char nombre[MAX_NOMBRE];
	enum TIPO tipo_del_pok;
	struct ataque ataques_pok[MAX_ATAQUES];
};

struct info_pokemon {
	struct pokemon *pokemones;
	int cantidad_pokemones;
};

void copiar_pokemon(struct pokemon *nuevo_pokemon,
		    char nombre_pokemon[MAX_NOMBRE], enum TIPO tipo_pokemon)
{
	strcpy(nuevo_pokemon->nombre, nombre_pokemon);
	nuevo_pokemon->tipo_del_pok = tipo_pokemon;
}
void cargar_ataques(struct pokemon *nuevo_pokemon,
		    char nombre_ataque[MAX_NOMBRE], enum TIPO tipo_ataque,
		    unsigned int poder, int contador_ataques)
{
	strcpy(nuevo_pokemon->ataques_pok[contador_ataques].nombre,
	       nombre_ataque);
	nuevo_pokemon->ataques_pok[contador_ataques].tipo = tipo_ataque;
	nuevo_pokemon->ataques_pok[contador_ataques].poder = poder;
}
void cargar_informacion(struct info_pokemon *nueva_informacion,
			struct pokemon *nuevo_pokemon)
{
	nueva_informacion->pokemones = realloc(
		nueva_informacion->pokemones,
		(long unsigned int)(nueva_informacion->cantidad_pokemones + 1) *
			sizeof(struct pokemon));
	nueva_informacion->pokemones[nueva_informacion->cantidad_pokemones] =
		*nuevo_pokemon;
	nueva_informacion->cantidad_pokemones++;
}
void ordenar_pokemones(informacion_pokemon_t *ip, int cantidad_pokes)
{
	struct pokemon pokemon_aux;
	int j = 0;
	for (int i = 1; i < cantidad_pokes; i++) {
		pokemon_aux = ip->pokemones[i];
		j = i - 1;
		while (j >= 0 && strcmp(ip->pokemones[j].nombre,
					pokemon_aux.nombre) > 0) {
			ip->pokemones[j + 1] = ip->pokemones[j];
			j--;
		}
		ip->pokemones[j + 1] = pokemon_aux;
	}
}
bool verificar_nombre(char *nombre)
{
	if (nombre[0] == '\0') {
		return false;
	}
	return true;
}
enum TIPO verificar_tipo(char tipo)
{
	switch (tipo) {
	case TIPO_NORMAL:
		return NORMAL;
		break;
	case TIPO_FUEGO:
		return FUEGO;
		break;
	case TIPO_AGUA:
		return AGUA;
		break;
	case TIPO_PLANTA:
		return PLANTA;
		break;
	case TIPO_ELECTRICO:
		return ELECTRICO;
		break;
	case TIPO_ROCA:
		return ROCA;
		break;
	default:
		return -1;
		break;
	}
	return true;
}

informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	if (path == NULL)
		return NULL;
	FILE *archivo = fopen(path, "r");
	char linea_archivo[500];

	char nombre[MAX_NOMBRE];
	char tipo = ' ';
	int poder = 0;
	int contador_ataques = 3;
	bool parametros_correctos = true;

	struct info_pokemon *nueva_informacion =
		calloc(1, sizeof(struct info_pokemon));

	struct pokemon *nuevo_pokemon = malloc(sizeof(struct pokemon));
	if (archivo == NULL || nueva_informacion == NULL ||
	    nuevo_pokemon == NULL) {
		free(nuevo_pokemon);
		free(nueva_informacion);
		return NULL;
	}

	while (fgets(linea_archivo, sizeof(linea_archivo), archivo) != NULL &&
	       parametros_correctos) {
		int resultado = sscanf(linea_archivo, "%[^;];%c;%i\n", nombre,
				       &tipo, &poder);
		enum TIPO nuevo_tipo = verificar_tipo(tipo);

		if (resultado == 2 && contador_ataques == 3 &&
		    verificar_nombre(nombre) && nuevo_tipo != -1) {
			copiar_pokemon(nuevo_pokemon, nombre, nuevo_tipo);
			contador_ataques = 0;

		} else if (resultado == 3 && contador_ataques < 3 &&
			   poder != 0 && verificar_nombre(nombre) &&
			   nuevo_tipo != -1) {
			cargar_ataques(nuevo_pokemon, nombre, nuevo_tipo,
				       (unsigned int)poder, contador_ataques);
			contador_ataques++;
		} else {
			parametros_correctos = false;
		}

		if (contador_ataques == 3 && parametros_correctos) {
			cargar_informacion(nueva_informacion, nuevo_pokemon);
		}
	}

	free(nuevo_pokemon);
	fclose(archivo);

	if (nueva_informacion->cantidad_pokemones == 0 &&
	    !parametros_correctos) {
		free(nueva_informacion->pokemones);
		free(nueva_informacion);
		return NULL;
	}

	return nueva_informacion;
}

pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre)
{
	if (ip == NULL || nombre == NULL) {
		return NULL;
	}
	bool pokemon_encontrado = false;
	int i = 0;
	int posicion = 0;

	while (i < ip->cantidad_pokemones && !pokemon_encontrado) {
		if (strcmp(ip->pokemones[i].nombre, nombre) == 0) {
			pokemon_encontrado = true;
			posicion = i;
		}
		i++;
	}

	if (pokemon_encontrado) {
		return ip->pokemones + posicion;
	}
	return NULL;
}
int pokemon_cantidad(informacion_pokemon_t *ip)
{
	if (ip == NULL) {
		return 0;
	}
	return ip->cantidad_pokemones;
}
const char *pokemon_nombre(pokemon_t *pokemon)
{
	if (pokemon == NULL) {
		return NULL;
	}
	return pokemon->nombre;

	return NULL;
}
enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	if (pokemon == NULL) {
		return NORMAL;
	}
	return pokemon->tipo_del_pok;
}
const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
					   const char *nombre)
{
	if (pokemon == NULL || nombre == NULL) {
		printf("El pokemon es NLL o el nombre\n");
		return NULL;
	}

	bool ataque_encontrado = false;
	int i = 0;
	int posicion = 0;

	while (i < MAX_ATAQUES && !ataque_encontrado) {
		if (strcmp(pokemon->ataques_pok[i].nombre, nombre) == 0) {
			ataque_encontrado = true;
			posicion = i;
		}
		i++;
	}
	if (ataque_encontrado) {
		return pokemon->ataques_pok + posicion;
	} else {
		return NULL;
	}
}

int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
		     void *aux)
{
	if (ip == NULL || f == NULL || aux == NULL) {
		return 0;
	}
	ordenar_pokemones(ip, ip->cantidad_pokemones);

	int cantidad_aplicaciones = 0;
	for (int i = 0; i < ip->cantidad_pokemones; i++) {
		f(ip->pokemones + i, aux);
		cantidad_aplicaciones++;
	}
	return cantidad_aplicaciones;
}

int con_cada_ataque(pokemon_t *pokemon,
		    void (*f)(const struct ataque *, void *), void *aux)
{
	if (pokemon == NULL || f == NULL)
		return 0;
	int cantidad_aplicaciones = 0;
	for (int i = 0; i < MAX_ATAQUES; i++) {
		f(pokemon->ataques_pok + i, aux);
		cantidad_aplicaciones++;
	}
	return cantidad_aplicaciones;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
	if (ip == NULL)
		return;

	if (ip->pokemones != NULL)
		free(ip->pokemones);

	free(ip);
}
