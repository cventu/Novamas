
//**********************************************DEFINES***********************************************
#define NAME_MAX_LENGTH			20
#define ID_LENGTH			 			 6	
#define MAX_PASSWORD_LENGTH	20

#define PLAYER_DATA_LOADED	 0
#define PLAYER_DATA_UPDATED	 0
#define	PLAYER_CREATED			 0
#define	PLAYER_NOT_FOUND	  -1
#define	INVALID_POSITION	  -1
#define NAME_TOO_LONG			  -1
#define DATABASE_ERROR		  -2
#define	PASSWORD_TOO_LONG		-3
//****************************************************************************************************


//**********************************************DATA TYPES********************************************
typedef struct player_tag
{
	char name[NAME_MAX_LENGTH];
	uint32_t cash;
	uint32_t wins;
	char id[ID_LENGTH+1];
	char password[MAX_PASSWORD_LENGTH+1];
	uint8_t is_admin;
}player_t;
//****************************************************************************************************


//**********************************************PROTOTYPES********************************************
int16_t players_db_find(char * player_id);
int8_t players_db_get(uint16_t player_position, player_t * player_data);
int8_t players_db_update(uint16_t player_position, const player_t * player_data);
int8_t players_db_get_all(player_t * players_array);
int8_t players_db_create(char * name, char * id, char * password);
int8_t players_db_migrate(void);
//****************************************************************************************************