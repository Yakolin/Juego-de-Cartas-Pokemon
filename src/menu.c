#include "menu.h"
#include "hash.h"

struct menu {
	hash_t *comandos;
};

typedef struct comando_data {
	char *descripcion;
	bool (*funcion)(void *);
} comando_t;

void destructor_de_comandos(void *aux)
{
	comando_t *comando = aux;
	if (comando)
		free(comando);
}
/*
	Crea un nuevo menu
*/
menu_t *menu_crear()
{
	menu_t *nuevo_menu = calloc(1, sizeof(menu_t));
	if (!nuevo_menu)
		return NULL;
	nuevo_menu->comandos = hash_crear(3);
	return nuevo_menu;
}
/*
	Esta funcion agrega al menu un nuevo comando con una funcion bool
	En caso de fallar devuelve false, sino devuelve true
*/
bool menu_agregar_comando(menu_t *menu, char *comando, char *descripcion,
			  bool (*f)(void *))
{
	comando_t *nuevo_comando = calloc(1, sizeof(comando_t));
	if (!nuevo_comando)
		return false;
	nuevo_comando->descripcion = descripcion;
	nuevo_comando->funcion = f;
	if (hash_insertar(menu->comandos, comando, nuevo_comando, NULL))
		return true;
	else {
		free(nuevo_comando);
		return false;
	}
}
//VERIFICAR QUE SE BORRE
bool menu_quitar_comando(menu_t *menu, char *comando)
{
	void *comando_a_quitar = hash_quitar(menu->comandos, comando);
	if (!comando_a_quitar)
		return false;
	destructor_de_comandos(comando_a_quitar);
	return true;
}
/*
	Recibe por parametro un menu y un comando, se busca el comando y si existe se ejecuta, en caso de
	no existir ese comando devuelve MENU_INEXISTENTE o si falla al ejecutar el comando MENU_ERROR
	si todo sale bien devuelve MENU_Ok
*/
MENU_RESULTADO ejecutar_comando(menu_t *menu, char *comando, void *contexto)
{
	comando_t *comando_ejecutar = hash_obtener(menu->comandos, comando);
	if (!comando_ejecutar)
		return MENU_INEXISTENTE;
	if (!contexto || comando_ejecutar->funcion(contexto) == false)
		return MENU_ERROR;
	return MENU_OK;
}
void menu_destruir(menu_t *menu)
{
	if (!menu)
		return;
	hash_destruir_todo(menu->comandos, destructor_de_comandos);
	free(menu);
	printf("LIBERASTE MEMORIA\n");
}