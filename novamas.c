
//**********************************************INCLUDES**********************************************
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include "players_db_manager.h"
//****************************************************************************************************


//**********************************************DEFINES***********************************************
#define CASH				  0
#define	WINS				  1

#define LOGIN_FAILED	0
#define LOGIN_SUCCESS	1

#define PLAYERS_MAX	 10

#define CONSULTAR		  1
#define	GANADOR			  2
#define	CREAR				  3
#define TABLAS			  4
#define	MIGRAR			  5
#define MODIFICAR		  6
#define	SALIR				  7

#define	ALIAS_MOD		  1
#define	CASH_MOD		  2
#define	WINS_MOD		  3
#define	ADMIN_MOD		  4
#define PASS_MOD		  5
#define SALIR_MOD		  6

#define CHANGE_ALIAS	1
#define CHANGE_PASS		2
#define	TABLES				3
#define	QUIT					4
//****************************************************************************************************


//**********************************************PROTOTYPES********************************************
uint8_t user_login(player_t * user_data, int16_t * position);
uint8_t admin_menu(void);
uint8_t player_menu(player_t * player_data, int16_t position);
void print_table(uint8_t category);
int compare_cash(const void * player_1, const void * player_2);
int compare_wins(const void * player_1, const void * player_2);
//****************************************************************************************************

int main (void)
{
	player_t player_data;
	uint8_t quit=0;
	uint8_t login_result;
	int16_t position=0;
	
	login_result = user_login(&player_data, &position);
		
	while(login_result == LOGIN_SUCCESS && (!quit))
	{
		if (player_data.is_admin)
		{
			quit = admin_menu();
		}
		else
		{
			quit = player_menu(&player_data, position);
		}
	}
			
	return 0;
}


