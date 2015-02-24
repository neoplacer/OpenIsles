#include "config/ConfigMgr.h"
#include "game/Game.h"


bool Building::isInsideCatchmentArea(const ConfigMgr* configMgr, const MapCoords& mapCoords) const {
    const BuildingConfig* buildingConfig = configMgr->getBuildingConfig(structureType);
    const RectangleData<char>* catchmentArea = buildingConfig->getCatchmentArea();

    // Gebäude hat keinen Einzugsbereich?
    if (catchmentArea == nullptr) {
        return false;
    }

    // Koordinaten innerhalb von buildingConfig.catchmentArea.data ermitteln
    int x, y;
    if (view == "south") {
        x = (mapCoords.x() - this->mapCoords.x()) + ((catchmentArea->width - this->mapWidth) / 2);
        y = (mapCoords.y() - this->mapCoords.y()) + ((catchmentArea->height - this->mapHeight) / 2);
        return (catchmentArea->getData(x, y, '0') == '1'); // TODO String-'1' durch echte 1 ersetzen, wenn wir das über die Config einlesen

    } else if (view == "east" ) {
        x = (mapCoords.x() - this->mapCoords.x()) + ((catchmentArea->height - this->mapWidth) / 2);
        y = (mapCoords.y() - this->mapCoords.y()) + ((catchmentArea->width - this->mapHeight) / 2);
        return (catchmentArea->getData(y, (catchmentArea->height - 1) - x, '0') == '1'); // TODO String-'1' durch echte 1 ersetzen, wenn wir das über die Config einlesen

    } else if (view == "north") {
        x = (mapCoords.x() - this->mapCoords.x()) + ((catchmentArea->width - this->mapWidth) / 2);
        y = (mapCoords.y() - this->mapCoords.y()) + ((catchmentArea->height - this->mapHeight) / 2);
        return (catchmentArea->getData((catchmentArea->width - 1) - x, (catchmentArea->height - 1) - y, '0') == '1'); // TODO String-'1' durch echte 1 ersetzen, wenn wir das über die Config einlesen

    } else if (view == "west") {
        x = (mapCoords.x() - this->mapCoords.x()) + ((catchmentArea->height - this->mapWidth) / 2);
        y = (mapCoords.y() - this->mapCoords.y()) + ((catchmentArea->width - this->mapHeight) / 2);
        return (catchmentArea->getData((catchmentArea->width - 1) - y, x, '0') == '1'); // TODO String-'1' durch echte 1 ersetzen, wenn wir das über die Config einlesen
    }
    else {
        assert(false);
        return false;
    }
}

bool Building::isInsideCatchmentArea(const ConfigMgr* configMgr, const MapObjectFixed& otherMapObject) const {
    const MapCoords& otherMapObjectCoords = otherMapObject.getMapCoords();
    int otherMapObjectWidth = otherMapObject.getMapWidth();
    int otherMapObjectHeight = otherMapObject.getMapHeight();

    for (int mapY = otherMapObjectCoords.y(); mapY < otherMapObjectCoords.y() + otherMapObjectHeight; mapY++) {
        for (int mapX = otherMapObjectCoords.x(); mapX < otherMapObjectCoords.x() + otherMapObjectWidth; mapX++) {
            if (isInsideCatchmentArea(configMgr, MapCoords(mapX, mapY))) {
                return true;
            }
        }
    }
    return false;
}