#include <cstring>
#include <iostream>
#include <stdexcept>
#include "config/ConfigMgr.h"
#include "config/Good.h"
#include "utils/StringFormat.h"


// TODO Fehlermanagement, wenn die XML-Dateien mal nicht so hübsch aussiehen, dass alle Tags da sind

ConfigMgr::ConfigMgr() {
    loadGoods();
    std::cout << "Loaded goods." << std::endl;

    loadMapObjectTypes();
    std::cout << "Loaded mapObjectTypes." << std::endl;

    loadTilesConfig();
    std::cout << "Loaded tiles." << std::endl;
}

ConfigMgr::~ConfigMgr() {
}

void ConfigMgr::loadGoods() {
    rapidxml::file<> xmlFile("data/config/goods.xml");

    rapidxml::xml_document<>* xmlDocument = new rapidxml::xml_document<>();
    xmlDocument->parse<0>(xmlFile.data());

    rapidxml::xml_node<>* goodsNode = xmlDocument->first_node("goods", 5, true);

    for (rapidxml::xml_node<>* goodNode = goodsNode->first_node("good", 4, true); goodNode != nullptr;
         goodNode = goodNode->next_sibling("good", 4, true)) {

        const char* name = goodNode->first_attribute("name", 4, true)->value();
        const char* label = goodNode->value();
        bool rawMaterial = xmlAttributeToBool(goodNode->first_attribute("raw-material", 12, true), false);
        bool invisible = xmlAttributeToBool(goodNode->first_attribute("invisible", 9, true), false);

        Good& good = goodsMap[name];
        good.name = name;
        good.label = label;
        good.rawMaterial = rawMaterial;
        good.invisible = invisible;

        goodsList.push_back(&good);
    }

    delete xmlDocument;
}

