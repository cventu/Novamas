
//**********************************************INCLUDES**********************************************
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
//****************************************************************************************************


//**********************************************DEFINES***********************************************
#define NAME_MAX_LENGTH	20
#define	OK				0
#define ERROR			-1
#define PLAYERS_DB		"\\\\PUMP\\Ingenieria\\Proyectos Terminados\\MUX04x02\\db_j.bin"
//****************************************************************************************************


//**********************************************DATA TYPES********************************************
typedef struct player_tag
{
	char name[NAME_MAX_LENGTH];
	uint32_t cash;
	uint32_t wins;
}player_t;
//****************************************************************************************************


//**********************************************PROTOTYPES********************************************
int8_t create_player (char * name);
//****************************************************************************************************

int main (void)
{
	char name_aux[NAME_MAX_LENGTH];
	
	printf("Ingrese el nombre del jugador: ");
	fgets(name_aux, NAME_MAX_LENGTH, stdin);
	strtok(name_aux, "\n");	// fgets stores new line character at the end. Strtok is used to delete that character
	
	create_player(name_aux);
		
	getchar();
	return 0;
}


int8_t create_player (char * name)
{
	FILE *fp;
	char ret=ERROR;
	player_t new_player;
	
	fp = fopen (PLAYERS_DB, "ab");
	
	if (fp == NULL)
	{
		perror("Error: ");
		return(-1);
	}
	else if (strlen(name) >= NAME_MAX_LENGTH)
	{
		printf("El nombre del jugador es demasiado largo\n");
	}
	else
	{
		strcpy(new_player.name, name);
		new_player.wins = 0;
		new_player.cash = 0;
		fwrite(&new_player, sizeof(player_t), 1, fp);
		printf("Jugador creado exitosamente!!\n");
		ret = OK;
	}
	
	fclose(fp);
	
	return ret;
}
