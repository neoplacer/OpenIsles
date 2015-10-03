#include <algorithm>
#include <cassert>
#include <cmath>
#include <string>
#include "map/Building.h"
#include "map/Map.h"
#include "map/MapCoordUtils.h"
#include "map/Street.h"


ScreenCoords MapCoordUtils::mapToScreenCoords(
    const MapCoords& mapCoords, const FourthDirection& screenView, const Map& map) {

    return mapToScreenCoords((const DoubleMapCoords&) mapCoords, screenView, map);
}

ScreenCoords MapCoordUtils::mapToScreenCoords(
    const DoubleMapCoords& mapCoords, const FourthDirection& screenView, const Map& map) {

    const MapCoords& mapCoordsCentered = map.getMapCoordsCentered();
    double mapXDiff, mapYDiff;

    // Entsprechend der Ansicht die Entfernung zur Center-Kachel bestimmen
    if (screenView == Direction::SOUTH) {
        mapXDiff = mapCoords.x() - mapCoordsCentered.x();
        mapYDiff = mapCoords.y() - mapCoordsCentered.y();
    } else if (screenView == Direction::EAST) {
        mapXDiff = -(mapCoords.y() - mapCoordsCentered.y());
        mapYDiff = mapCoords.x() - mapCoordsCentered.x();
    } else if (screenView == Direction::NORTH) {
        mapXDiff = -(mapCoords.x() - mapCoordsCentered.x());
        mapYDiff = -(mapCoords.y() - mapCoordsCentered.y());
    } else if (screenView == Direction::WEST) {
        mapXDiff = mapCoords.y() - mapCoordsCentered.y();
        mapYDiff = -(mapCoords.x() - mapCoordsCentered.x());
    }
    else {
        assert(false);
        mapXDiff = mapYDiff = 0;
    }

    double screenX = (mapXDiff - mapYDiff) * (double) IGraphicsMgr::TILE_WIDTH_HALF;
    double screenY = (mapXDiff + mapYDiff) * (double) IGraphicsMgr::TILE_HEIGHT_HALF;

    return ScreenCoords((int) screenX, (int) screenY);
}

MapCoords MapCoordUtils::screenToMapCoords(
    const ScreenCoords& screenCoords, const FourthDirection& screenView, const Map& map) {

    int mapXDiffInSouthView = (int) std::floor((screenCoords.x() / (double) IGraphicsMgr::TILE_WIDTH) +
                                               (screenCoords.y() / (double) IGraphicsMgr::TILE_HEIGHT));
    int mapYDiffInSouthView = (int) std::floor((-screenCoords.x() / (double) IGraphicsMgr::TILE_WIDTH) +
                                               (screenCoords.y() / (double) IGraphicsMgr::TILE_HEIGHT));

    // Entsprechend der Ansicht umrechnen (= Invers-Operation zu der in mapToScreenCoords())
    int mapXDiff, mapYDiff;
    if (screenView == Direction::SOUTH) {
        mapXDiff = mapXDiffInSouthView;
        mapYDiff = mapYDiffInSouthView;
    } else if (screenView == Direction::EAST) {
        mapXDiff = mapYDiffInSouthView;
        mapYDiff = -mapXDiffInSouthView;
    } else if (screenView == Direction::NORTH) {
        mapXDiff = -mapXDiffInSouthView;
        mapYDiff = -mapYDiffInSouthView;
    } else if (screenView == Direction::WEST) {
        mapXDiff = -mapYDiffInSouthView;
        mapYDiff = mapXDiffInSouthView;
    }
    else {
        assert(false);
        mapXDiff = mapYDiff = 0;
    }

    const MapCoords& mapCoordsCentered = map.getMapCoordsCentered();
    return MapCoords(mapCoordsCentered.x() + mapXDiff, mapCoordsCentered.y() + mapYDiff);
}

Rect MapCoordUtils::mapToDrawCoords(
    const DoubleMapCoords& mapCoords, const Map& map, int elevation, const IGraphic& graphic,
    unsigned char mapWidth, unsigned char mapHeight) {

    ScreenCoords screenCoords = mapToScreenCoords(mapCoords, map.getScreenView(), map);

    return screenToDrawCoords(screenCoords, map, elevation, graphic, mapWidth, mapHeight);
}