void ConfigMgr::loadMapObjectTypes() {
    rapidxml::file<> xmlFile("data/config/map-objects.xml");

    rapidxml::xml_document<>* xmlDocument = new rapidxml::xml_document<>();
    xmlDocument->parse<0>(xmlFile.data());

    rapidxml::xml_node<>* mapObjectsNode = xmlDocument->first_node("map-objects", 11, true);

    for (rapidxml::xml_node<>* node = mapObjectsNode->first_node(); node != nullptr; node = node->next_sibling()) {
        std::string name = std::string(node->first_attribute("name", 4, true)->value());
        MapObjectType& mapObjectType = mapObjectTypesMap[name];
        mapObjectType.name = name;

        // Knoten-Typ
        const char* nodeName = node->name();
        if (strcmp(nodeName, "harvestable") == 0) {
            mapObjectType.type = MapObjectTypeClass::HARVESTABLE;
        } else if (strcmp(nodeName, "structure") == 0) {
            mapObjectType.type = MapObjectTypeClass::STRUCTURE;
        } else if (strcmp(nodeName, "street") == 0) {
            mapObjectType.type = MapObjectTypeClass::STREET;
        } else if (strcmp(nodeName, "building") == 0) {
            mapObjectType.type = MapObjectTypeClass::BUILDING;
        } else {
            std::cerr << "Illegal node '" << nodeName << "'." << std::endl;
            throw std::runtime_error("Illegal node");
        }

        // Basics
        mapObjectType.mapWidth = (unsigned char) stringToUnsignedLong(
            node->first_attribute("width", 5, true)->value());
        mapObjectType.mapHeight = (unsigned char) stringToUnsignedLong(
            node->first_attribute("height", 6, true)->value());
        mapObjectType.title = std::string(node->first_node("title", 5, true)->value());

        // Structure-Placing
        const char* structurePlacing = node->first_node("structure-placing", 17, true)->value();
        if (strcmp(structurePlacing, "individually") == 0) {
            mapObjectType.structurePlacing = StructurePlacing::INDIVIDUALLY;
        } else if (strcmp(structurePlacing, "rectangle") == 0) {
            mapObjectType.structurePlacing = StructurePlacing::RECTANGLE;
        } else if (strcmp(structurePlacing, "path") == 0) {
            mapObjectType.structurePlacing = StructurePlacing::PATH;
        } else {
            std::cerr << "Illegal value '" << structurePlacing << "' for structurePlacing." << std::endl;
            throw std::runtime_error("Illegal value for structurePlacing");
        }

        // Baubar auf
        MapTileTypeInt placeableOnMapTileTypeMask = 0;
        rapidxml::xml_node<>* placeableOnNode = node->first_node("placeable-on", 12, true);
        for (rapidxml::xml_node<>* mapTileTypeNode = placeableOnNode->first_node("map-tile-type", 13, true);
             mapTileTypeNode != nullptr;
             mapTileTypeNode = mapTileTypeNode->next_sibling("map-tile-type", 13, true)) {

            const char* typeString = mapTileTypeNode->value();
            MapTileType mapTileType = getMapTileTypeByName(typeString);

            placeableOnMapTileTypeMask |= (MapTileTypeInt) mapTileType;
        }
        mapObjectType.placeableOnMapTileTypeMask = placeableOnMapTileTypeMask;

        // Wald?
        rapidxml::xml_node<>* forestNode = node->first_node("forest", 6, true);
        mapObjectType.isForest = (forestNode != nullptr);
        // TODO das Tag können wir später erweitern, für welchen Inseltyp der Wald ist (nördlich, südlich)

        // Baukosten
        rapidxml::xml_node<>* buildingCostsNode = node->first_node("building-costs", 14, true);
        BuildingCosts& buildingCosts = mapObjectType.buildingCosts;
        buildingCosts.coins = (unsigned int) stringToUnsignedLong(
            buildingCostsNode->first_node("coins", 5, true)->value());
        buildingCosts.tools = (unsigned int) stringToUnsignedLong(
            buildingCostsNode->first_node("tools", 5, true)->value());
        buildingCosts.wood = (unsigned int) stringToUnsignedLong(
            buildingCostsNode->first_node("wood", 4, true)->value());
        buildingCosts.bricks = (unsigned int) stringToUnsignedLong(
            buildingCostsNode->first_node("bricks", 6, true)->value());

        // optionale Tags
        rapidxml::xml_node<>* catchmentAreaNode = node->first_node("catchment-area", 14, true);
        if (catchmentAreaNode != nullptr) {
            const char* catchmentAreaValue = catchmentAreaNode->value();
            mapObjectType.catchmentArea.reset(parseCatchmentArea(catchmentAreaValue));
        }

        rapidxml::xml_node<>* productionSlotsNode = node->first_node("production-slots", 16, true);
        if (productionSlotsNode != nullptr) {
            rapidxml::xml_node<>* outputNode = productionSlotsNode->first_node("output", 6, true);
            readGoodSlotConfig(mapObjectType.buildingProduction.output, outputNode);

            rapidxml::xml_node<>* inputNode = productionSlotsNode->first_node("input", 5, true);
            if (inputNode != nullptr) {
                readGoodSlotConfig(mapObjectType.buildingProduction.input, inputNode);

                rapidxml::xml_node<>* input2Node = productionSlotsNode->first_node("input2", 6, true);
                if (input2Node != nullptr) {
                    readGoodSlotConfig(mapObjectType.buildingProduction.input2, input2Node);
                }
            }
        }

        rapidxml::xml_node<>* inhabitantsNode = node->first_node("inhabitants", 11, true);
        if (inhabitantsNode != nullptr) {
            mapObjectType.inhabitants = (unsigned char) stringToUnsignedLong(inhabitantsNode->value());
        }

        rapidxml::xml_node<>* maxCarriersNode = node->first_node("max-carriers", 12, true);
        if (maxCarriersNode != nullptr) {
            mapObjectType.maxCarriers = (unsigned char) stringToUnsignedLong(maxCarriersNode->value());
        }

        rapidxml::xml_node<>* maxAgeNode = node->first_node("max-age", 7, true);
        if (maxAgeNode != nullptr) {
            mapObjectType.maxAge = (unsigned char) stringToUnsignedLong(maxAgeNode->value());
        }

        std::cout << "Loaded mapObjectType '" << mapObjectType.name << "'." << std::endl;
    }

    delete xmlDocument;
}

