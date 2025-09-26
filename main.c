#include "src/pokemon.h"
#include "src/tipo.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "src/adversario.h"
#include <ctype.h>
#include "src/hash.h"
#include "src/menu.h"
#include <string.h>
#include <stdbool.h>
typedef struct estado_juego {
	juego_t *juego;
	adversario_t *adversario;
	lista_t *lista_pokemones;
	char pokemones_jugador[3][23];
	bool continuar;
} estado_t;

//Compara el nombre de una structura pokemon con un nombre, si son iguales devuelve 0, sino 1
int comparar_nombres(void *struct_poke1, void *nombre_poke2)
{
	pokemon_t *pokemon = struct_poke1;
	if (strcmp(pokemon_nombre(pokemon), (char *)nombre_poke2) == 0) {
		return 0;
	}
	return 1;
}
//Devuelve un str igual al tipo enviado
char *devolver_str_tipo(enum TIPO tipo)
{
	if (tipo == NORMAL)
		return "NORMAL";
	if (tipo == AGUA)
		return "AGUA";
	if (tipo == FUEGO)
		return "FUEGO";
	if (tipo == AGUA)
		return "Agua";
	if (tipo == ROCA)
		return "ROCA";
	if (tipo == ELECTRICO)
		return "ELECTRICO";
	if (tipo == PLANTA)
		return "PLANTA";
	return "NORMAL";
}
//Muestra por pantalla un la PRESENTACION DEL JUEGO
void imprimir_presentacion()
{
	;
	printf("  ____    ___   _____   _   _  __     __  _____   _   _   ___   ____    ___    \n");
	printf(" | __ )  |_ _| | ____| | \\ | | \\ \\   / / | ____| | \\ | | |_ _| |  _ \\  / _ \\  \n");
	printf(" |  _ \\   | |  |  _|   |  \\| |  \\ \\ / /  |  _|   |  \\| |  | |  | |  ||| | | |  \n");
	printf(" | |_) |  | |  | |___  | |\\  |   \\ V /   | |___  | |\\  |  | |  | |_| || |_| |  \n");
	printf(" |____/  |___| |_____| |_| \\_|    \\_/    |_____| |_| \\_| |___| |____/  \\___/   \n");
	printf("                              _      \n");
	printf("                             / \\    \n");
	printf("                            / _ \\   \n");
	printf("                           / ___ \\  \n");
	printf("                          /_/   \\_\\\n");

	printf("                                  ,'\\                               \n");
	printf("    _.----.        ____         ,'  _\\    ___    ___     ____        \n");
	printf("_,-'       `.     |    |  /`.   \\,-'     |   \\  /   |   |    \\  |`. \n");
	printf("\\      __    \\    '-.  | /   `.  ___     |    \\/    |   '-.   \\ |   |\n");
	printf(" \\.    \\ \\   | __   |  |/    ,','_  `.   |          | __  |    \\|   |\n");
	printf("   \\    \\/   /,' _`.|      ,' / / / /    |          ,' _`.|     |   |\n");
	printf("    \\     ,-'/  /   \\    ,'   | \\/ / ,`. |         /  /   \\  |      |\n");
	printf("     \\    \\ |   \\_/  |   `-.  \\    `'  / |  |    ||   \\_/  | | \\    |\n");
	printf("      \\    \\ \\      /       `-.`.___,-'  |  |\\  /| \\      /  |  |   |\n");
	printf("       \\    \\ `.__,'|  |`-._    `|       |__| \\/ |  `.__,'|  |  |   |\n");
	printf("        \\_.-'       |__|    `-._ |               '-.|     '-.|  |   |\n");
	printf("                                `'                              '-._|\n");

	printf("_______________________________________________________________________________\n");
	printf("||     PRIMERO TE PEDIREMOS QUE INGRESES EL NOMBRE DE UN ARCHIVO VALIDO      ||\n");
	printf("||___________________________________________________________________________||\n");
}
//Imprime el campo de batalla de los pokemon
void imprimir_campo_batalla()
{
	printf("⠀⠀⠀⠀⠀⢸⠓⢄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                       \n");
	printf("⠀⠀⠀⠀⠀⢸⠀⠀⠑⢤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                        \n");
	printf("⠀⠀⠀⠀⠀⢸⡆⠀⠀⠀⠙⢤⡷⣤⣦⣀⠤⠖⠚⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀                      \n");
	printf("⣠⡿⠢⢄⡀⠀⡇⠀⠀⠀⠀⠀⠉⠀⠀⠀⠀⠀⠸⠷⣶⠂⠀⠀⠀⣀⣀⠀⠀⠀                      \n");
	printf("⢸⣃⠀⠀⠉⠳⣷⠞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠉⠉⠉⢉⡭⠋        o              \n");
	printf("⠀⠘⣆⠀⠀⠀⠁⠀⢀⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡴⠋⠀⠀       /|\\            \n");
	printf("⠀⠀⠘⣦⠆⠀⠀⢀⡎⢹⡀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⡀⣠⠔⠋⠀⠀⠀⠀       / \\             \n");
	printf("⠀⠀⠀⡏⠀⠀⣆⠘⣄⠸⢧⠀⠀⠀⠀⢀⣠⠖⢻⠀⠀⠀⣿⢥⣄⣀⣀⣀ 	 		_-_       \n");
	printf("⠀⠀⢸⠁⠀⠀⡏⢣⣌⠙⠚⠀⠀⠠⣖⡛⠀⣠⠏⠀⠀⠀⠇⠀⠀⠀⠀⢙⣣⠄               /~~   ~~\\  \n");
	printf("⠀⠀⢸⡀⠀⠀⠳⡞⠈⢻⠶⠤⣄⣀⣈⣉⣉⣡⡔⠀⠀⢀⠀⠀⣀⡤⠖⠚⠀		  /~~         ~~\\\n");
	printf("⠀⠀⡼⣇⠀⠀⠀⠙⠦⣞⡀⠀⢀⡏⠀⢸⣣⠞⠀⠀⠀⡼⠚⠋⠁⠀⠀⠀⠀ 		 {               }\n");
	printf("⠀⢰⡇⠙⠀⠀⠀⠀⠀⠀⠉⠙⠚⠒⠚⠉⠀⠀⠀⠀⡼⠁⠀⠀⠀⠀⠀⠀⠀⠀   	  \\  _-     -_  /\n");
	printf("⠀⠀⢧⡀⠀⢠⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀             ~  \\\\ //  ~   \n");
	printf("⠀⠀⠀⠙⣶⣶⣿⠢⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀      	_- -   | | _- _  \n");
	printf("⠀⠀⠀⠀⠀⠉⠀⠀⠀⠙⢿⣳⠞⠳⡄⠀⠀⠀⢀⡞⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀            _ -  | |   -_  \n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠀⠀⠹⣄⣀⡤⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                // \\\\    \n");

	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	printf("						  ⠰⡧⠚⡄⠀⠀⠀⠀⠀\n");
	printf("						 ⢠⠗⡠⢄⠉⠉⢙⠀⠀⠀⠀⠀⠀\n");
	printf("					   ⣠⡴⠦⠝⠋⢌⠀⠀⠱⡀⠀⢸⠁⠀⠀⠀⠀⠀\n");
	printf("					 ⡠⣊⠄⠒⠩⡩⠂⠒⡑⡄⠀⠘⡀⠀⡘⠀⠀⠀⠀⠀\n");
	printf("				 	⢸⡄⡣⡀⠀⡇⠃⠀⣶⣾⢰⠀⠀⡇⡀⠭⠋⠀⠀⠀⠀\n");
	printf("				         ⠹⡛⢾⣦⣥⠡⣀⣩⢌⠌⠀⢀⡣⠁⠀⠀⠀⠀⠀⠀⠀\n");
	printf("      _  _  _  _     	   		    ⠂⠍⡻⠎⠀⠈⡁⢀⣠⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("    _(_)(_)(_)(_)_   			    ⡨⠗⠁⠊⠿⠻⠉⠓⡲⠦⠤⢤⣄⠀⠀⠀⠀⠀\n");
	printf("   (_)          (_) 			   ⠀⡠⠊⠀⢀⠆⠀⠀⢧⡀⣠⡽⠤⠀⠒⣉⣀⡀⠀⠀⠀\n");
	printf("   (_)          (_)  			 ⡠⠊⠀⢀⠴⡙⠖⣉⠻⣋⠳⢡⢝⣄⠄⠁⣀⣀⠈⠑⢆⠀\n");
	printf("   (_)          (_)  			⠰⣿⡤⠐⠊⠀⠀⢏⡎⠀⠓⠁⠈⠃⠀⡇⢠⠊⠤⡀⠑⡄⠀⢃\n");
	printf("   (_)          (_)  			 ⠀⠀⠀⠀⢠⠊⠀⠀⠀⠀⠀⠀⠀⢇⠈⠢⠄⠃⢀⠃⠀⢸   \n");
	printf("     (_)(_)(_)(_)    			⠀⠀⠀⠀⠘⡄⠀⠀⡰⠚⢦⡀⠀⠀⠁⠒⠀⠒⠁⠀⢠⠇    \n");
	printf("          | |				⠀⠀⠰⠶⠙⣀⢀⠇⠀⠸⠍⢂⡤⢀⢀⣀⠀⠤⠒⠁⠀    \n");
}
//Muestra por pantalla un ATAQUE EFECTIVO ENEMIGO
void enemigo_efectivo()
{
	printf("⠀⠳⡄⠀⠘⢌⢂⠹⡄⠀⠘⡆⠀⠀⠀⡮⡒⡇⣰⡿⢀⡇⣦⡎⡱⠂⢀⢠⣹⡶  \n");
	printf("⣄⠀⠈⢢⡀⠈⢊⢆⠹⡄⠀⢡⠀⠀⠀⡇⡇⡜⢹⠃⣸⠀⣿⣿⡽⢀⣰⣯⡏⣩  \n");
	printf("⣕⢷⣄⠀⠑⢄⠀⠙⣆⢳⡀⢰⣸⣧⢠⠇⡿⢠⡏⠀⣿⠀⠸⣿⣿⣛⣵⠾⡛⠁  \n");
	printf("⠈⠓⢬⡳⣄⠀⠠⡀⠈⢂⣷⠿⢋⠈⢻⠻⠃⠈⠀⠐⢼⡀⢠⠜⠩⠛⡝⠀⠀⠀  \n");
	printf("⣶⣖⣶⣾⡶⣷⡦⣤⠤⠗⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡑⣤⡤⠤⢀⣀⠀⠀⠀    \n");
	printf("⠙⢿⣷⠦⣀⠁⠑⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢣⠈⢣⠀⣀⠿⠀⠀⠀     \n");
	printf("⠀⠀⠙⢄⠀⠁⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⡇⠘⣿⠁⠒⠀⠀⠀      \n");
	printf("⠀⠀⠀⠀⢑⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡜⢀⠇⢰⠋⡆⠀⠀⠀⠀     \n");
	printf("⠀⠀⠀⠀⣨⣇⠀⠀⠀⠀⢸⠒⠤⣀⠀⠀⠀⠀⠀⠀⠓⠈⡠⢹⢨⡇⠀⠀⠀⠀    \n");
	printf("⠀⢀⣤⣶⣟⡉⠇⠀⡐⠀⠈⢦⡀⠁⠈⡩⠇⠀⠀⠀⡠⠞⢄⣾⣽⠃⠀⠀⠀⠀   \n");
	printf("⣴⣿⣿⣿⣠⠎⠽⠀⠠⠡⡀⠸⣌⣙⣉⣀⡠⠤⠐⠈⠄⣠⣾⣾⣯⠐⠂⠄⡀⠀  \n");
	printf("⠿⠻⠷⠷⠟⠛⢿⢿⣦⣤⣀⣄⣈⣑⣲⣵⣤⣬⣦⣶⣺⣽⣿⣿⣿⣧⠄⠀⠀⠀  \n");
	printf("⠀⠠⣤⡊⠀⣜⡿⢏⡟⠿⣟⣿⣟⣿⣿⡟⣿⣿⡿⠿⣿⣿⣿⣿⣿⣿⣧⣤⣤⣄  \n");
	printf("⠀⠀⠠⣞⡭⢿⣴⣏⣵⣿⣽⣿⣓⣟⣫⣍⣁⣴⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿    \n");
	printf("⠀⠀⠠⠚⢉⡶⠛⠛⠛⢛⠛⠛⢟⡟⠹⡟⢹⡭⠍⠙⡍⢫⡉⢉⡫⠉⠉⠉⠉    \n");
	printf("____ _   _  ____ _   _ _ ____ ____   ____ ____ ____ ____ ___ _ _  _ ____ \n");
	printf("|___ |\\ | |___ |\\/| | | __ |  |    |___ |___ |___ |     |  | |  | |  | \n");
	printf("|___ | \\| |___ |   | | |__] |__|    |___ |    |___ |___  |  |  \\/ |__| \n");
}
//Muestra por pantalla un ATAQUE EFECTIVO
void jugador_efectivo()
{
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣶⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⠛⢿⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡏⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⠃⠀⠼⣄⡀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⡴⠋⠳⣄⠀⠀⢀⢎⣔⠀⠀⠀⠀⠉⠳⣄⠀⠀⠀⠀⠀\n");
	printf("⠀⢠⠞⠁⠀⠀⠀⠹⣶⢛⠘⡋⠀⢠⣎⣦⠀⠀⠈⠙⠲⢤⡀⠀\n");
	printf("⣰⠏⠀⠀⠀⠀⠀⠀⢰⠀⠰⡤⡀⠀⢛⣋⠀⠀⠰⣄⣀⣠⣿⣆\n");
	printf("⠛⠒⠶⠶⢶⡶⠄⢀⣨⢦⡀⠢⠃⠀⠸⣿⠇⠀⢰⠃⠉⠉⠉⠉\n");
	printf("⠀⠀⠀⠰⠿⣤⣀⠛⢧⣌⡇⠀⠀⠀⠀⠀⠰⠉⠙⢳⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⣼⣣⣞⣽⣇⠀⠈⠑⢄⠀⠐⢄⣀⣸⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠈⠛⣿⣿⠁⠉⠠⡀⠀⠀⡆⠀⠀⠀⢹⡀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠹⡿⠇⠀⠀⢈⠁⠊⠀⠀⠀⠀⠈⣇⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⣧⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⣸⠗⠀⣀⣦⣀⣤⣤⠴⠾⣄⡀⠺⡄⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠉⠉⠁⠀⠀\n");
	printf("____ ___ ____ _  _ ____    ____ ____ ____ ____ ___ _ _  _ ____ \n");
	printf("|__|  |  |  | |  | |___    |___ |___ |___ |     |  | |  | |  | \n");
	printf("|  |  |  |_\\ |__| |___    |___ |    |___ |___  |  | \\/  |__| \n");
}
//Muestra por pantalla la pantalla de derrota
void perdiste()
{
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⣤⣤⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡤⠤⠖⠒⠋⠉⠉⠁⠀⠀⠀⠀⠀⠀⠈⠙⠲⢤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠞⠋⠁⠀⠀⠀⠀⠀⣀⣤⠤⠤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠳⢦⣤⡀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡏⠀⠀⠀⠀⠀⣠⠔⠋⠁⠀⠀⠀⠀⣹⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣹⢶⣄⠀⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠇⢲⠀⠀⠀⣴⠃⠀⣠⠶⠒⠳⣶⡛⠉⠘⡇⠀⠀⠀⠀⠀⠀⠀⣀⡴⠛⠁⠀⢙⣷⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠄⠀⠀⠀⠀⠉⠉⠛⠁⠀⠀⠀⠛⢷⣄⣰⠇⠀⠀⠀⣀⡤⠖⠋⠁⠀⠀⠀⠀⠀⢹⡆⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⡏⠀⠀⢀⣠⣤⠤⠖⠒⠒⠒⠦⣄⡀⠀⢀⣉⣠⠴⠖⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⢘⡇⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⣀⣼⠷⠒⠋⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⢤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⣀⡴⠟⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠲⢤⣄⡀⠀⠀⠀⠀⠀⢀⣀⣾⣃⣀⣀⡀\n");
	printf("⠀⣀⡴⠞⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⣛⣶⡴⠖⠛⠛⠹⣿⣿⡟⠛⠁\n");
	printf("⠘⠿⢤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣠⠴⠚⠉⡴⣿⠀⠀⠀⠀⣀⣽⣿⣿⠷⠆\n");
	printf("⠀⠀⠀⠀⠉⢉⣷⣶⡖⠒⠒⠒⠒⠒⠒⠒⠒⠒⠒⠒⠚⠛⠛⠋⠉⠉⠉⠉⠉⢀⣀⠴⠊⢰⡇⠸⠈⢿⣿⣿⣿⣯⡀⠀⠀\n");
	printf("⠀⠀⠀⠀⠺⠿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡏⣾⣤⠸⣿⣾⡿⠟⠛⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⣰⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣧⣼⠾⢿⣿⣿⡄⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠉⠉⠀⠹⣆⠀⠀⠀⠀⠀⠀⠠⠔⠒⠒⠢⢄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⡟⠁⠀⠀⣿⠈⠁⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣧⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣄⣤⠞⠁⠀⠀⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢷⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣠⣤⣶⣾⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⠶⠶⠶⠶⢶⡦⠀⠀⠀⠀⠀⣿⠋⠻⣿⡟⠛⠻⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("                         ⠸⠇⠀⠀⠀⠀⠀⠃⠀⠀⠀⠁⠀⠀⠀\n");
	printf("  _______   _______   ___ ___   _______      _______   ___ ___   _______   _______ \n");
	printf(" |   _   | |   _   | |   Y   | |   _   |    |   _   | |   Y   | |   _   | |   _   \\\n");
	printf(" |.  |___| |.  1   | |.      | |.  1___|    |.  |   | |.  |   | |.  1___| |.  l   /\n");
	printf(" |.  |   | |.  _   | |. \\_/  | |.  __)_    |.  |   | |.  |   | |.  __)_  |.  _   1\n");
	printf(" |:  1   | |:  |   | |:  |   | |:  1   |    |:  1   | |:  1   | |:  1   | |:  |   |\n");
	printf(" |::.. . | |::.|:. | |::.|:. | |::.. . |    |::.. . |  \\:.. ./ |::.. . | |::.|:. |\n");
	printf(" `-------' `--- ---' `--- ---' `-------'    `-------'   `---'   `-------' `--- ---'\n");
}
//Muestra por pantalla la pantalla de victoria
void ganaste()
{
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⠋⠒⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡏⠀⠈⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⢿⡄⠀⠀⠙⢦⡀⠀⠀⠀⠀⠀⠀⠀⢇⠀⠀⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠂⠀⠀⠀⠀⠀⠀⠀⠀⠑⢄⡀⠀⠀⠈⠲⠒⠉⠉⠉⠁⠐⠺⢦⡀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⢀⣱⢠⠀⡆⠀⠀⠀⠀⠀⠀⠉⠓⣶⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢾⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⠀⠤⠤⢤⣀⣘⢯⣾⢾⡷⡚⠉⠀⠀⠀⠀⠀⡘⠈⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⡀⠀⠀⠀⠀⠈⠉⠉⠒⠲⠮⣉⣀⠀⠀⠀⢠⡇⠀⠀⣶⣟⡆⠀⠀⠀⠀⢰⣲⡆⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⡿⠀⠈⠸⡇⢀⠤⡛⠛⠁⠀⠀⠠⠀⠸⠟⠋⢰⣅⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⢡⠀⠀⠀⠀⠂⠀⠀⠀⠀⢰⠀⡸⠁⠀⠀⠀⠇⣇⠀⢸⠀⠰⣤⣤⣤⣤⠄⠀⢠⢺⡿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⡈⣆⣠⠤⠤⠤⠐⠖⢲⠀⡄⢰⠁⠀⠀⠀⠀⡜⠈⠓⠋⠀⠀⢣⡀⣠⠎⠀⠀⣾⡎⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠠⣀⡁⠀⠀⡎⢠⠃⣇⠀⠀⠀⠀⢠⡇⠀⠀⠀⠀⣲⢤⣄⡠⢤⡖⠋⠀⢡⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⠀⠀⣆⠀⣘⠛⠂⠰⢇⣘⠂⠬⣁⠒⡄⠀⢸⢷⡀⠀⠀⠀⠀⠀⠀⠉⠛⢄⠀⠀⠸⢣⡀⣤⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
	printf("⠀⠐⣸⡾⢋⣀⡄⠀⠀⠀⠉⠳⣤⣡⡇⠀⢸⠈⠃⠀⠀⠀⠀⠀⠀⠀⠀⠈⣃⣠⠃⠀⡏⡿⠁⠀⠀⠀⠀⠀⠀⠀⠠⢶⠆⠀⠀\n");
	printf("⠀⠀⠋⠃⡾⠟⠓⠀⠀⠀⠀⠀⠧⠥⠭⡝⡎⣧⡀⠀⠀⠀⠉⠐⠒⠐⠒⠊⢣⢀⣴⣶⣷⠇⠀⠀⠀⡀⠀⣼⠀⠀⡀⠉⠘⠀⠀\n");
	printf("⠀⠀⠀⠘⠁⠁⠀⠨⡀⠀⠀⠀⠀⠀⠀⠳⠷⢿⣅⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⡿⠋⠁⠀⠀⢀⡿⣀⢿⣶⡼⠷⠟⠁⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢻⣿⣶⠤⢄⡀⠀⠀⢀⡀⣾⣻⡟⠁⠀⠀⠀⠔⠁⠀⠉⢸⠉⠩⠍⠀⠰⡰⠦\n");
	printf(". ,   __    _    _  __    _     ___ _____   ___     . ,\n");
	printf(" x  ,'_/  .' \\ / |/ /  .' \\  ,' _//_  _/ / _/      x \n");
	printf("' `/ /_n / o / / || /  / o / _\\ `. / /   / _/      ' `\n");
	printf("   |__,'/_n_/ /_/|_/  /_n_/ /___,' /_/   /___/         \n");
	printf("                                                      \n");
}
/*
	Imprime por pantalla el ataque utilizado
*/
void informar_ataque(char *pokemon, char *ataque)
{
	printf("_________________________\n");
	printf("|| %s  USA  %s!!!!                   \n", pokemon, ataque);
	printf("||_______________________\n");
}
//Muestra los puntos de los jugadores por pantalla
bool mostrar_puntos(void *estado)
{
	estado_t *estado_juego = estado;
	printf("___________________________\n");
	printf("|                          |\n");
	printf("| Puntaje jugador 1: %i    |\n",
	       juego_obtener_puntaje(estado_juego->juego, JUGADOR1));
	printf("| Puntaje jugador 2: %i    |\n",
	       juego_obtener_puntaje(estado_juego->juego, JUGADOR2));
	printf("|__________________________|\n");
	return true;
}
//MUETRA LOS ATAQUES ENVIADOS
void mostrar_ataques(const struct ataque *ataque, void *aux)
{
	printf("- %s              \n", ataque->nombre);
	char *tipo = devolver_str_tipo(ataque->tipo);
	printf("- Tipo %s   \n", tipo);
	printf("- Poder %i  \n", ataque->poder);
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}
//MUESTRA LOS POKEMON ENVIADOS
bool mostrar_pokemon(void *pokemon, void *aux)
{
	printf("    POKEMON:\n");
	printf("_______________________________\n");
	pokemon_t *pokemon_st = pokemon;
	printf("||                           ||\n");
	printf("|| %s                        ||\n", pokemon_nombre(pokemon_st));
	char *tipo = devolver_str_tipo(pokemon_tipo(pokemon_st));
	printf("|| Tipo %s                   ||\n", tipo);
	printf("||___________________________||\n\n");
	printf("    ATAQUES:\n");
	printf("    _______\n");
	if (con_cada_ataque(pokemon_st, mostrar_ataques, NULL) == 0) {
		return false;
	}
	printf("\n");
	return true;
}
//Le pide al usuario que escriba
void pedir_escribir(char *string)
{
	fgets(string, 23, stdin);
	size_t len = strlen(string);
	if (string[len - 1] == '\n') {
		string[len - 1] = '\0';
	}
}
// Función para obtener los nombres de los tres Pokémon
void obtener_nombres_pokemon(char *nombre1, char *nombre2, char *nombre3)
{
	printf("Ingresa el nombre del primer Pokemon: ");
	pedir_escribir(nombre1);
	printf("Ingresa el nombre del segundo Pokemon: ");
	pedir_escribir(nombre2);
	printf("Ingresa el nombre del tercer Pokemon: ");
	pedir_escribir(nombre3);
}
/*
	Le pide al usario ingresar el nombre del pokemon que quiere utilizar y su ataque, si alguno de estos no se encuentra, 
	se repetira la pregunta.
*/
jugada_t jugador_pedir_nombre_y_ataque()
{
	jugada_t nueva_jugada;
	printf("Ingresa el nombre del pokemon: ");
	pedir_escribir(nueva_jugada.pokemon);
	printf("Ingresa el nombre del ataque: ");
	pedir_escribir(nueva_jugada.ataque);
	return nueva_jugada;
}
//REALIZA LA JUGADA DEL JUEGO
bool realizar_jugada(void *contexto)
{
	imprimir_campo_batalla();
	estado_t *estado = contexto;
	resultado_jugada_t resultado_ronda;
	bool ataques_correctos = false;
	jugada_t jugada_jugador;
	jugada_t jugada_adversario;
	int i = 0;
	while (!ataques_correctos) {
		jugada_jugador = jugador_pedir_nombre_y_ataque();
		jugada_adversario =
			adversario_proxima_jugada(estado->adversario);
		resultado_ronda = juego_jugar_turno(
			estado->juego, jugada_jugador, jugada_adversario);
		if (resultado_ronda.jugador1 == ATAQUE_ERROR ||
		    resultado_ronda.jugador2 == ATAQUE_ERROR) {
			printf("Uno de los ataques tuvo un error\n");
			if (i == 4) {
				printf("Te equivocaste muchas veces, vuelve a ver tus pokemones\n");
				return true;
			}
		} else
			ataques_correctos = true;
		i++;
	}
	printf("\n|| JUGADOR ||\n");
	informar_ataque(jugada_jugador.pokemon, jugada_jugador.ataque);

	printf("\n|| ENEMIGO ||\n");
	informar_ataque(jugada_adversario.pokemon, jugada_adversario.ataque);
	if (resultado_ronda.jugador1 == ATAQUE_EFECTIVO) {
		jugador_efectivo();
	}
	if (resultado_ronda.jugador2 == ATAQUE_EFECTIVO) {
		enemigo_efectivo();
	}
	adversario_informar_jugada(estado->adversario, jugada_jugador);
	return true;
}
//MUESTRA LA LISTA DE POKEMON SELECCIONADOS POR EL USUARIO
bool listar_pokes_disponibles(void *contexto)
{
	bool se_itero = true;
	estado_t *estado = contexto;
	lista_t *lista_pokes = juego_listar_pokemon(estado->juego);
	if (!lista_pokes)
		return false;
	if (lista_con_cada_elemento(lista_pokes, mostrar_pokemon, NULL) == 0)
		se_itero = false;
	estado->lista_pokemones = lista_pokes;
	return se_itero;
}
/*
	Muestra un pokemon de un jugador con todos sus ataques
*/
bool mostar_pokes_jugador(void *pokemon, void *aux)
{
	estado_t *estado = aux;
	if (strcmp(estado->pokemones_jugador[0], pokemon_nombre(pokemon)) == 0)
		mostrar_pokemon(pokemon, NULL);
	if (strcmp(estado->pokemones_jugador[1], pokemon_nombre(pokemon)) == 0)
		mostrar_pokemon(pokemon, NULL);
	if (strcmp(estado->pokemones_jugador[2], pokemon_nombre(pokemon)) == 0)
		mostrar_pokemon(pokemon, NULL);
	return true;
}
/*
	Lista todos los pokemones en la fase de combate seleccionados por el jugador
*/
bool listar_pokemones_combate(void *contexto)
{
	bool se_itero = true;
	estado_t *estado = contexto;
	lista_con_cada_elemento(estado->lista_pokemones, mostar_pokes_jugador,
				estado);
	return se_itero;
}
//MUESTRA LOS COMANDOS DEL MENU
bool mostrar_comandos_menu(void *contexto)
{
	printf("_________________________________\n");
	printf("||     COMANDOS MENU           ||\n");
	printf("||_____________________________||\n");
	printf("||   q : Quitar juego          ||\n");
	printf("||   i : Inicializa el juego   ||\n");
	printf("||_____________________________||\n");
	printf("||     COMANDOS DE COMBATE     ||\n");
	printf("||_____________________________||\n");
	printf("||   r : Realizar jugada       ||\n");
	printf("||   l : Muestra los pokemones ||\n");
	printf("||   	    que seleccionaste  ||\n");
	printf("||   p : Muestra los puntos    ||\n");
	printf("||   	    de ambos jugadores ||\n");
	printf("||_____________________________||\n");
	return true;
}
//Inicia el juego
bool iniciar_juego(void *estado)
{
	return true;
}
//Quita el juego
bool quitar_juego(void *estado)
{
	estado_t *estado_juego = estado;
	estado_juego->continuar = false;
	return true;
}
/*
	Carga el juego con los pokemones, pidiendo por consola al usuario 3 pokemones, los guarda en el estado para tenerlos mas a mano
	en caso de error general / memoria, se devolvera ERROR_GENERAL o pasado 4 intentos de poner los nombres
	En caso de todo funcionar correctamente devolvera TODO_OK
*/
JUEGO_ESTADO cargar_pokemones(estado_t *estado, juego_t *juego,
			      adversario_t *adversario)
{
	char nombre_poke1_jugador[23], nombre_poke2_jugador[23],
		nombre_poke3_jugador[23];
	char *nombre_poke1_adversario, *nombre_poke2_adversario,
		*nombre_poke3_adversario;
	bool se_inserto = false;
	int i = -1;
	while (!se_inserto) {
		if (i == 4) {
			printf("Pusiste mal demasiadas veces los nombres, vuelve a intentarlo mas tarde\n");
			return ERROR_GENERAL;
		}
		obtener_nombres_pokemon(nombre_poke1_jugador,
					nombre_poke2_jugador,
					nombre_poke3_jugador);
		JUEGO_ESTADO inserccion = juego_seleccionar_pokemon(
			juego, JUGADOR1, nombre_poke1_jugador,
			nombre_poke2_jugador, nombre_poke3_jugador);
		if (inserccion == POKEMON_INEXISTENTE) {
			i++;
			printf("UN POKEMON NO EXISTE\n");
		} else if (inserccion == POKEMON_REPETIDO) {
			i++;
			printf("UN POKEMON ESTA REPETIDO\n");
		} else if (inserccion == ERROR_GENERAL)
			return ERROR_GENERAL;
		else
			se_inserto = true;
	}
	se_inserto = false;

	adversario_seleccionar_pokemon(adversario, &nombre_poke1_adversario,
				       &nombre_poke2_adversario,
				       &nombre_poke3_adversario);

	if (juego_seleccionar_pokemon(juego, JUGADOR2, nombre_poke1_adversario,
				      nombre_poke2_adversario,
				      nombre_poke3_adversario) == ERROR_GENERAL)
		return ERROR_GENERAL;
	if (!adversario_pokemon_seleccionado(adversario, nombre_poke1_jugador,
					     nombre_poke2_jugador,
					     nombre_poke3_jugador))
		return ERROR_GENERAL;

	strcpy(estado->pokemones_jugador[0], nombre_poke1_jugador);
	strcpy(estado->pokemones_jugador[1], nombre_poke2_jugador);
	strcpy(estado->pokemones_jugador[2], nombre_poke3_adversario);

	return TODO_OK;
}
/*
	Es el menu del juego, se le pedira al usuario ingresar algun comando
	Se le informara al usuario de todas sus acciones

*/
bool menu_juego_pokemon(menu_t *menu, estado_t estado_juego)
{
	bool juego_iniciado = false;
	MENU_RESULTADO resultado = 0;
	while (!juego_iniciado && estado_juego.continuar &&
	       !juego_finalizado(estado_juego.juego)) {
		printf(">");

		char linea[200];
		fgets(linea, 200, stdin);
		linea[1] = 0;
		resultado = ejecutar_comando(menu, linea, &estado_juego);
		if (resultado == MENU_OK && *linea == 'i')
			juego_iniciado = true;
		if (resultado == MENU_INEXISTENTE)
			printf("Ese comando no existe :( \n Usa 'a' para ver los comandos\n");
		else if (resultado == MENU_ERROR)
			printf("Hubo un error al ejecutar el comando\n");
	}
	return juego_iniciado;
}
// DEVUELVE EL ARCHIVO SI LO ENCUENTRA, SINO DEVUELVE NULL
char *pedir_archivo()
{
	bool existe = false;
	char *archivo = calloc(MAX_LETRAS, sizeof(char));
	if (!archivo)
		return NULL;

	while (!existe) {
		printf("Ingrese el nombre del archivo: ");
		pedir_escribir(archivo);
		FILE *file = fopen(archivo, "r");
		if (file) {
			fclose(file);
			existe = true;
		} else {
			printf("El archivo no se encuentra. Intente de nuevo.\n");
		}
	}
	return archivo;
}
// PIDE UN ARCHIVO Y CARGA EL JUEGO EN EL PROSCESO
bool cargar_archivo(juego_t *juego)
{
	char *archivo = NULL; //Esto tampoco va

	JUEGO_ESTADO es_juego = ERROR_GENERAL;

	while (archivo == NULL && es_juego != TODO_OK) {
		archivo = pedir_archivo();
		if (archivo != NULL)
			es_juego = juego_cargar_pokemon(juego, archivo);
		if (es_juego == POKEMON_INSUFICIENTES)
			printf("ESTE ARCHIVO TIENE POKEMONES INSUFICIENTES\n PROBA CON OTRO\n");
		else if (es_juego == ERROR_GENERAL)
			printf("ERROR AL CARGAR LOS POKEMON\n");
		else if (archivo == NULL)
			printf("ERROR AL CARGAR EL ARCHIVO\n");
	}
	printf("Cargaste correctamente el archivo\n\n");
	free(archivo);
	return true;
}
//Destruye todas las estructuras creadas
void destruir_todo(menu_t *menu, adversario_t *adversario, juego_t *juego)
{
	if (menu)
		menu_destruir(menu);
	if (adversario)
		adversario_destruir(adversario);
	if (juego)
		juego_destruir(juego);
}
int main()
{
	juego_t *juego = juego_crear();
	if (!juego)
		return 1;
	menu_t *menu = menu_crear();
	if (!menu) {
		juego_destruir(juego);
		return 1;
	}
	estado_t estado_juego;
	estado_juego.juego = juego;
	estado_juego.continuar = true;
	menu_agregar_comando(menu, "a", "Muestra los comandos",
			     mostrar_comandos_menu);
	menu_agregar_comando(menu, "q", "Quita el juego", quitar_juego);
	menu_agregar_comando(menu, "i", "Iniciar el juego", iniciar_juego);

	imprimir_presentacion();
	if (!cargar_archivo(juego)) {
		destruir_todo(menu, NULL, juego);
		return 1;
	}
	char ver_lista[20];
	printf("Preparado para comenzar? (y/n)\n");
	pedir_escribir(ver_lista);
	if (ver_lista[0] == 'y')
		listar_pokes_disponibles(&estado_juego);
	else {
		destruir_todo(menu, NULL, juego);
		return 1;
	}
	printf("_______________________________________________________________________________\n");
	printf("||    BIENVENIDO AL MENU DEL JUEGO, SI QUIERES VER LOS COMANDOS USA 'a'      ||\n");
	printf("||___________________________________________________________________________||\n");
	//menu del juego
	if (!menu_juego_pokemon(menu, estado_juego)) {
		destruir_todo(menu, NULL, juego);
		return 1;
	}
	adversario_t *adversario =
		adversario_crear(estado_juego.lista_pokemones);
	if (!adversario) {
		destruir_todo(menu, NULL, juego);
		return 1;
	}
	estado_juego.adversario = adversario;
	if (cargar_pokemones(&estado_juego, juego, adversario) ==
	    ERROR_GENERAL) {
		printf("Fallos al cargar pokemones\n");
		destruir_todo(menu, adversario, juego);
		return 1;
	}
	//PUEDO AGREGAR UNA LISTA ADICIONAL QUE TENGA SOLO MIS POKEMONES
	printf("_______________________________________________________________________________\n");
	printf("||        COMIENZA EL JUEGO, PREPARATE PARA LA BATALLA POKEMON               ||\n");
	printf("||___________________________________________________________________________||\n");
	menu_quitar_comando(menu, "i");
	menu_agregar_comando(menu, "l", "Listar pokemones seleccionados",
			     listar_pokemones_combate);
	menu_agregar_comando(menu, "p", "Mostrar los puntos de ambos jugadores",
			     mostrar_puntos);
	menu_agregar_comando(menu, "r", "Realizar jugada", realizar_jugada);

	if (!menu_juego_pokemon(menu, estado_juego)) {
		if (juego_obtener_puntaje(juego, JUGADOR1) <
		    juego_obtener_puntaje(juego, JUGADOR2))
			perdiste();
		else
			ganaste();
		destruir_todo(menu, adversario, juego);
		return 1;
	}
}