Rect MapCoordUtils::mapToDrawCoords(
    const MapCoords& mapCoords, const Map& map, int elevation, const IGraphic& graphic,
    unsigned char mapWidth, unsigned char mapHeight) {

    ScreenCoords screenCoords = mapToScreenCoords(mapCoords, map.getScreenView(), map);

    return screenToDrawCoords(screenCoords, map, elevation, graphic, mapWidth, mapHeight);
}

Rect MapCoordUtils::getDrawCoordsForMapObjectFixed(
    const Map& map, IGraphicsMgr* graphicsMgr, const MapObjectFixed* mapObjectFixed) {

    const MapCoords& mapCoords = mapObjectFixed->getMapCoords();

    const FourthDirection& structureView = mapObjectFixed->getView();
    const FourthDirection& viewToRender = Direction::addDirections(structureView, map.getScreenView());

    const MapObjectType* mapObjectType = mapObjectFixed->getMapObjectType();
    const std::string graphicSetName = graphicsMgr->getGraphicSetNameForMapObject(mapObjectType);
    const GraphicSet* graphicSet = graphicsMgr->getGraphicSet(graphicSetName);

    // TODO duplicate code
    const IGraphic* graphic;
    if (mapObjectType->type == MapObjectTypeClass::HARVESTABLE) {
        // Harvestable? ausgewachsenen Zustand nehmen
        unsigned char maxAge = mapObjectType->maxAge;
        const std::string fullgrownState = "growth" + toString(maxAge);
        graphic = graphicSet->getByStateAndView(fullgrownState, viewToRender)->getGraphic();
    }
    else if (mapObjectType->type == MapObjectTypeClass::STREET) {
        const Street* street = dynamic_cast<const Street*>(mapObjectFixed);
        const std::string state = street->getStateToRender();
        graphic = graphicSet->getByStateAndView(state, viewToRender)->getGraphic();
    }
    else {
        graphic = graphicSet->getByView(viewToRender)->getGraphic();
    }

    const int elevation = 1; // TODO für Gebäude wie Anlegestelle, Fischerhütte etc. muss auf 0 gesetzt werden

    return mapToDrawCoords(
        mapCoords, map, elevation, *graphic, mapObjectFixed->getMapWidth(), mapObjectFixed->getMapHeight());
}

ScreenCoords MapCoordUtils::getScreenCoordsUnderMouse(const Map& map, int mouseCurrentX, int mouseCurrentY) {
    int screenZoom = map.getScreenZoom();
    int screenX = (mouseCurrentX - Consts::mapClipRect.w / 2) * screenZoom;
    int screenY = (mouseCurrentY - Consts::mapClipRect.h / 2) * screenZoom;

    return ScreenCoords(screenX, screenY);
}

MapCoords MapCoordUtils::getMapCoordsUnderMouse(const Map& map, int mouseCurrentX, int mouseCurrentY) {
    ScreenCoords mouseScreenCoords = getScreenCoordsUnderMouse(map, mouseCurrentX, mouseCurrentY);

    // Wir machen es wie Anno 1602: Für die Position, wo der Mauszeiger steht, wird immer die elevatete Position
    // genommen, selbst, wenn wir uns auf dem Wasser befinden.
    const int elevation = 1;

    int screenZoom = map.getScreenZoom();
    mouseScreenCoords.addY(elevation * IGraphicsMgr::ELEVATION_HEIGHT / screenZoom);

    return screenToMapCoords(mouseScreenCoords, map.getScreenView(), map);
}

