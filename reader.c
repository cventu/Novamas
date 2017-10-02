
//**************************************INCLUDES**************************************
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
//************************************************************************************


//**************************************DEFINES***************************************
#define NAME_MAX_LENGTH	20
//************************************************************************************


//**************************************DATA TYPES************************************
typedef struct player_tag
{
	char name[NAME_MAX_LENGTH];
	uint32_t cash;
	uint32_t wins;
}player_t;
//************************************************************************************

int main (void)
{
	FILE *fp;
	player_t data_read;
	
	fp = fopen ("db.bin", "rb+");
	
	if (fp == NULL)
	{
		printf("File does not exist!\n");
	}
	else
	{
		fread(&data_read, sizeof(player_t), 1, fp);
		printf("Nombre: %s\n", data_read.name);
		fread(&data_read, sizeof(player_t), 1, fp);
		printf("Nombre: %s\n", data_read.name);
		fread(&data_read, sizeof(player_t), 1, fp);
		printf("Nombre: %s\n", data_read.name);
	}
	
	fclose(fp);
		
	getchar();
	return 0;
}