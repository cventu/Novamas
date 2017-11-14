
//**********************************************INCLUDES**********************************************
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include "players_db_manager.h"
//****************************************************************************************************


//**********************************************DEFINES***********************************************
#define CASH				0
#define	WINS				1

#define PLAYERS_MAX			10

#define CONSULTAR			1
#define	GANADOR				2
#define	CREAR				3
#define TABLAS				4
#define	MIGRAR				5
#define	SALIR				6
//****************************************************************************************************


//**********************************************PROTOTYPES********************************************
int compare_cash(const void * player_1, const void * player_2);
int compare_wins(const void * player_1, const void * player_2);
void print_table(uint8_t category);
//****************************************************************************************************

int main (void)
{
	int16_t position=0;
	player_t player_data;
	char name_aux[NAME_MAX_LENGTH];
	uint16_t prize=0;
	uint16_t option=0;
	uint8_t confirmation=0;
	
	while (option != SALIR)
	{
		printf("1) Ver datos de jugador\n2) Ingresar ganador\n3) Crear jugador\n4) Ver tablas\n5) Migrar DB\n6) Salir\n\nSeleccione la opcion deseada: ");
		scanf("%"SCNu16, &option);
		fflush(stdin);
		printf("\n");
		
		switch (option)
		{
			case CONSULTAR:
							printf("Jugador: ");
							fgets(name_aux, NAME_MAX_LENGTH, stdin);
							strtok(name_aux, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
							printf("\n");
							
							position = players_db_find(name_aux);
						
							if (position == PLAYER_NOT_FOUND)
							{
								printf("El jugador ingresado no existe!\n");
							}
							else if (position == DATABASE_ERROR)
							{
								printf("Error en la base de datos!\n");
							}
							else if (players_db_get(position, &player_data) != DATABASE_ERROR)
							{
								printf ("Nombre: %s\n", player_data.name);
								printf ("Victorias: %"PRIu32"\n", player_data.wins);
								printf ("Dinero: %"PRIu32"\n", player_data.cash);
							}
							else
							{
								printf("Error en la base de datos!\n");
							}
							break;
				
				
			case GANADOR:
							printf("Ganador: ");
							fgets(name_aux, NAME_MAX_LENGTH, stdin);
							strtok(name_aux, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
							printf("\n");
							
							position = players_db_find(name_aux);
						
							if (position == PLAYER_NOT_FOUND)
							{
								printf("El jugador ingresado no existe!\n");
							}
							else if (position == DATABASE_ERROR)
							{
								printf("Error en la base de datos!\n");
							}
							else
							{
								printf("Monto ganado: ");
								scanf("%"SCNu16, &prize);
								fflush(stdin);
								printf("\n");								
								
								if (players_db_get(position, &player_data) != DATABASE_ERROR)
								{
									player_data.wins++;
									player_data.cash += prize;
									
									if (players_db_update(position, &player_data) == DATABASE_ERROR)
									{
										printf("Error en la base de datos!\n");
									}
									else
									{
										printf("Ganador ingresado!\n");
									}
								}
								else
								{
									printf("Error en la base de datos!\n");
								}
							}
							break;
							
							
			case CREAR:
							printf("Ingrese el nombre del jugador: ");
							fgets(name_aux, NAME_MAX_LENGTH, stdin);
							strtok(name_aux, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
							printf("\n");
							
							switch (players_db_create(name_aux))
							{
								case DATABASE_ERROR:
													printf("Error en la base de datos!\n");
													break;
													
													
								case NAME_TOO_LONG:
													printf("El nombre ingresado es demasiado largo!\n");
													break;
													
													
								case PLAYER_CREATED:
													printf("Jugador creado exitosamente!\n");
													break;
							}
							break;
							
							
			case TABLAS:	
							print_table(CASH);
							printf("\n");
							print_table(WINS);
							break;
				
					
			case MIGRAR:	
							printf("Agrego el nuevo miembro en la estructura new_player_format?\n");
							printf("Lo inicializo en la funcion players_db_migrate?\n");
							printf("Recuerde que la estructura player_t no debe ser modificada aun\n\n");
							printf("Presione (Y/N): ");
							confirmation = getchar();
							printf("\n");
							
							if (confirmation == 'Y' || confirmation == 'y')
							{
								if (players_db_migrate() != DATABASE_ERROR)
								{
									printf("Migracion exitosa! Ahora recuerde:\n");
									printf("a) Agregar el nuevo miembro a la estructura player_t en el .c y en el .h\n");
									printf("b) Inicializar el nuevo miembro en la funcion players_db_create\n");
								}
								else
								{
									printf("Migracion fallida!\n");
								}
							}
							else
							{
								printf("Migracion abortada!\n");
							}
							break;
							
							
			case SALIR:
							break;
				
				
			default:
							printf("La opcion ingresada no es valida\n");
							break;
		}
		
		printf("****************************************************************************\n\n");
	}
		
	return 0;
}


void print_table(uint8_t category)
{
	player_t players_list[PLAYERS_MAX];
	int8_t players_num=0;
	uint8_t indexer=0;
	
	players_num = players_db_get_all(players_list);
	
	if (players_num != DATABASE_ERROR)
	{		
		switch(category)
		{			
			case CASH:
						printf("   Nombre\tPesos\n\n");
						qsort(players_list, players_num, sizeof(player_t), compare_cash);
					
						for (indexer=0 ; indexer < players_num ; indexer++)
						{
							printf("%"PRIu8") %-10s\t%"PRIu32"\n", indexer+1, players_list[indexer].name, players_list[indexer].cash);
						}
						break;
					
					
			case WINS:
						printf("   Nombre\tVictorias\n\n");
						qsort(players_list, players_num, sizeof(player_t), compare_wins);
							
						for (indexer=0 ; indexer < players_num ; indexer++)
						{
							printf("%"PRIu8") %-10s\t%"PRIu32"\n", indexer+1, players_list[indexer].name, players_list[indexer].wins);
						}
						break;
					
					
			default:
						break;
		}
	}
	else
	{
		printf ("Error en la base de datos!\n");
	}	
}


int compare_cash(const void * player_1, const void * player_2)
{
	int ret=0;
	player_t *p1 = (player_t *)player_1;
	player_t *p2 = (player_t *)player_2;
	
	if (p1->cash < p2->cash)
	{
		ret = 1;
	}
	else if(p1->cash > p2->cash)
	{
		ret = -1;
	}
	
	return ret;
}


int compare_wins(const void * player_1, const void * player_2)
{
	int ret=0;
	player_t *p1 = (player_t *)player_1;
	player_t *p2 = (player_t *)player_2;
	
	if (p1->wins < p2->wins)
	{
		ret = 1;
	}
	else if(p1->wins > p2->wins)
	{
		ret = -1;
	}
	
	return ret;
}