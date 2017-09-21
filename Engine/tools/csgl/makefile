
OBJ	:= csgl
#TOOLS_DIR	:=../../../../Public/tools/
TOOLS_DIR	:=./
SRCC_LEX	:=$(OBJ).yy.c
SRCC_YACC	:=$(OBJ).tab.c
SRCH_YACC	:=$(OBJ).tab.h
SRCO_LEX	:=$(OBJ).yy.o
SRCO_YACC	:=$(OBJ).tab.o
SRCH_CSGL	:=csgl.hpp
SRCL	:=$(OBJ).l	
SRCY	:=$(OBJ).y
CC	:= clang++
CFLAGS	:= -std=c++11 -lstdc++ -D _Debug -g -w
LIBS	:=
LEX	:= flex
YACC :=bison


.PHONY:all
all:$(OBJ)

$(OBJ):$(SRCO_YACC) $(SRCO_LEX) 
	$(CC) $(CFLAGS) $^ -o $(TOOLS_DIR)$@ $(LIBS)
	#rm -rf *.h *.c *.o *.output *.cpp

	
$(SRCO_LEX):$(SRCC_LEX) $(SRCH_CSGL)
	$(CC) -c $(CFLAGS) $< $(LIBS)

$(SRCO_YACC):$(SRCC_YACC) $(SRCH_CSGL)
	$(CC) -c $(CFLAGS) $< $(LIBS)

$(SRCC_YACC) $(SRCH_YACC):$(SRCY)
	$(YACC) -d -t --verbose $^


$(SRCC_LEX):$(SRCL)
	$(LEX) -o $@ $^ 
	
.PHONY:clean
clean:
	rm -rf $(OBJ) $(SRCC) *.h *.c *.o core.* *.output *.gch *.cpp

.PHONY:cleanobj
cleanobj:
	rm -rf *.o