uint8_t user_login(player_t * user_data, int16_t * position)
{
	char id[ID_LENGTH+1];					
	char password[MAX_PASSWORD_LENGTH+1];
	uint8_t result = LOGIN_FAILED;
		
	printf("Usuario: ");
	fgets(id, ID_LENGTH+2, stdin);
	fflush(stdin);
	strtok(id, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
	
	printf("Password: ");
	fgets(password, MAX_PASSWORD_LENGTH+1, stdin);
	fflush(stdin);
	strtok(password, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
	printf("\n");
	
	*position = players_db_find(id);
										
	if (*position == PLAYER_NOT_FOUND)
	{
		printf("Usuario o password incorrecto!\n");
		getchar();
	}
	else if (*position == DATABASE_ERROR)
	{
		printf("Error en la base de datos!\n");
	}
	else if (players_db_get(*position, user_data) != DATABASE_ERROR)
	{
		if (strcmp(password, user_data->password) == 0)
		{
			result = LOGIN_SUCCESS;
		}
		else
		{
			printf("Usuario o password incorrecto!\n");
			getchar();
		}
	}
	else
	{
		printf("Error en la base de datos!\n");
	}
	
	return result;
}


uint8_t admin_menu(void)
{
	int16_t position=0;
	uint16_t option=0;
	char id[ID_LENGTH+1];
	player_t player_data;
	uint16_t prize=0;
	char alias[ALIAS_MAX_LENGTH];
	char password[MAX_PASSWORD_LENGTH+1];
	uint8_t confirmation=0;
	uint8_t player_modified=0;
	uint8_t ret=0;
	
	printf("1) Ver datos de jugador\n2) Ingresar ganador\n3) Crear jugador\n4) Ver tablas\n5) Migrar DB\n6) Modificar datos de jugador\n7) Salir\n\nSeleccione la opcion deseada: ");
	scanf("%"SCNu16, &option);
	fflush(stdin);
	printf("\n");
	
	switch (option)
	{
		case CONSULTAR:
										printf("ID: ");
										fgets(id, ID_LENGTH+2, stdin);
										fflush(stdin);
										strtok(id, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
										printf("\n");
										
										position = players_db_find(id);
									
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
											printf("ID: %s\n", player_data.id);
											printf ("Alias: %s\n", player_data.alias);
											printf ("Victorias: %"PRIu32"\n", player_data.wins);
											printf ("Dinero: %"PRIu32"\n", player_data.cash);
											
											if (player_data.is_admin)
											{
												printf("Admin: Si\n");
											}
											else
											{
												printf("Admin: No\n");
											}
										}
										else
										{
											printf("Error en la base de datos!\n");
										}
										break;
			
			
		case GANADOR:
										printf("Ganador: ");
										fgets(id, ID_LENGTH+2, stdin);
										fflush(stdin);
										strtok(id, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
										printf("\n");
										
										position = players_db_find(id);
									
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
										printf("Ingrese el alias del jugador: ");
										fgets(alias, ALIAS_MAX_LENGTH, stdin);
										fflush(stdin);
										strtok(alias, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
										printf("Ingrese el ID del jugador: ");
										fgets(id, ID_LENGTH+1, stdin);
										fflush(stdin);
										strtok(id, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
										printf("Ingrese el password del jugador: ");
										fgets(password, MAX_PASSWORD_LENGTH, stdin);
										fflush(stdin);
										strtok(password, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
										printf("\n");
										
										switch (players_db_create(alias, id, password))
										{
											case DATABASE_ERROR:
																					printf("Error en la base de datos!\n");
																					break;
																
																
											case ALIAS_TOO_LONG:
																					printf("El alias ingresado es demasiado largo!\n");
																					break;
																					
																					
											case PASSWORD_TOO_LONG:
																					printf("El password ingresado es demasiado largo!\n");
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
										printf("Agrego el nuevo miembro en la estructura new_player_format en el .c?\n");
										printf("Compilo?\n");
										printf("Recuerde que la estructura player_t no debe ser modificada aun\n\n");
										printf("Presione [S/N]: ");
										confirmation = getchar();
										printf("\n");
										
										if (confirmation == 'S' || confirmation == 's')
										{
											if (players_db_migrate() != DATABASE_ERROR)
											{
												printf("Migracion exitosa! Ahora recuerde:\n");
												printf("a) Agregar el nuevo miembro a la estructura player_t en el .c y en el .h\n");
												printf("b) Agregar el nuevo miembro en la funcion players_db_migrate\n");
												printf("c) Inicializar el nuevo miembro en la funcion players_db_create\n");
												printf("d) Agregar el nuevo miembro en la opcion CONSULTAR datos del jugador en el menu\n");
												printf("e) Agregar el nuevo miembro en la opcion MODIFICAR datos del jugador en el menu\n");
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
										
										
		case MODIFICAR:	
										player_modified=0;
										printf("ID: ");
										fgets(id, ID_LENGTH+2, stdin);
										fflush(stdin);
										strtok(id, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
										printf("****************************************************************************\n\n");
										
										position = players_db_find(id);
									
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
											printf("1) Alias\n2) Dinero\n3) Victorias\n4) Administrador\n5) Password\n6) Salir\n\nSeleccione el parametro que desea modificar: ");
											scanf("%"SCNu16, &option);
											fflush(stdin);
											printf("\n");
											
											switch(option)
											{
												case ALIAS_MOD:
																			printf("Ingrese alias: ");
																			fgets(alias, ALIAS_MAX_LENGTH+1, stdin);
																			fflush(stdin);
																			strtok(alias, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
																			printf("\n");
																			strcpy(player_data.alias, alias);
																			player_modified=1;
																			break;
																	
																	
												case CASH_MOD:
																			printf("Ingrese dinero: ");
																			scanf("%"SCNu16, &prize);
																			fflush(stdin);
																			printf("\n");
																			player_data.cash = prize;
																			player_modified=1;
																			break;
																		
																		
												case WINS_MOD:
																			printf("Ingrese victorias: ");
																			scanf("%"SCNu16, &prize);
																			fflush(stdin);
																			printf("\n");
																			player_data.wins = prize;
																			player_modified=1;
																			break;
																		
																		
												case ADMIN_MOD:
																			printf("Privilegio de Administrador?\n");
																			printf("Presione [S/N]: ");
																			confirmation = getchar();
																			printf("\n");
																			
																			if (confirmation == 'S' || confirmation == 's')
																			{
																				player_data.is_admin = 1;
																				player_modified=1;
																			}
																			else if (confirmation == 'N' || confirmation == 'n')
																			{
																				player_data.is_admin = 0;
																				player_modified=1;
																			}
																			else
																			{
																				printf("Opcion invalida!\n");
																			}
																			break;
																			
																			
												case PASS_MOD:
																			printf("Ingrese password: ");
																			fgets(password, MAX_PASSWORD_LENGTH+1, stdin);
																			fflush(stdin);
																			strtok(password, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
																			printf("\n");
																			strcpy(player_data.password, password);
																			player_modified=1;
																			break;
																			
																
												case SALIR_MOD:
																			break;
																		
												
												default:
																			printf("La opcion ingresada no es valida\n");
																			break;
											}
											
											if (player_modified)
											{
												if (players_db_update(position, &player_data) == DATABASE_ERROR)
												{
													printf("Error en la base de datos!\n");
												}
												else
												{
													printf("Jugador modificado!\n");
												}
											}
										}
										else
										{
											printf("Error en la base de datos!\n");
										}
										break;
						
						
		case SALIR:
										ret=1;
										break;
			
			
		default:
										printf("La opcion ingresada no es valida\n");
										break;
	}
	
	printf("****************************************************************************\n\n");
	
	return ret;
}


uint8_t player_menu(player_t * player_data, int16_t position)
{
	uint16_t option=0;
	char alias[ALIAS_MAX_LENGTH];
	char aux_buffer_1[MAX_PASSWORD_LENGTH+2];	// This will be used temporary to store the password
	char aux_buffer_2[MAX_PASSWORD_LENGTH+2];	// This will be used temporary to store the password
	uint8_t ret=0;
	
	printf("1) Cambiar alias\n2) Cambiar password\n3) Ver tablas\n4) Salir\n\nSeleccione la opcion deseada: ");
	scanf("%"SCNu16, &option);
	fflush(stdin);
	printf("\n");
	
	switch (option)
	{
		case CHANGE_ALIAS:	
											printf("Ingrese nuevo alias: ");
											fgets(alias, ALIAS_MAX_LENGTH+2, stdin);
											fflush(stdin);
											strtok(alias, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
											printf("\n");
											strcpy(player_data->alias, alias);
											
											if (players_db_update(position, player_data) == DATABASE_ERROR)
											{
												printf("Error en la base de datos!\n");
											}
											else
											{
												printf("Alias actualizado!\n");
											}
											break;
										
										
		case CHANGE_PASS:	
											printf("Ingrese nuevo password: ");
											fgets(aux_buffer_1, MAX_PASSWORD_LENGTH+2, stdin);
											fflush(stdin);
											strtok(aux_buffer_1, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
											printf("\n");
											
											printf("Ingrese password nuevamente: ");
											fgets(aux_buffer_2, MAX_PASSWORD_LENGTH+2, stdin);
											fflush(stdin);
											strtok(aux_buffer_2, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
											printf("\n");
											
											if ( strcmp(aux_buffer_1, aux_buffer_2) == 0 )
											{
												if ( strlen(aux_buffer_1) <= MAX_PASSWORD_LENGTH )
												{
													strcpy(player_data->password, aux_buffer_1);
													
													if (players_db_update(position, player_data) == DATABASE_ERROR)
													{
														printf("Error en la base de datos!\n");
													}
													else
													{
														printf("Password actualizado!\n");
													}
												}
												else
												{
													printf("El password ingresado es demasiado largo. Intente nuevamente.\n");
												}
											}
											else
											{
												printf("Los passwords ingresados no coinciden. Intente nuevamente.\n");
											}
											break;
										
										
		case TABLES:	
											print_table(CASH);
											printf("\n");
											print_table(WINS);
											break;
						
						
		case QUIT:
											ret = 1;
											break;
			
			
		default:
											printf("La opcion ingresada no es valida\n");
											break;
	}
	
	printf("****************************************************************************\n\n");
	
	return ret;
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
								printf("   Alias\tPesos\n\n");
								qsort(players_list, players_num, sizeof(player_t), compare_cash);
							
								for (indexer=0 ; indexer < players_num ; indexer++)
								{
									printf("%"PRIu8") %-10s\t%"PRIu32"\n", indexer+1, players_list[indexer].alias, players_list[indexer].cash);
								}
								break;
					
					
			case WINS:
								printf("   Alias\tVictorias\n\n");
								qsort(players_list, players_num, sizeof(player_t), compare_wins);
									
								for (indexer=0 ; indexer < players_num ; indexer++)
								{
									printf("%"PRIu8") %-10s\t%"PRIu32"\n", indexer+1, players_list[indexer].alias, players_list[indexer].wins);
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