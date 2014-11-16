#include <algorithm>
#include "game/Game.h"
#include "graphics/GraphicsMgr.h"
#include "map/Building.h"
#include "map/MapCoordUtils.h"

// Aus main.cpp importiert
extern int mouseCurrentX, mouseCurrentY;
extern Game* game;
extern GraphicsMgr* graphicsMgr;


void MapCoordUtils::mapToScreenCoords(int mapX, int mapY, int& screenX, int& screenY) {
	screenX = (mapX - mapY) * GraphicsMgr::TILE_WIDTH_HALF;
	screenY = (mapX + mapY) * GraphicsMgr::TILE_HEIGHT_HALF;
}

void MapCoordUtils::mapToScreenCoords(double mapX, double mapY, int& screenX, int& screenY) {
    screenX = (int) ((mapX - mapY) * GraphicsMgr::TILE_WIDTH_HALF);
    screenY = (int) ((mapX + mapY) * GraphicsMgr::TILE_HEIGHT_HALF);
}

void MapCoordUtils::mapToScreenCoordsCenter(int mapX, int mapY, int& screenX, int& screenY) {
	screenX = (mapX - mapY) * GraphicsMgr::TILE_WIDTH_HALF + GraphicsMgr::TILE_WIDTH_HALF;
	screenY = (mapX + mapY) * GraphicsMgr::TILE_HEIGHT_HALF + GraphicsMgr::TILE_HEIGHT_HALF;
}

void MapCoordUtils::screenToMapCoords(int screenX, int screenY, int& mapX, int& mapY) {
	/*
	 * Screen-Koordinaten erst in ein (TILE_WIDTH x TILE_HEIGHT)-Koordinatensystem runterrechnen
	 * Dann gibts 8 Fälle und wir haben es. Ohne unperformante Matrizen und Projektionen :-)
	 *
	 * |          TILE_WIDTH           |
	 * |   xDiff<0.5       xDiff>0.5   |
	 * O-------------------------------O-------------
	 * | x-1          /|\          y-1 |
	 * |   [1]   /---- | ----\   [6]   | yDiff<0.5
	 * |    /----  [2] | [5]  ----\    |
	 * |---------------+---------------|  TILE_HEIGHT
	 * |    \----  [4] | [7]  ----/    |
	 * |   [3]   \---- | ----/   [8]   | yDiff>0.5
	 * | y+1          \|/          x+1 |
	 * O-------------------------------O-------------
	 *
	 * O = obere linke Ecke der Kacheln = Punkt, der mit mapToScreenCoords() berechnet wird.
	 *     Entspricht genau den (TILE_WIDTH x TILE_HEIGHT)-Koordinatensystem-Punkten
	 * [x] = 8 Fälle, siehe if-Block unten
	 *
	 * x/yDouble = exakte Koordinate im (TILE_WIDTH x TILE_HEIGHT)-Koordinatensystem.
	 * x/yInt = abgerundete Koordinate im (TILE_WIDTH x TILE_HEIGHT)-Koordinatensystem, entspricht den O-Punkten im Bild
	 * x/yDiff = Wert zwischen 0.0 und 1.0, siehe Grafik oben, mit dem die 8 Fälle identifiziert werden
	 *
	 */

	double xDouble = (double) screenX / (double) GraphicsMgr::TILE_WIDTH;
	double yDouble = (double) screenY / (double) GraphicsMgr::TILE_HEIGHT;
	int xInt = (int) floor(xDouble);
	int yInt = (int) floor(yDouble);
	double xDiff = xDouble - (double) xInt;
	double yDiff = yDouble - (double) yInt;

	// preliminaryMap-Koordinate = Map-Koordinate der Kachel, die ggf. noch in 4 von 8 Fällen angepasst werden muss
	int preliminaryMapX = xInt + yInt;
	int preliminaryMapY = yInt - xInt;

	// Jetzt die 8 Fälle durchgehen und ggf. noch plus-minus 1 auf x oder y
	if (xDiff < 0.5) {
		if (yDiff < 0.5) {
			if (xDiff < 0.5 - yDiff) {
				// Fall 1
				mapX = preliminaryMapX - 1;
				mapY = preliminaryMapY;
			} else {
				// Fall 2
				mapX = preliminaryMapX;
				mapY = preliminaryMapY;
			}
		} else {
			if (xDiff < yDiff - 0.5) {
				// Fall 3
				mapX = preliminaryMapX;
				mapY = preliminaryMapY + 1;
			} else {
				// Fall 4
				mapX = preliminaryMapX;
				mapY = preliminaryMapY;
			}
		}
	} else {
		if (yDiff < 0.5) {
			if (xDiff - 0.5 < yDiff) {
				// Fall 5
				mapX = preliminaryMapX;
				mapY = preliminaryMapY;
			} else {
				// Fall 6
				mapX = preliminaryMapX;
				mapY = preliminaryMapY - 1;
			}
		} else {
			if (xDiff - 0.5 < 1 - yDiff) {
				// Fall 7
				mapX = preliminaryMapX;
				mapY = preliminaryMapY;
			} else {
				// Fall 8
				mapX = preliminaryMapX + 1;
				mapY = preliminaryMapY;
			}
		}
	}
}

