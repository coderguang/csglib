############################################
#makefile for GameWorld
#author	royalchen
#date	2017-07-27
############################################

BLACK = "\e[33;0m"
RED  =  "\e[31;1m"
GREEN = "\e[32;1m"
YELLOW = "\e[33;3m"
BLUE  = "\e[34;1m"
PURPLE = "\e[35;1m"
CYAN  = "\e[36;1m"
WHITE = "\e[37;1m"



#set project dir
PROJECT_DIR		:=$(shell pwd)
PROJECT_BIN_DIR	:=../bin/
PROJECT_LIB_DIR	:=../lib/
LIB_NAME		:= libEngine.a

#set build param
CC	:= clang++
CFLAG	:= -I$(PROJECT_DIR) -std=c++11 -D _DEBUG
LIBS	:= 
RM :=rm


#set file dir
SEARCH_FILE	= $(wildcard $(subdir)/*.h $(subdir)/*.cpp)
CUR_SRC_DIR	= $(shell ls -AxR $(PROJECT_DIR)|grep ":"|tr -d ':')
CUR_SRC 	:= $(foreach subdir,$(CUR_SRC_DIR),$(SEARCH_FILE))
CUR_H		:= $(filter %.h, $(CUR_SRC))
CUR_CPP 	:= $(filter %.cpp, $(CUR_SRC))
CUR_CPP_DIR	:= $(dir $(CUR_CPP))
CUR_CPP_DIR	:= $(sort $(CUR_CPP_DIR))
CUR_H_DIR	:= $(dir $(CUR_H))
CUR_H_PRE	:= $(addprefix -I,$(CUR_H_DIR))

#set obj dir
CUR_OBJ		:=$(strip $(CUR_CPP:.cpp=.o))
CUR_OBJ_EX	:=$(patsubst %.o,%,$(CUR_OBJ))
CUR_DEP		:=$(strip $(CUR_CPP:.cpp=.d) $(CUR_H:.h=.d))


define build_obj
$1:$2
	$(CC) -c $(CFLAG) -o $1 $2 
endef

define build_dep
$2:$1
	$(CC) $(CFLAG) -MM $1 >$2
endef


$(foreach tmp,$(CUR_OBJ_EX),$(eval $(call build_dep,$(tmp).cpp,$(tmp).d)))

$(foreach tmp,$(CUR_OBJ_EX),$(eval $(call build_obj,$(tmp).o,$(tmp).cpp)))

.PHONY:all
all:$(LIB_NAME)

$(LIB_NAME):$(CUR_OBJ)
	test -d $(PROJECT_LIB_DIR) || mkdir -p $(PROJECT_LIB_DIR)
	test -d $(PROJECT_BIN_DIR) || mkdir -p $(PROJECT_BIN_DIR)
	@echo "start build $(LIB_NAME).."
	ar cr $(PROJECT_LIB_DIR)$@ $^ 
	@echo -e $(GREEN) "build $(LIB_NAME) success!" $(BLACK)


.PHONY:clean
clean :
	@$(RM) -rf $(CUR_OBJ) $(PROJECT_LIB_DIR)$(LIB_NAME) $(CUR_DEP)
