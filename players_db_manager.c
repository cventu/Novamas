
//**********************************************INCLUDES**********************************************
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
//****************************************************************************************************


//**********************************************DEFINES***********************************************
#define PLAYERS_DB			"\\\\PUMP\\Ingenieria\\Proyectos Terminados\\MUX04x02\\db_j.bin"
//#define PLAYERS_DB			"db_j.bin"	// Testing DB

#define NAME_MAX_LENGTH		20

#define PLAYER_DATA_LOADED	 0
#define PLAYER_DATA_UPDATED	 0
#define	PLAYER_CREATED		 0
#define	PLAYER_NOT_FOUND	-1
#define	INVALID_POSITION	-1
#define NAME_TOO_LONG		-1
#define DATABASE_ERROR		-2
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
int16_t players_db_find(char * player_name);
int8_t players_db_get(uint16_t player_position, player_t * player_data);
int8_t players_db_update(uint16_t player_position, const player_t * player_data);
int8_t players_db_get_all(player_t * players_array);
int8_t players_db_create(char * name);
//****************************************************************************************************

int16_t players_db_find(char * player_name)
{
	FILE *fp;
	uint16_t count=0;
	int16_t player_found=PLAYER_NOT_FOUND;
	player_t data_read;
	
	fp = fopen (PLAYERS_DB, "rb");	// Opens the file only for reading
	
	if (fp == NULL)
	{
		player_found = DATABASE_ERROR;
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


int8_t players_db_get(uint16_t player_position, player_t * player_data)
{
	FILE *fp;
	int8_t ret;
	
	fp = fopen (PLAYERS_DB, "rb");	// Opens the file only for reading
	
	if (fp == NULL)
	{
		ret = DATABASE_ERROR;
	}
	else
	{
		fseek(fp, player_position * sizeof(player_t), SEEK_SET);
		fread(player_data, sizeof(player_t), 1, fp);
		
		if (feof(fp) == 0)
		{
			ret = PLAYER_DATA_LOADED;
		}
		else
		{
			ret = INVALID_POSITION;
		}
	}
	
	fclose(fp);
	
	return ret;
}


int8_t players_db_update(uint16_t player_position, const player_t * player_data)
{
	FILE *fp;
	int8_t ret;
	
	fp = fopen (PLAYERS_DB, "rb+");	// Opens the file for reading and writing
	
	if (fp == NULL)
	{
		ret = DATABASE_ERROR;
	}
	else
	{
		fseek(fp, player_position * sizeof(player_t), SEEK_SET);
		fwrite(player_data, sizeof(player_t), 1, fp);
		ret = PLAYER_DATA_UPDATED;
	}
	
	fclose(fp);
	
	return ret;
}


int8_t players_db_get_all(player_t * players_array)	// Fills array with all players data and returns the number of players read.
{
	int8_t response=0;
	uint8_t indexer=0;
	player_t player_read;
	
	while (response != INVALID_POSITION && response != DATABASE_ERROR)
	{
		response = players_db_get(indexer, &player_read);
		
		if (response != INVALID_POSITION && response != DATABASE_ERROR)
		{
			players_array[indexer] = player_read;
			indexer++;
		}
		else if (response == DATABASE_ERROR)
		{
			indexer = DATABASE_ERROR;
		}
	}
	
	return indexer;
}


int8_t players_db_create(char * name)
{
	FILE *fp;
	char ret;
	player_t new_player;
	
	fp = fopen (PLAYERS_DB, "ab");
	
	if (fp == NULL)
	{
		ret = DATABASE_ERROR;
	}
	else if (strlen(name) >= NAME_MAX_LENGTH)
	{
		ret = NAME_TOO_LONG;
	}
	else
	{
		strcpy(new_player.name, name);
		new_player.wins = 0;
		new_player.cash = 0;
		fwrite(&new_player, sizeof(player_t), 1, fp);
		ret = PLAYER_CREATED;
	}
	
	fclose(fp);
	
	return ret;
}