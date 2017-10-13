
//**********************************************INCLUDES**********************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
//****************************************************************************************************


//**********************************************DEFINES***********************************************
#define NAME_MAX_LENGTH		20
#define	PLAYER_NOT_FOUND	-1
#define PLAYERS_MAX			10
#define CASH				0
#define	WINS				1
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


//**********************************************DEFINES***********************************************
int16_t find_player(char * player_name);
int8_t get_player_data(uint16_t player_position, player_t * player_data);
uint8_t get_all_players(player_t * players_array);
int compare_cash(const void * player_1, const void * player_2);
int compare_wins(const void * player_1, const void * player_2);
void print_table(uint8_t category);
//****************************************************************************************************

int main (void)
{	
	print_table(CASH);
	printf("\n");
	print_table(WINS);
	
	getchar();
	return 0;
}


int16_t find_player(char * player_name)
{
	FILE *fp;
	uint16_t count=0;
	int16_t player_found = PLAYER_NOT_FOUND;
	player_t data_read;
	
	fp = fopen (PLAYERS_DB, "rb");	// Opens the file only for reading
	
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


int8_t get_player_data(uint16_t player_position, player_t * player_data)
{
	FILE *fp;
	int8_t ret=PLAYER_NOT_FOUND;
	
	fp = fopen (PLAYERS_DB, "rb");	// Opens the file only for reading
	
	if (fp == NULL)
	{
		printf("Error en la base de datos!\n");
	}
	else
	{
		fseek(fp, player_position * sizeof(player_t), SEEK_SET);
		fread(player_data, sizeof(player_t), 1, fp);
		
		if (feof(fp) == 0)
		{
			ret = 0;
		}
	}
	
	fclose(fp);
	
	return ret;
}


uint8_t get_all_players(player_t * players_array)	// Fills array with all players data and returns the number of players read.
{
	int8_t response=0;
	uint8_t indexer=0;
	player_t player_read;
	
	while (response != -1)
	{
		response = get_player_data(indexer, &player_read);
		
		if (response != -1)
		{
			players_array[indexer] = player_read;
			indexer++;
		}
	}
	
	return indexer;
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


void print_table(uint8_t category)
{
	player_t players_list[PLAYERS_MAX];
	uint8_t players_num=0;
	uint8_t indexer=0;
	
	players_num = get_all_players(players_list);
	printf("\n\n");
	
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
