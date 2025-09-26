#ifndef __MENU_H_
#define __MENU_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LETRAS 23

typedef struct menu menu_t;
typedef enum { MENU_OK, MENU_INEXISTENTE, MENU_ERROR } MENU_RESULTADO;

menu_t *menu_crear();

bool menu_agregar_comando(menu_t *menu, char *comando, char *descripcion,
			  bool (*f)(void *));

bool menu_quitar_comando(menu_t *menu, char *comando);

MENU_RESULTADO ejecutar_comando(menu_t *menu, char *comando, void *contexto);

void menu_destruir(menu_t *menu);

#endif // __MENU_H_