void ConfigMgr::loadTilesConfig() {
    // Datei öffnen
    rapidxml::file<> xmlFile("data/config/tiles.xml");

    rapidxml::xml_document<>* xmlDocument = new rapidxml::xml_document<>();
    xmlDocument->parse<0>(xmlFile.data());

    rapidxml::xml_node<>* tilesConfigNode = xmlDocument->first_node("tiles-config", 12, true);
    rapidxml::xml_node<>* tilesNode = tilesConfigNode->first_node("tiles", 5, true);

    // Tiles auslesen
    for (rapidxml::xml_node<>* tileNode = tilesNode->first_node("tile", 4, true); tileNode != nullptr;
         tileNode = tileNode->next_sibling("tile", 4, true)) {

        const char* tileName = tileNode->first_attribute("name", 4, true)->value();

        const char* mapTileTypeName = tileNode->first_attribute("type", 4, true)->value();
        const MapTileType mapTileType = getMapTileTypeByName(mapTileTypeName);

        MapTileConfig& mapTileConfig = mapTileConfigs[tileName];
        mapTileConfig.tileName = std::string(tileName);
        mapTileConfig.mapTileType = mapTileType;

        // TODO konfigurierbar machen
        if (mapTileConfig.tileName == "grass") {
            mapTileConfig.goodToHarvest = getGood("grassland");
        }

        for (rapidxml::xml_node<>* tmxTileNode = tileNode->first_node("tmx-tile", 8, true); tmxTileNode != nullptr;
             tmxTileNode = tmxTileNode->next_sibling("tmx-tile", 8, true)) {

            int tmxTileIndex = stringToInteger(tmxTileNode->first_attribute("tmx-tile-index", 14, true)->value());
            const char* tileViewName = tmxTileNode->first_attribute("view", 4, true)->value();
            int xOffsetInTileset = stringToInteger(tmxTileNode->first_attribute("x", 1, true)->value());
            int yOffsetInTileset = stringToInteger(tmxTileNode->first_attribute("y", 1, true)->value());

            FourthDirection tileView = Direction::fromString(tileViewName);
            mapTileConfig.mapTileViewsOffsetXYInTileset[tileView] =
                std::make_pair(xOffsetInTileset, yOffsetInTileset);

            // Zuordnung zu Tiled-Kachel-ID und View-Offset merken
            tiledIdToMapTileConfig[tmxTileIndex] = &mapTileConfig;
            tiledIdToViewOffset[tmxTileIndex] = tileView / 2;
        }

        // Den Ozean separat merken
        // TODO Was, wenn mehrere Ozean-Kacheln in der tiles.xml stehen?
        if (mapTileType == MapTileType::OCEAN) {
            mapTileConfigOcean = &mapTileConfig;
        }
    }

    delete xmlDocument;
}

MapTileType ConfigMgr::getMapTileTypeByName(const std::string& mapTileTypeName) {
    if (mapTileTypeName == "ocean") {
        return MapTileType::OCEAN;
    }
    else if (mapTileTypeName == "fish-grounds") {
        return MapTileType::FISH_GROUNDS;
    }
    else if (mapTileTypeName == "shallow-water") {
        return MapTileType::SHALLOW_WATER;
    }
    else if (mapTileTypeName == "shore-ocean") {
        return MapTileType::SHORE_OCEAN;
    }
    else if (mapTileTypeName == "shore-grass") {
        return MapTileType::SHORE_GRASS;
    }
    else if (mapTileTypeName == "river") {
        return MapTileType::RIVER;
    }
    else if (mapTileTypeName == "mountain") {
        return MapTileType::MOUNTAIN;
    }
    else if (mapTileTypeName == "grass") {
        return MapTileType::GRASS;
    }
    else {
        std::cerr << "Illegal mapTileTypeName '" << mapTileTypeName << "'." << std::endl;
        throw std::runtime_error("Illegal mapTileTypeName");
    }
}