void MapCoordUtils::getMapCoordsInScreenEdges(const Map& map,
    MapCoords& mapCoordsTopLeft, MapCoords& mapCoordsTopRight,
    MapCoords& mapCoordsBottomLeft, MapCoords& mapCoordsBottomRight) {

    const MapCoords& mapCoordsCentered = map.getMapCoordsCentered();
    int screenHalfWidth = (Consts::mapClipRect.h / 2) * map.getScreenZoom();
    int screenHalfHeight = (Consts::mapClipRect.w / 2) * map.getScreenZoom();


    int mapXDistance = (int) ((screenHalfWidth / (double) IGraphicsMgr::TILE_WIDTH) +
                              (screenHalfHeight / (double) IGraphicsMgr::TILE_HEIGHT));
    int mapYDistance = (int) ((-screenHalfWidth / (double) IGraphicsMgr::TILE_WIDTH) +
                              (screenHalfHeight / (double) IGraphicsMgr::TILE_HEIGHT));

    // TODO ggf. was draufrechnen, weil wir abrunden?

    // Ecken-Koordinaten bestimmen
    MapCoords mcTopLeft = MapCoords(mapCoordsCentered.x() - mapXDistance, mapCoordsCentered.y() - mapYDistance);
    MapCoords mcTopRight = MapCoords(mapCoordsCentered.x() + mapYDistance, mapCoordsCentered.y() - mapXDistance);
    MapCoords mcBottomLeft = MapCoords(mapCoordsCentered.x() - mapYDistance, mapCoordsCentered.y() + mapXDistance);
    MapCoords mcBottomRight = MapCoords(mapCoordsCentered.x() + mapXDistance, mapCoordsCentered.y() + mapYDistance);

    // Und je Ansicht der richtigen Ecke zuordnen
    const FourthDirection& screenView = map.getScreenView();
    if (screenView == Direction::SOUTH) {
        mapCoordsTopLeft = mcTopLeft;
        mapCoordsTopRight = mcTopRight;
        mapCoordsBottomLeft = mcBottomLeft;
        mapCoordsBottomRight = mcBottomRight;
    } else if (screenView == Direction::EAST) {
        mapCoordsTopLeft = mcBottomLeft;
        mapCoordsTopRight = mcTopLeft;
        mapCoordsBottomLeft = mcBottomRight;
        mapCoordsBottomRight = mcTopRight;
    } else if (screenView == Direction::NORTH) {
        mapCoordsTopLeft = mcBottomRight;
        mapCoordsTopRight = mcBottomLeft;
        mapCoordsBottomLeft = mcTopRight;
        mapCoordsBottomRight = mcTopLeft;
    } else if (screenView == Direction::WEST) {
        mapCoordsTopLeft = mcTopRight;
        mapCoordsTopRight = mcBottomRight;
        mapCoordsBottomLeft = mcTopLeft;
        mapCoordsBottomRight = mcBottomLeft;
    }
    else {
        assert(false);
    }
}

Rect MapCoordUtils::screenToDrawCoords(
    const ScreenCoords& screenCoords, const Map& map, int elevation, const IGraphic& graphic,
    unsigned char mapWidth, unsigned char mapHeight) {

    Rect drawCoordsRect;

    drawCoordsRect.x = screenCoords.x();
    drawCoordsRect.y = screenCoords.y();

    // Grafik entsprechend ihrer Größe in Map-Koordinaten ausrichten.
    const FourthDirection& screenView = map.getScreenView();
    if (screenView == Direction::SOUTH) {
        drawCoordsRect.x -= graphic.getWidth() - mapWidth * IGraphicsMgr::TILE_WIDTH_HALF;
        drawCoordsRect.y -= graphic.getHeight() - (mapWidth + mapHeight) * IGraphicsMgr::TILE_HEIGHT_HALF;
    } else if (screenView == Direction::EAST) {
        drawCoordsRect.x -= graphic.getWidth() - IGraphicsMgr::TILE_WIDTH_HALF;
        drawCoordsRect.y -= graphic.getHeight() - (mapWidth + 1) * IGraphicsMgr::TILE_HEIGHT_HALF;
    } else if (screenView == Direction::NORTH) {
        drawCoordsRect.x -= graphic.getWidth() - mapHeight * IGraphicsMgr::TILE_WIDTH_HALF;
        drawCoordsRect.y -= graphic.getHeight() - IGraphicsMgr::TILE_HEIGHT;
    } else if (screenView == Direction::WEST) {
        drawCoordsRect.x -= IGraphicsMgr::TILE_WIDTH_HALF;
        drawCoordsRect.y -= graphic.getHeight() - (mapHeight + 1) * IGraphicsMgr::TILE_HEIGHT_HALF;
    } else {
        assert(false);
    }

    // Elevation
    drawCoordsRect.y -= elevation * IGraphicsMgr::ELEVATION_HEIGHT;

    // Zoom anwenden
    int screenZoom = map.getScreenZoom();
    drawCoordsRect.x /= screenZoom;
    drawCoordsRect.y /= screenZoom;

    // Zum Schluss in die Bildschirmmitte bringen
    drawCoordsRect.x += Consts::mapClipRect.w / 2;
    drawCoordsRect.y += Consts::mapClipRect.h / 2;

    // Größe ist gleich der Grafikgröße
    drawCoordsRect.w = graphic.getWidth() / screenZoom;
    drawCoordsRect.h = graphic.getHeight() / screenZoom;

    return drawCoordsRect;
}