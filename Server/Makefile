
## FILES

EXEC		=	server

SRC		=	main			\
			Application		\
			Config			\
			MagicHeader		\
			Header			\
			Log			\
			Coord			\
			Client			\
			Network			\
			TcpClient		\
			ManageDB		\
			libIni/INIReader	\
			libIni/ini

SRC_EXT		=	.cpp

# DIRECTORIES

OBJ_DIR		=	obj
SRC_DIR		=	src
BIN_DIR		=	.

## COMPILATION

INC_DIR		=	include			\
			include/libIni		\
			libJsoncpp/include	\

CFLAGS		=	-W 		\
			-Wall		\
			-Wextra		\
			-Werror		\
			-std=c++11	\
			-g3

MAKED		=	make --no-print-directory -C

## LINKAGE

LIB		=	boost_system

LIB_DIR		=	/usr/lib/

LIB_FLAGS	=	-ljsoncpp	\
			-lmongoclient

## RUN

CXX		=	g++
RM		=	rm -f
MD		=	mkdir -p

##------------------
##-INTERNAL RULES---
##------------------

## files

OBJ		=	$(addsuffix .o, $(SRC))
_SRC		=	$(addsuffix $(SRC_EXT), $(SRC))

# directories

INC_DIR		:=	$(addsuffix /, $(INC_DIR))
OBJ_DIR		:=	$(addsuffix /, $(OBJ_DIR))
SRC_DIR		:=	$(addsuffix /, $(SRC_DIR))
BIN_DIR		:=	$(addsuffix /, $(BIN_DIR))

# compilation arguments

INC_COMP	=	$(addprefix -I, $(INC_DIR))
OBJ_COMP	=	$(addprefix $(OBJ_DIR), $(OBJ))
SRC_COMP	=	$(addprefix $(SRC_DIR), $(_SRC))
EXEC_COMP	=	$(addprefix $(BIN_DIR), $(EXEC))

# linkage arguments

LIB		:=	$(addprefix -l, $(LIB))
LIB_DIR		:=	$(addprefix -L, $(addsuffix /, $(LIB_DIR)))

## flags

CFLAGS		+=	$(INC_COMP)

LIB_FLAGS	+=	$(LIB_DIR)	\
			$(LIB)

# misc

MAKE_OBJ	=	$(addprefix $(OBJ_DIR), %.o) : $(addprefix $(addprefix $(SRC_DIR), %), $(SRC_EXT))




## COMMANDS

all		:	$(EXEC)

$(EXEC)		:	$(OBJ_COMP)
			@$(MD) $(BIN_DIR)
			@echo
			@echo -en "\033[33mLinkage : \033[0m$(EXEC)"
			@$(CXX) $(OBJ_COMP) -o $@ $(LIB_FLAGS)
			@echo -e "\t\t\t\t\033[32mDone\033[0m\n"

$(OBJ_COMP)	:	$(MAKE_OBJ)
			@$(MD) $(OBJ_DIR)
			@for dir in `cd $(SRC_DIR); ls -d */ 2> /dev/null`; \
			do \
			if [ ! -d "$(OBJ_DIR)$$dir" ]; then \
				$(MD) $(OBJ_DIR)$$dir; \
			fi \
			done
			@echo -en "\033[36mCompilation : \033[0m$<"
			@$(CXX) -c $< -o $@ $(CFLAGS)
			@./conf/printTheDone.sh $<

clean		:
			@echo -en "\033[31mErase object files\033[0m"
			@$(RM) $(OBJ_COMP)
			@echo -e "\t\t\t\t\033[32mDone\033[0m\n"

fclean		:	clean
			@echo -en "\033[31mErase exec file\033[0m"
			@$(RM) $(EXEC)
			@echo -e "\t\t\t\t\t\033[32mDone\033[0m\n"

re		:	fclean all

.PHONY		:	all clean fclean re