bool ConfigMgr::xmlAttributeToBool(rapidxml::xml_attribute<>* attribute, bool defaultValue) {
    // Attribut nicht da? Default-Value verwenden
    if (attribute == nullptr) {
        return defaultValue;
    }

    const char* value = attribute->value();
    if (strcmp(value, "false") == 0) {
        return false;
    }
    else if (strcmp(value, "true") == 0) {
        return true;
    }
    else {
        std::cerr << "Illegal bool value '" << value << "'." << std::endl;
        throw std::runtime_error("Illegal bool value");
    }
}

RectangleData<char>* ConfigMgr::parseCatchmentArea(const char* catchmentAreaValue) {
    // Step 1: Durchgehen und width/height des Einzugsbereichs prüfen und ob alles zusammenpasst ///////////////////////
    bool notReachedFirstLine = true;
    int x = 0, y = 0; // 0-based Koordinaten, die wir grade vor uns haben
    int catchmentAreaWidth = 0;
    for (const char* ptr = catchmentAreaValue; *ptr != '\0'; ptr++) {
        if (notReachedFirstLine) {
            if (*ptr == '\n') {
                notReachedFirstLine = false;
            } else if (*ptr == ' ') {
                // Zeichen ok, ignorieren
            } else {
                throw std::runtime_error("Could not parse catchmentArea: Illegal char before first line");
            }
            continue;
        }

        if (*ptr == ' ') {
            if (x == 0) {
                continue; // Leerzeichen am Zeilenanfang ok
            } else {
                throw std::runtime_error("Could not parse catchmentArea: Illegal space inside line");
            }
        }

        if (*ptr == '0' || *ptr == '1') {
            x++;
        }

        if (*ptr == '\n') {
            // Zeile zu Ende? Wenn erste Zeile, gibt diese die Breite des Einzugsbereichs vor.
            // Jede weitere Zeile muss dieselbe Breite haben.
            if (y == 0) {
                catchmentAreaWidth = x;
            } else {
                if (x != catchmentAreaWidth) {
                    throw std::runtime_error("Could not parse catchmentArea: widths are not equal");
                }
            }

            x = 0;
            y++;
        }
    }

    // Letzte Zeile muss leer sein.
    if (x != 0) {
        throw std::runtime_error("Could not parse catchmentArea: Last line was not completly empty.");
    }
    int catchmentAreaHeight = y;

    // Step 2: Nun nur durchgehen und die '1'/'0'-Zeichen aufnehmen
    RectangleData<char>* catchmentArea = new RectangleData<char>(catchmentAreaWidth, catchmentAreaHeight);

    int i = 0;
    for (const char* ptr = catchmentAreaValue; *ptr != '\0'; ptr++) {
        if (*ptr == '0' || *ptr == '1') {
            catchmentArea->data[i++] = *ptr;
        }
    }

    return catchmentArea;
}

void ConfigMgr::readGoodSlotConfig(GoodsSlot& goodSlot, rapidxml::xml_node<>* produtionSlotNode) {
    const char* goodName = produtionSlotNode->first_attribute("good", 4, true)->value();
    const Good* good = getGood(goodName);

    goodSlot.good = good;
    goodSlot.capacity = (unsigned int) stringToUnsignedLong(
        produtionSlotNode->first_attribute("capacity", 8, true)->value());
    goodSlot.rate = stringToDouble(produtionSlotNode->first_attribute("rate", 4, true)->value());
}
