	
COMPILADOR=gcc
C_ARGS= -Wall -c
OBJS = novamas.o players_db_manager.o

ejecutable: $(OBJS)
	$(COMPILADOR) -o Novamas $(OBJS) 

novamas.o: novamas.c players_db_manager.h
	$(COMPILADOR) $(C_ARGS) $<
	
players_db_manager.o: players_db_manager.c
	$(COMPILADOR) $(C_ARGS) $<
	
clean:
	del $(OBJS)
	del Novamas.exe
	del db_j.bin