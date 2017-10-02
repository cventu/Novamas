
//******************************************INCLUDES******************************************
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
//********************************************************************************************


//******************************************DEFINES*******************************************
#define NAME_MAX_LENGTH		20
#define	PLAYER_NOT_FOUND	-1

#define CONSULTAR	1
#define	INGRESAR	2
#define	SALIR		3
//********************************************************************************************


//******************************************DATA TYPES****************************************
typedef struct player_tag
{
	char name[NAME_MAX_LENGTH];
	uint32_t cash;
	uint32_t wins;
}player_t;
//********************************************************************************************


//******************************************DEFINES*******************************************
int16_t find_player(char * player_name);
void get_player_data(uint16_t player_position, player_t * player_data);
void update_player_data(uint16_t player_position, const player_t * player_data);
//********************************************************************************************

int main (void)
{
	int16_t position=0;
	player_t player_data;	
	char name_aux[NAME_MAX_LENGTH];
	uint16_t prize=0;
	uint16_t option=0;
	
	while (option != SALIR)
	{
		printf("1) Ver datos de jugador\n2) Ingresar ganador\n3) Salir\n\nSeleccione la opcion deseada: ");
		scanf("%"SCNu16, &option);
		fflush(stdin);
		printf("\n\n");
		
		switch (option)
		{
			case CONSULTAR:
							printf("Jugador: ");
							fgets(name_aux, NAME_MAX_LENGTH, stdin);
							strtok(name_aux, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
							printf("\n\n");
							
							position = find_player(name_aux);
						
							if (position == PLAYER_NOT_FOUND)
							{
								printf("El jugador ingresado no existe!\n\n\n");
							}
							else
							{
								get_player_data(position, &player_data);
								printf ("Nombre: %s\n", player_data.name);
								printf ("Victorias: %"PRIu32"\n", player_data.wins);
								printf ("Dinero: %"PRIu32"\n", player_data.cash);
							}
							break;
				
				
			case INGRESAR:
							printf("Ganador: ");
							fgets(name_aux, NAME_MAX_LENGTH, stdin);
							strtok(name_aux, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
							printf("\n\n");
							
							position = find_player(name_aux);
						
							if (position == PLAYER_NOT_FOUND)
							{
								printf("El jugador ingresado no existe!\n\n\n");
							}
							else
							{
								printf("Monto ganado: ");
								scanf("%"SCNu16, &prize);
								fflush(stdin);
						
								get_player_data(position, &player_data);
								player_data.wins++;
								player_data.cash += prize;
								update_player_data(position, &player_data);
							}
							break;
				
				
			case SALIR:
									
							break;
				
				
			default:
							printf("La opcion ingresada no es valida\n\n\n");
							break;
		}
	}
		
	return 0;
}


int16_t find_player(char * player_name)
{
	FILE *fp;
	uint16_t count=0;
	int16_t player_found = PLAYER_NOT_FOUND;
	player_t data_read;
	
	fp = fopen ("db.bin", "rb");	// Opens the file only for reading
	
	if (fp == NULL)
	{
		printf("Error en la base de datos!\n");
	}
	else
	{
		while(feof(fp) == 0 && player_found == PLAYER_NOT_FOUND)
		{
			fread(&data_read, sizeof(player_t), 1, fp);
			
			if (strcmp(player_name, data_read.name) == 0)
			{
				player_found = count;
			}
			else
			{
				count++;
			}
		}
	}
	
	fclose(fp);
		
	return player_found;
}


void get_player_data(uint16_t player_position, player_t * player_data)
{
	FILE *fp;
	
	fp = fopen ("db.bin", "rb");	// Opens the file only for reading
	
	if (fp == NULL)
	{
		printf("Error en la base de datos!\n");
	}
	else
	{
		fseek(fp, player_position * sizeof(player_t), SEEK_SET);
		fread(player_data, sizeof(player_t), 1, fp);
	}
	
	fclose(fp);
}


void update_player_data(uint16_t player_position, const player_t * player_data)
{
	FILE *fp;
	
	fp = fopen ("db.bin", "rb+");	// Opens the file for reading and writing
	
	if (fp == NULL)
	{
		printf("Error en la base de datos!\n");
	}
	else
	{
		fseek(fp, player_position * sizeof(player_t), SEEK_SET);
		fwrite(player_data, sizeof(player_t), 1, fp);
	}
	
	fclose(fp);
}












