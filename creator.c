
//**************************************INCLUDES**************************************
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
//************************************************************************************


//**************************************DEFINES***************************************
#define NAME_MAX_LENGTH	20
#define	OK				0
#define ERROR			-1
//************************************************************************************


//**************************************DATA TYPES************************************
typedef struct player_tag
{
	char name[NAME_MAX_LENGTH];
	uint32_t cash;
	uint32_t wins;
}player_t;
//************************************************************************************


//**************************************PROTOTYPES************************************
char create_player (FILE * fp, char * name, uint32_t cash, uint32_t wins);
//************************************************************************************

int main (void)
{
	FILE *fp;
	char response=OK;
	
	fp = fopen ("db.bin", "wb");
	
	response += create_player(fp, "Charly", 6, 1);
	response += create_player(fp, "Diegote", 18, 2);
	response += create_player(fp, "Marce", 36, 4);
	
	if (response == OK)
	{
		printf("Archivo creado exitosamente!\n");
	}
	else
	{
		printf("Error al intentar crear el archivo\n");
	}
	
	fclose(fp);
		
	getchar();
	return 0;
}


char create_player (FILE * fp, char * name, uint32_t cash, uint32_t wins)
{
	char ret=ERROR;
	player_t new_player;
		
	if (strlen(name) < NAME_MAX_LENGTH)
	{
		ret = OK;
		strcpy(new_player.name, name);
		new_player.wins = wins;
		new_player.cash = cash;
		
		fwrite(&new_player, sizeof(player_t), 1, fp);
	}
	
	return ret;
}































