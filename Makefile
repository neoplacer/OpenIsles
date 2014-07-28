SRC_DIRECTORY := src
DATA_DIRECTORY := data
BUILD_DIRECTORY := build
OBJ_FILES := \
	$(BUILD_DIRECTORY)/gui/Gui.o \
	$(BUILD_DIRECTORY)/map/Building.o \
	$(BUILD_DIRECTORY)/map/Map.o \
	$(BUILD_DIRECTORY)/Graphic.o \
	$(BUILD_DIRECTORY)/GraphicsMgr.o \
	$(BUILD_DIRECTORY)/main.o
	
MAP_HEADER_FILES := $(addprefix $(SRC_DIRECTORY)/map/,Building.h Map.h MapObject.h Structure.h)

# Für das fertige Tileset für Tiled: Anzahl Kacheln pro Zeile
TILESET_WIDTH := 4

SDL_CFLAGS := `sdl2-config --cflags`
SDL_LDFLAGS := `sdl2-config --libs`

CC := /usr/bin/g++
CFLAGS := -std=c++11 -Wall -Werror -pedantic -g -O0 -I$(SRC_DIRECTORY) $(SDL_CFLAGS)
LDFLAGS := $(SDL_LDFLAGS) -lSDL2_image -lSDL2_ttf


.PHONY: all clean build-tiles clean-tiles build-gui clean-gui

all: build-tiles build-gui $(BUILD_DIRECTORY)/OpenIsles

clean: clean-tiles clean-gui
	rm -rf $(BUILD_DIRECTORY)
	

$(BUILD_DIRECTORY):
	mkdir $@


$(BUILD_DIRECTORY)/gui: | $(BUILD_DIRECTORY)
	mkdir $@
	
$(BUILD_DIRECTORY)/gui/Gui.o: $(SRC_DIRECTORY)/gui/Gui.cpp $(SRC_DIRECTORY)/gui/Gui.h $(SRC_DIRECTORY)/Graphic.h | $(BUILD_DIRECTORY)/gui
	$(CC) $(CFLAGS) -o $@ -c $<
	
	
$(BUILD_DIRECTORY)/map: | $(BUILD_DIRECTORY)
	mkdir $@
	
$(BUILD_DIRECTORY)/map/Building.o: $(SRC_DIRECTORY)/map/Building.cpp $(MAP_HEADER_FILES) | $(BUILD_DIRECTORY)/map
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(BUILD_DIRECTORY)/map/Map.o: $(SRC_DIRECTORY)/map/Map.cpp $(MAP_HEADER_FILES) $(SRC_DIRECTORY)/GraphicsMgr.h | $(BUILD_DIRECTORY)/map
	$(CC) $(CFLAGS) -o $@ -c $<
	
	
$(BUILD_DIRECTORY)/Graphic.o: $(SRC_DIRECTORY)/Graphic.cpp $(SRC_DIRECTORY)/Graphic.h | $(BUILD_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(BUILD_DIRECTORY)/GraphicsMgr.o: $(SRC_DIRECTORY)/GraphicsMgr.cpp $(SRC_DIRECTORY)/GraphicsMgr.h | $(BUILD_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(BUILD_DIRECTORY)/main.o: $(SRC_DIRECTORY)/main.cpp $(SRC_DIRECTORY)/gui/Gui.h $(SRC_DIRECTORY)/map/Map.h $(SRC_DIRECTORY)/GraphicsMgr.h | $(BUILD_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<
	

$(BUILD_DIRECTORY)/OpenIsles: $(OBJ_FILES) | $(BUILD_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)


build-tiles:
	awk 'NF > 0 && /^[^#]/ { print "convert $(DATA_DIRECTORY)/img/tiles/tileset-src.xcf -crop 64x32+" $$2 "+" $$3 " $(DATA_DIRECTORY)/img/tiles/alpha-mask.png -alpha Off -compose CopyOpacity -composite $(DATA_DIRECTORY)/img/tiles/" $$4 }' $(DATA_DIRECTORY)/img/tiles/tiles.txt | bash
	convert -size $$(($(TILESET_WIDTH)*64))x256 canvas:transparent $(DATA_DIRECTORY)/img/tiles/tileset.png
	awk 'NF > 0 && /^[^#]/ { print "convert $(DATA_DIRECTORY)/img/tiles/tileset.png $(DATA_DIRECTORY)/img/tiles/" $$4 " -geometry +" (($$1-1)%$(TILESET_WIDTH))*64 "+" int(($$1-1)/$(TILESET_WIDTH))*32 " -composite $(DATA_DIRECTORY)/img/tiles/tileset.png" }' $(DATA_DIRECTORY)/img/tiles/tiles.txt | bash
	
clean-tiles:
	awk 'NF > 0 && /^[^#]/ { print "rm -f $(DATA_DIRECTORY)/img/tiles/" $$4 }' $(DATA_DIRECTORY)/img/tiles/tiles.txt | bash
	rm -f $(DATA_DIRECTORY)/img/tiles/tileset.png
	
	
clean-gui:
	rm $(DATA_DIRECTORY)/img/gui/panel.png
	rm $(DATA_DIRECTORY)/img/gui/statusbar.png
	
build-gui: $(DATA_DIRECTORY)/img/gui/panel.png $(DATA_DIRECTORY)/img/gui/statusbar.png | $(DATA_DIRECTORY)/img/gui

$(DATA_DIRECTORY)/img/gui:
	mkdir $@
	
$(DATA_DIRECTORY)/img/gui/panel.png: | $(DATA_DIRECTORY)/img/gui
	convert -size 236x748 canvas:"#907f67" -mattecolor "#6f5038" -frame 10x10+5+5 $@
	
$(DATA_DIRECTORY)/img/gui/statusbar.png: | $(DATA_DIRECTORY)/img/gui
	convert -size 758x24 canvas:"#907f67" -mattecolor "#6f5038" -frame 5x5+2+2 $@