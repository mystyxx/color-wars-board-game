SOURCE_FILES = src/main.cpp src/game.cpp src/pieces.cpp src/renderer.cpp
HEADERS = src/game_variables.hpp src/pieces.hpp src/renderer.hpp
ARGS =
OUTPUT_FILE = colorwars
ZIP = zip
ARCHIVE_NAME = PAUL_ROUX_L2X_POO_COLORWARS.$(ZIP)
OBJ = $(SOURCE_FILES:.cpp=.o)
USEFUL_FILES = Makefile	README.md

all : $(OUTPUT_FILE) 

$(OUTPUT_FILE) : $(OBJ)
	g++ $(ARGS) $(OBJ) -o $(OUTPUT_FILE)

%.o : %.cpp
	g++ $(ARGS) -c $< -o $@

dist : $(SOURCE_FILES) $(HEADERS)
	make clean
	$(ZIP) -r $(ARCHIVE_NAME) src/ $(USEFUL_FILES)

clean : 
	rm -f $(OUTPUT_FILE) $(OBJ)

