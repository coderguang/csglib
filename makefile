############################################
#makefile for GameWorld
#author	royalchen
#date	2017-07-27
############################################

#set project dir
PROJECT_DIR		:=$(shell pwd)
PROJECT_BIN_DIR	:=../bin/
PROJECT_LIB_DIR	:=../lib/
LIB_NAME		:= libEngine.a

#set build param
CC	:= clang++
CFLAG	:= -I$(PROJECT_DIR) -std=c++11 -lstdc++ -D _DEBUG
LIBS	:= -lMessage
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


define build_obj
$1:$2
	$(CC) -c $(CFLAG) -o $1 $2 
endef

$(foreach tmp,$(CUR_OBJ_EX),$(eval $(call build_obj,$(tmp).o,$(tmp).cpp)))

.PHONY:all
all:$(LIB_NAME)

$(LIB_NAME):$(CUR_OBJ)
	@echo "start build $(LIB_NAME).."
	ar cr $(PROJECT_LIB_DIR)$@ $^ 
	@echo "build $(LIB_NAME) success!"





#for test
printPath :
	@echo -e $(GREEN) PROJECT_DIR		$(BLACK)	= $(PROJECT_DIR)
	#@echo -e $(GREEN) PROJECT_BIN_DIR	$(BLACK)	=$(PROJECT_BIN_DIR)
	#@echo -e $(GREEN) PROJECT_LIB_DIR	$(BLACK)	=$(PROJECT_LIB_DIR)
	#@echo -e $(GREEN) CUR_SRC_DIR		$(BLACK)	=$(CUR_SRC_DIR)
	#@echo -e $(GREEN) CUR_SRC			$(BLACK)	=$(CUR_SRC)
	#@echo -e $(GREEN) CUR_H				$(BLACK)	=$(CUR_H)
	#@echo -e $(GREEN) CUR_CPP			$(BLACK)	=$(CUR_CPP) $(BLACK)
	#@echo -e $(GREEN) CUR_OBJ			$(BLACK)	=$(CUR_OBJ) $(BLACK)
	#@echo -e $(GREEN) CUR_OBJ_EX		$(BLACK)	=$(CUR_OBJ_EX) $(BLACK)
	#@echo -e $(GREEN) CUR_CPP_DIR			$(BLACK)	=$(CUR_CPP_DIR) $(BLACK)
	#@echo -e $(GREEN) CUR_H_PRE			$(BLACK)	=$(CUR_H_PRE) $(BLACK)
	@echo -e $(GREEN) BUILD_STR			$(BLACK)	=$(BUILD_STR) $(BLACK)

clean :
	@$(RM) -rf $(CUR_OBJ) $(PROJECT_LIB_DIR)$(LIB_NAME) $(CUR_DEP)
