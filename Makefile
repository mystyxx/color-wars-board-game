SOURCE_FILES = src/main.cpp src/game.cpp src/pieces.cpp src/renderer.cpp
HEADERS = src/game_variables.hpp src/pieces.hpp src/renderer.hpp
ARGS = 
DEBUG_ARGS = -fsanitize=address
OUTPUT_FILE = colorwars
DEBUG_FILE = colorwars_debug
ZIP = zip
ARCHIVE_NAME = PAUL_ROUX_L2X_POO_COLORWARS.$(ZIP)
OBJ = $(SOURCE_FILES:.cpp=.o)
DEBUG_OBJ = $(SOURCE_FILES:.cpp=.debug.o)
USEFUL_FILES = Makefile	README.md

all : $(OUTPUT_FILE) 

debug : $(DEBUG_FILE)

$(OUTPUT_FILE) : $(OBJ)
	g++ $(ARGS) $(OBJ) -o $(OUTPUT_FILE)

$(DEBUG_FILE) : $(DEBUG_OBJ)
	g++ $(ARGS) $(DEBUG_ARGS) $(DEBUG_OBJ) -o $(DEBUG_FILE)

%.o : %.cpp
	g++ $(ARGS) -c $< -o $@

%.debug.o : %.cpp
	g++ $(ARGS) $(DEBUG_ARGS) -c $< -o $@

dist : $(SOURCE_FILES) $(HEADERS)
	make clean
	$(ZIP) -r $(ARCHIVE_NAME) src/ $(USEFUL_FILES)

clean : 
	rm -f $(DEBUG_FILE) $(OUTPUT_FILE) $(OBJ) $(DEBUG_OBJ)

