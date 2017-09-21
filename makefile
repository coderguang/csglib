############################################
#makefile for GameWorld
#author	royalchen
#date	2017-07-27
############################################

# Color setting
# *nix system tools defined. You needn't modify these vars below generally.
BLACK = "\e[33;0m"
RED  =  "\e[31;1m"
GREEN = "\e[32;1m"
YELLOW = "\e[33;3m"
BLUE  = "\e[34;1m"
PURPLE = "\e[35;1m"
CYAN  = "\e[36;1m"
WHITE = "\e[37;1m"


#设定项目路径
PROJECT_DIR		:=$(shell pwd)
PROJECT_BIN_DIR	:=$(PROJECT_DIR)/bin
PROJECT_LIB_DIR	:=$(PROJECT_DIR)/lib

#设定代码文件后缀
SEARCH_FILE	= $(wildcard $(subdir)/*.h $(subdir)/*.cpp)
CUR_SRC_DIR	= $(shell ls -AxR $(PROJECT_DIR)|grep ":"|tr -d ':')
CUR_SRC 	:= $(foreach subdir,$(CUR_SRC_DIR),$(SEARCH_FILE))
CUR_H		:= $(filter %.h, $(CUR_SRC))
CUR_CPP 	:= $(filter %.cpp, $(CUR_SRC))
CUR_CPP_DIR	:= $(dir $(CUR_CPP))
CUR_CPP_DIR	:= $(sort $(CUR_CPP_DIR))

#设定要编译的obj,链接辅助 .d 文件
CUR_OBJ		:=$(strip $(CUR_CPP:.cpp=.o))
CUR_DEP		:=$(strip $(CUR_CPP:.cpp=.d) $(CUR_H:.h=.d))

#创建目标文件夹
	
vpath %.h $(sort $(dir $(CUR_H)))

#test
$(CUR_OBJ) : $(CUR_CPP) $(CUR_H)
	clang++ -c $< -o $@ -I./ -I/usr/include/sys/ -I/usr/include/

lib.a : $(CUR_OBJ)
	ar cr $@ $(CUR_OBJ)


printVpath :
	@echo "test vpath"
	@echo -e $(GREEN) 	$(vpath %.h $(sort $(dir $(CUR_H))))	$(BLACK)
	
#for test
printPath :
	@echo -e $(GREEN) PROJECT_DIR		$(BLACK)	= $(PROJECT_DIR)
	@echo -e $(GREEN) PROJECT_BIN_DIR	$(BLACK)	=$(PROJECT_BIN_DIR)
	@echo -e $(GREEN) PROJECT_LIB_DIR	$(BLACK)	=$(PROJECT_LIB_DIR)
	@echo -e $(GREEN) PROJECT_INC_DIR	$(BLACK)	=$(PROJECT_INC_DIR)
	@echo -e $(GREEN) CUR_SRC_DIR		$(BLACK)	=$(CUR_SRC_DIR)
	@echo -e $(GREEN) CUR_SRC			$(BLACK)	=$(CUR_SRC)
	@echo -e $(GREEN) CUR_H				$(BLACK)	=$(CUR_H)
	@echo -e $(GREEN) CUR_CPP			$(BLACK)	=$(CUR_CPP) $(BLACK)
	@echo -e $(GREEN) CUR_OBJ			$(BLACK)	=$(CUR_OBJ) $(BLACK)
	@echo -e $(GREEN) CUR_DEP			$(BLACK)	=$(CUR_DEP) $(BLACK)
	@echo -e $(GREEN) CFXX			$(BLACK)	=$(CFXX) $(BLACK)
	@echo -e $(GREEN) CUR_CPP_DIR			$(BLACK)	=$(CUR_CPP_DIR) $(BLACK)