void MapCoordUtils::screenToDrawCoords(
    int screenX, int screenY, int elevation, MapObjectGraphic* graphic, SDL_Rect* rect) {

    rect->x = screenX - (
        graphic->getWidth() - (graphic->getMapWidth() + 1) * GraphicsMgr::TILE_WIDTH_HALF);
    rect->y = screenY - (
        graphic->getHeight() - (graphic->getMapWidth() + graphic->getMapHeight()) * GraphicsMgr::TILE_HEIGHT_HALF);

    // Elevation
    rect->y -= elevation * GraphicsMgr::ELEVATION_HEIGHT;

    // Scrolling-Offset anwenden
    Map* map = game->getMap();
    rect->x -= map->getScreenOffsetX();
    rect->y -= map->getScreenOffsetY();

    int screenZoom = map->getScreenZoom();
    rect->x /= screenZoom;
    rect->y /= screenZoom;

    // Größe ist gleich der Grafikgröße
    rect->w = graphic->getWidth() / screenZoom;
    rect->h = graphic->getHeight() / screenZoom;
}

void MapCoordUtils::mapToDrawCoords(int mapX, int mapY, int elevation, MapObjectGraphic* graphic, SDL_Rect* rect) {
    int screenX, screenY;
    mapToScreenCoords(mapX, mapY, screenX, screenY);

    screenToDrawCoords(screenX, screenY, elevation, graphic, rect);
}

void MapCoordUtils::mapToDrawCoords(double mapX, double mapY, int elevation, MapObjectGraphic* graphic, SDL_Rect* rect) {
    int screenX, screenY;
    mapToScreenCoords(mapX, mapY, screenX, screenY);

    screenToDrawCoords(screenX, screenY, elevation, graphic, rect);
}

void MapCoordUtils::getDrawCoordsForBuilding(Building* building, SDL_Rect* rect) {
    int mapX, mapY;
    building->getMapCoords(mapX, mapY);

    MapObjectGraphic* graphic = graphicsMgr->getGraphicForStructure(building->getStructureType());
    const int elevation = 1; // TODO für Gebäude wie Anlegestelle, Fischerhütte etc. muss auf 0 gesetzt werden

    mapToDrawCoords(mapX, mapY, elevation, graphic, rect);
}

void MapCoordUtils::getMapCoordsUnderMouse(int& mouseCurrentMapX, int& mouseCurrentMapY) {
    Map* map = game->getMap();
    int screenZoom = map->getScreenZoom();

    int mouseScreenX = (mouseCurrentX * screenZoom) + map->getScreenOffsetX();
    int mouseScreenY = (mouseCurrentY * screenZoom) + map->getScreenOffsetY();

    // Wir machen es wie Anno 1602: Für die Position, wo der Mauszeiger steht, wird immer die elevatete Position
    // genommen, selbst, wenn wir uns auf dem Wasser befinden.
    const int elevation = 1;
    MapCoordUtils::screenToMapCoords(
        mouseScreenX, mouseScreenY + elevation * GraphicsMgr::ELEVATION_HEIGHT / screenZoom,
        mouseCurrentMapX, mouseCurrentMapY);
}