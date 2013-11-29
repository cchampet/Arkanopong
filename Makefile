CC = gcc -g
CFLAGS = -Wall -ansi
LDFLAGS = -lm -lSDL -lGLU -lGL -lSDL_image

APP_BIN = arkanpong

SRC_PATH = src
OBJ_PATH = obj
INC_PATH = -I include
BIN_PATH = bin
LIB_PATH = lib

SRC_FILES = $(shell find $(SRC_PATH) -type f -name '*.c')
OBJ_FILES = $(patsubst $(SRC_PATH)/%.c,$(OBJ_PATH)/%.o, $(SRC_FILES))

DIRNAME  = $(shell basename $$PWD)
BACKUP   = $(shell date +`basename $$PWD`.tgz)

all: $(APP_BIN)

$(APP_BIN): $(OBJ_FILES)
	@mkdir -p $(BIN_PATH)
	$(CC) $(OBJ_FILES) $(LDFLAGS) -o $(BIN_PATH)/$(APP_BIN)
	@echo "--------------------------------------------------------------"
	@echo "                 to execute type: ./$(BIN_PATH)/$(APP_BIN) &"
	@echo "--------------------------------------------------------------"

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) $(INC_PATH) -c $< -o $@ 
	@echo "done..."
	
clean:
	rm $(OBJ_FILES) $(BIN_PATH)/$(APP_BIN)
	@echo "**************************"
	@echo "CLEAN"
	@echo "**************************"

tar : clean 
	@echo "**************************"
	@echo "TAR"
	@echo "**************************"
	cd .. && tar cvfz $(BACKUP) $(DIRNAME)
