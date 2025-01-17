#include <random>
#include <set>
#include "game/Game.h"
#include "map/buildqueue/BuildOperation.h"
#include "utils/RandomEngine.h"


void BuildOperation::requestBuild(const MapCoords& mapCoords, const MapObjectType* mapObjectType,
                                  const FourthDirection& view) {

    assert (mapObjectsToBuildMode == MapObjectToBuild::EMPTY ||
            mapObjectsToBuildMode == MapObjectToBuild::BUILD);

    // TODO Außerhalb der Map?
    // TODO Außerhalb der Insel?

    // Erst überprüfen, ob sich das neue Objekt mit einem bestehenden in der Queue überlappt
    unsigned char mapWidth, mapHeight;
    if (view == Direction::NORTH || view == Direction::SOUTH) {
        mapWidth = mapObjectType->mapWidth;
        mapHeight = mapObjectType->mapHeight;
    } else {
        mapWidth = mapObjectType->mapHeight;
        mapHeight = mapObjectType->mapWidth;
    }

    for (int y = 0, my = mapCoords.y(); y < mapHeight; y++, my++) {
        for (int x = 0, mx = mapCoords.x(); x < mapWidth; x++, mx++) {
            auto iter = result.find(MapCoords(mx, my));
            if (iter != result.cend()) {
                if (!iter->second->costsNothingBecauseOfChange) {
                    /*
                     * Überlappung gefunden? Dann abbrechen, wir ignorieren den Aufruf.
                     * Allerdings nur, wenn das ResultBit costsNothingBecauseOfChange==false hat,
                     * da wir nur abbrechen, wenn ein bewusster Bau vorliegt.
                     * Wenn ein Bauauftrag für Kachel A eine Ersetzung für Kachel B zur Folge hat,
                     * dann darf natürlich nachträglich ein Bauauftrag für Kachel B vergeben werden.
                     */
                    return;
                }
            }
        }
    }

    if (!testColony(mapCoords)) {
        return; // keine/falsche Kolonie: Bauen ist hier verboten
    }

    // Sonderfälle, wo zufällig gewählt wird: Haus (= zufälliges Pionier-Haus) und Wald
    // TODO über Config steuern
    if (mapObjectType->name == "pioneers-house1" || mapObjectType->name == "northern-forest1") {
        if (mapObjectType->name == "pioneers-house1") {
            std::uniform_int_distribution<int> randomPioneerHouse(1, 5);
            std::string mapObjectTypeName = "pioneers-house" + toString(randomPioneerHouse(*context.randomEngine));

            mapObjectType = context.configMgr->getMapObjectType(mapObjectTypeName);
        } else if (mapObjectType->name == "northern-forest1") {
            std::uniform_int_distribution<int> randomNorthernForest(1, 2);
            std::string mapObjectTypeName = "northern-forest" + toString(randomNorthernForest(*context.randomEngine));

            mapObjectType = context.configMgr->getMapObjectType(mapObjectTypeName);
        }
    }

    // Objekt in die Bauqueue aufnehmen und Result-Objekt aktualisieren
    mapObjectsToBuild.push_back({ mapCoords, mapObjectType, view });
    mapObjectsToBuildMode = MapObjectToBuild::BUILD;
    rebuildResult();
}

void BuildOperation::requestBuildWhenNothingInTheWay(const MapCoords& mapCoords, const MapObjectType* mapObjectType,
                                                     const FourthDirection& view) {

    assert (mapObjectsToBuildMode == MapObjectToBuild::EMPTY ||
            mapObjectsToBuildMode == MapObjectToBuild::BUILD);

    // Überprüfen. ob auf der Karte alles frei is,...
    if (!isBuildAllowed({ mapCoords, mapObjectType, view })) {
        return;
    }

    // ...dann requestBuild() übergeben
    requestBuild(mapCoords, mapObjectType, view);
}

void BuildOperation::requestDemolish(const MapObjectFixed& mapObjectFixed) {
    assert (mapObjectsToBuildMode == MapObjectToBuild::EMPTY ||
            mapObjectsToBuildMode == MapObjectToBuild::DEMOLISH);

    const MapCoords& mapCoords = mapObjectFixed.getMapCoords();

    if (!testColony(mapCoords)) {
        return; // keine/falsche Kolonie: Abreißen ist hier verboten
    }

    mapObjectsToBuild.push_back({ mapCoords });
    mapObjectsToBuildMode = MapObjectToBuild::DEMOLISH;
    rebuildResult();
}

bool BuildOperation::testColony(const MapCoords& mapCoords) {
    // Kolonie prüfen: Alle Objekte in der Bau-Queue müssen in derselben Kolonie sein!
    if (colony == nullptr) {
        // Erste Map-Objekt gibt die Kolonie vor
        colony = context.game->getColony(mapCoords);
        if (colony == nullptr) {
            return false; // Keine Kolonie, da darf man eh nicht bauen
        }

    } else {
        // Jedes weitere Map-Objekt muss in dieser Kolonie sein
        Colony* colonyThere = context.game->getColony(mapCoords);
        if (colonyThere != colony) {
            return false;
        }
    }

    return true;
}

bool BuildOperation::isBuildAllowed(const MapObjectToBuild& mapObjectToBuild) const {
    const MapObjectType* mapObjectType = mapObjectToBuild.mapObjectType;
    const Map* map = context.game->getMap();

    unsigned char mapWidth, mapHeight;
    if (mapObjectToBuild.view == Direction::NORTH || mapObjectToBuild.view == Direction::SOUTH) {
        mapWidth = mapObjectType->mapWidth;
        mapHeight = mapObjectType->mapHeight;
    } else {
        mapWidth = mapObjectType->mapHeight;
        mapHeight = mapObjectType->mapWidth;
    }

    // Gucken, ob alles frei is
    for (int y = 0, my = mapObjectToBuild.mapCoords.y(); y < mapHeight; y++, my++) {
        for (int x = 0, mx = mapObjectToBuild.mapCoords.x(); x < mapWidth; x++, mx++) {
            MapCoords mapCoords(mx, my);
            
            // Passt das Gelände?
            const MapTile* mapTile = map->getMapTileAt(mapCoords);
            const MapTileConfig* mapTileConfig = mapTile->getMapTileConfig();
            if (((MapTileTypeInt) mapTileConfig->mapTileType & mapObjectType->placeableOnMapTileTypeMask) == 0) {
                return false;
            }

            // Gebiet erschlossen und unseres?
            if (mapTile->player == nullptr || mapTile->player != &player) {
                return false;
            }

            MapObjectFixed* mapObjectFixedAlreadyThere = map->getMapObjectFixedAt(mapCoords);
            if (mapObjectFixedAlreadyThere != nullptr) {
                // Überbauen erlaubt?
                if (!(mayBuildOver(mapObjectFixedAlreadyThere->getMapObjectType(), mapObjectType))) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool BuildOperation::mayBuildOver(const MapObjectType* mapObjectTypeThere,
                                  const MapObjectType* mapObjectTypeToBuildOver) const {

    // TODO via Config steuerbar

    // Über Harvestables darf man immer bauen
    bool isHarvestableThere = (mapObjectTypeThere->type == MapObjectTypeClass::HARVESTABLE);
    if (isHarvestableThere) {
        return true;
    }

    // Ersetzung für Straßen erlauben
    bool isStreetThere = (mapObjectTypeThere->type == MapObjectTypeClass::STREET);
    bool isStreetToBuildOver = (mapObjectTypeToBuildOver->type == MapObjectTypeClass::STREET);
    if (isStreetThere && isStreetToBuildOver) {
        return true;
    }

    // Plätze dürfen über Straßen gebaut werden
    bool isSquareToBuildOver = (mapObjectTypeToBuildOver->name.substr(0, 6) == "square");
    if (isStreetThere && isSquareToBuildOver) {
        return true;
    }

    return false;
}

StreetConnections BuildOperation::calculateStreetConnections(
    const MapCoords& mapCoords, const ToDrawOrToReplaceWith& toDrawOrToReplaceWith) const {

    // Angrenzende Straßen (auf der Karte und im BuildOperationResult) checken
    const Map* map = context.game->getMap();
    auto isStreetThere = [&](const MapCoords& mapCoordsInLamdba) {
        if (map->isStreetAt(mapCoordsInLamdba)) {
            return true;
        }

        auto iter = result.find(mapCoordsInLamdba);
        if (iter != result.cend()) {
            std::shared_ptr<MapObjectFixed>& mapObjectToUse = (toDrawOrToReplaceWith == TO_DRAW) ?
                iter->second->mapObjectToDraw : iter->second->mapObjectToReplaceWith;
            return (std::dynamic_pointer_cast<Street>(mapObjectToUse) != nullptr);
        }

        return false;
    };

    bool isStreetNorth = isStreetThere(MapCoords(mapCoords.x(), mapCoords.y() - 1));
    bool isStreetEast = isStreetThere(MapCoords(mapCoords.x() + 1, mapCoords.y()));
    bool isStreetSouth = isStreetThere(MapCoords(mapCoords.x(), mapCoords.y() + 1));
    bool isStreetWest = isStreetThere(MapCoords(mapCoords.x() - 1, mapCoords.y()));

    StreetConnections streetConnections;
    streetConnections.set(StreetConnections::BIT_POS_NORTH, isStreetNorth);
    streetConnections.set(StreetConnections::BIT_POS_EAST, isStreetEast);
    streetConnections.set(StreetConnections::BIT_POS_SOUTH, isStreetSouth);
    streetConnections.set(StreetConnections::BIT_POS_WEST, isStreetWest);

    return streetConnections;
}

void BuildOperation::adeptExistingStreets(
    const Street& streetToAdeptAround, bool resourcesEnoughToBuildThis) {

    Map* map = context.game->getMap();

    auto adeptExistingStreet = [&](const MapCoords& mapCoords) {
        // Ist hier eine bestehende Straße?
        const Street* existingStreet = dynamic_cast<const Street*>(map->getMapObjectFixedAt(mapCoords));
        if (existingStreet == nullptr) {
            return;
        }

        /* Nicht ersetzen, wenn dort im BuildOperationResult bereits was neue gebaut wird
         *
         * Haben wir eine Ersetzung dort, dann nicht überspringen. Es kann sein, dass zwei Kacheln eine Ersetzung
         * triggern, die erste mit resourcesEnoughToBuildThis==false, die zweite mit resourcesEnoughToBuildThis==true.
         * Dann müssen wir natürlich reinlaufen, um die Ersetzung korrekt zu setzen.
         */
        auto iter = result.find(mapCoords);
        if (iter != result.cend() && !iter->second->costsNothingBecauseOfChange) {
            return;
        }

        // Prüfen, ob Ersetzung notwendig ist
        StreetConnections streetConnectionsShouldBeToDraw =
            calculateStreetConnections(existingStreet->getMapCoords(), TO_DRAW);
        StreetConnections streetConnectionsShouldBeToReplaceWith =
            calculateStreetConnections(existingStreet->getMapCoords(), TO_REPLACE_WITH);

        if ((streetConnectionsShouldBeToDraw == existingStreet->streetConnections) &&
            (streetConnectionsShouldBeToDraw == streetConnectionsShouldBeToReplaceWith) &&
            (existingStreet->getView() == Direction::SOUTH)) {

            return; // passt alles schon
        }

        // Ersetzung anlegen
        std::shared_ptr<BuildOperationResultBit> resultBit;
        if (iter == result.cend()) {
            resultBit.reset(new BuildOperationResultBit());
            result[mapCoords] = resultBit;
        } else {
            resultBit = iter->second;
        }

        resultBit->buildAllowed = true;
        resultBit->costsNothingBecauseOfChange = true;
        if (resourcesEnoughToBuildThis) {
            // wenn auch nur ein angrenzender Straßenneubau genug Baumaterial hat, hat es die Ersetzung natürlich auch
            // und wir setzen auf true.
            resultBit->resourcesEnoughToBuildThis = true;
        }

        auto createReplacement = [&](const StreetConnections& streetConnectionsShouldBe) -> Street* {
            assert((existingStreet->getMapWidth() == 1) && (existingStreet->getMapHeight() == 1));

            Street* streetReplacement = (Street*) MapObject::instantiate(existingStreet->getMapObjectType());
            streetReplacement->setMapCoords(mapCoords);
            streetReplacement->setView(Direction::SOUTH);
            streetReplacement->setMapWidth(existingStreet->getMapWidth());
            streetReplacement->setMapHeight(existingStreet->getMapHeight());
            streetReplacement->streetConnections = streetConnectionsShouldBe;

            return streetReplacement;
        };

        if (streetConnectionsShouldBeToDraw != existingStreet->streetConnections) {
            resultBit->mapObjectToDraw.reset(createReplacement(streetConnectionsShouldBeToDraw));

            if (resultBit->resourcesEnoughToBuildThis) {
                if (streetConnectionsShouldBeToReplaceWith != streetConnectionsShouldBeToDraw) {
                    resultBit->mapObjectToReplaceWith.reset(
                        createReplacement(streetConnectionsShouldBeToReplaceWith));
                } else {
                    resultBit->mapObjectToReplaceWith = resultBit->mapObjectToDraw;
                }
            }
        }
    };

    const MapCoords& mapCoords = streetToAdeptAround.getMapCoords();
    adeptExistingStreet(MapCoords(mapCoords.x(), mapCoords.y() - 1));
    adeptExistingStreet(MapCoords(mapCoords.x() + 1, mapCoords.y()));
    adeptExistingStreet(MapCoords(mapCoords.x(), mapCoords.y() + 1));
    adeptExistingStreet(MapCoords(mapCoords.x() - 1, mapCoords.y()));
}

void BuildOperation::rebuildResult() {
    result.clear();
    result.buildingCosts.reset();
    result.result = BuildOperationResult::OK;

    switch (mapObjectsToBuildMode) {
        case MapObjectToBuild::EMPTY:
            // nix tun
            return;

        case MapObjectToBuild::BUILD:
            rebuildResultBuild();
            return;

        case MapObjectToBuild::DEMOLISH:
            rebuildResultDemolish();
            return;
    }
    assert(false);
}

void BuildOperation::rebuildResultBuild() {
    BuildingCosts resourcesAvailable = {
        player.coins,
        (int) colony->getGoods(context.configMgr->getGood("tools")).inventory,
        (int) colony->getGoods(context.configMgr->getGood("wood")).inventory,
        (int) colony->getGoods(context.configMgr->getGood("bricks")).inventory
    };

    bool enoughResources = true;
    for (const MapObjectToBuild& mapObjectToBuild : mapObjectsToBuild) {
        const MapObjectType* mapObjectType = mapObjectToBuild.mapObjectType;

        assert((mapObjectType->type == MapObjectTypeClass::HARVESTABLE) ||
               (mapObjectType->type == MapObjectTypeClass::STRUCTURE) ||
               (mapObjectType->type == MapObjectTypeClass::STREET) ||
               (mapObjectType->type == MapObjectTypeClass::BUILDING));

        unsigned char mapWidth, mapHeight;
        if (mapObjectToBuild.view == Direction::NORTH || mapObjectToBuild.view == Direction::SOUTH) {
            mapWidth = mapObjectType->mapWidth;
            mapHeight = mapObjectType->mapHeight;
        } else {
            mapWidth = mapObjectType->mapHeight;
            mapHeight = mapObjectType->mapWidth;
        }

        std::shared_ptr<BuildOperationResultBit> resultBit(new BuildOperationResultBit());

        // Was im Weg?
        bool buildAllowed = isBuildAllowed(mapObjectToBuild);
        resultBit->buildAllowed = buildAllowed;

        if (buildAllowed) {
            MapObjectFixed* mapObjectToDraw = (MapObjectFixed*) MapObject::instantiate(mapObjectType);
            mapObjectToDraw->setMapCoords(mapObjectToBuild.mapCoords);
            mapObjectToDraw->setView(mapObjectToBuild.view);
            mapObjectToDraw->setMapWidth(mapWidth);
            mapObjectToDraw->setMapHeight(mapHeight);
            resultBit->mapObjectToDraw.reset(mapObjectToDraw);
        }
        else {
            result.result = BuildOperationResult::NOT_OK;
        }

        // Gratis-Überbauen: Wurde mit demselben Map-Objekt-Typ überbaut, so ist das nur eine "Änderung" und kostet nix
        bool costsNothingBecauseOfChange = false;

        MapObjectFixed* mapObjectFixedAlreadyThere =
            context.game->getMap()->getMapObjectFixedAt(mapObjectToBuild.mapCoords);
        if (mapObjectFixedAlreadyThere != nullptr) {
            if (mapObjectFixedAlreadyThere->getMapObjectType() == mapObjectType) {
                costsNothingBecauseOfChange = true;
            }
        }
        resultBit->costsNothingBecauseOfChange = costsNothingBecauseOfChange;

        // Resourcen überprüfen (einmal false geworden, sparen wir uns die Differenz-Bildung)
        if (enoughResources && !costsNothingBecauseOfChange) {
            resourcesAvailable -= mapObjectType->buildingCosts;
            enoughResources = resourcesAvailable.isNonNegative();
        }
        resultBit->resourcesEnoughToBuildThis = enoughResources;

        if (!resultBit->costsNothingBecauseOfChange) {
            result.buildingCosts += mapObjectType->buildingCosts;
        }

        if (resultBit->resourcesEnoughToBuildThis) {
            resultBit->mapObjectToReplaceWith = resultBit->mapObjectToDraw;
        } else {
            resultBit->mapObjectToReplaceWith = nullptr;
        }

        // ResultBit den Koordinaten zuweisen
        for (int y = 0, my = mapObjectToBuild.mapCoords.y(); y < mapHeight; y++, my++) {
            for (int x = 0, mx = mapObjectToBuild.mapCoords.x(); x < mapWidth; x++, mx++) {
                result[MapCoords(mx, my)] = resultBit;
            }
        }
    }

    // Jetzt für Straßen streetConnections setzen, nachdem alles fertig is.
    // Wichtig: Result abkopieren, der Iterator funktioniert nicht korrekt, wenn sich in der Schleife der
    // Containerinhalt, den er iteriert, ändert
    BuildOperationResult resultCopy = result;

    for (auto iter : resultCopy) {
        std::shared_ptr<BuildOperationResultBit>& resultBit = iter.second;
        std::shared_ptr<Street> streetPtr = std::dynamic_pointer_cast<Street>(resultBit->mapObjectToDraw);
        if (!streetPtr) {
            continue; // TODO beim Abreißen von Straßen dürfen wir nicht continue machen, sondern müssen Straßen anpassen
        }

        Street& streetToDraw = *streetPtr;

        // streetConnections ermitteln
        StreetConnections streetConnectionsToDraw =
            calculateStreetConnections(streetToDraw.getMapCoords(), TO_DRAW);
        StreetConnections streetConnectionsToReplaceWith =
            calculateStreetConnections(streetToDraw.getMapCoords(), TO_REPLACE_WITH);

        streetToDraw.streetConnections = streetConnectionsToDraw;

        // mindestens eine angrenzende Straße? Dann die View auf SOUTH stellen.
        if (streetConnectionsToDraw.any()) {
            streetToDraw.setView(Direction::SOUTH);
        }

        // Wenn für toDraw und toReplaceWith nicht gleich sind, müssen wir ein separates toReplaceWith-Objekt anlegen
        if (resultBit->mapObjectToReplaceWith &&
            (resultBit->mapObjectToDraw == resultBit->mapObjectToReplaceWith) &&
            (streetConnectionsToDraw != streetConnectionsToReplaceWith)) {

            MapObjectFixed* mapObjectToReplaceWith = (MapObjectFixed*) MapObject::instantiate(streetToDraw.getMapObjectType());
            mapObjectToReplaceWith->setMapCoords(streetToDraw.getMapCoords());
            mapObjectToReplaceWith->setView(streetToDraw.getView());
            mapObjectToReplaceWith->setMapWidth(streetToDraw.getMapWidth());
            mapObjectToReplaceWith->setMapHeight(streetToDraw.getMapHeight());

            Street* streetToReplaceWith = (Street*) mapObjectToReplaceWith;
            streetToReplaceWith->streetConnections = streetConnectionsToReplaceWith;

            // mindestens eine angrenzende Straße? Dann die View auf SOUTH stellen.
            if (streetConnectionsToReplaceWith.any()) {
                streetToReplaceWith->setView(Direction::SOUTH);
            }

            resultBit->mapObjectToReplaceWith.reset(mapObjectToReplaceWith);
        }

        // Zum Schluss Straßen außenrum ggf. ersetzen
        // (für Ersetzungen können wir sparen, diese triggern niemals eine zweite Ersetzung)
        if (!resultBit->costsNothingBecauseOfChange) {
            adeptExistingStreets(streetToDraw, resultBit->resourcesEnoughToBuildThis);
        }
    }
}

void BuildOperation::rebuildResultDemolish() {
    for (const MapObjectToBuild& mapObjectToBuild : mapObjectsToBuild) {
        const MapCoords& mapCoords = mapObjectToBuild.mapCoords;
        const MapObjectFixed* mapObjectFixed = context.game->getMap()->getMapObjectFixedAt(mapCoords);
        assert(mapObjectFixed != nullptr);

        std::shared_ptr<BuildOperationResultBit> resultBit(new BuildOperationResultBit());
        resultBit->buildAllowed = true;
        resultBit->deleteMapObjectThere = true;

        // ResultBit den Koordinaten zuweisen
        for (int y = 0, my = mapObjectToBuild.mapCoords.y(); y < mapObjectFixed->getMapHeight(); y++, my++) {
            for (int x = 0, mx = mapObjectToBuild.mapCoords.x(); x < mapObjectFixed->getMapWidth(); x++, mx++) {
                result[MapCoords(mx, my)] = resultBit;
            }
        }
    }

    // TODO Straßen umbiegen, wenn wir löschen
}

void BuildOperation::doBuild() {
    assert(result.result == BuildOperationResult::OK);

    if (result.empty()) {
        return;
    }

    Map* map = context.game->getMap();
    assert(colony != nullptr);

    // Merken, welche Objekte wir bereits bearbeiten haben. In der result-Map ist kachelweise alles drin.
    // Wir wollen für ein 2x2-Gebäude nicht 4x das Ding bauen, sondern nur einmal.
    std::set<MapCoords> mapCoordsDone;

    BuildingCosts buildingCostsEffective;

    for (auto iter : result) {
        const MapCoords& mapCoords = iter.first;
        const BuildOperationResultBit& resultBit = *iter.second;

        assert(resultBit.buildAllowed);

        // Kachel schon abgearbeitet?
        if (mapCoordsDone.count(mapCoords)) {
            continue;
        }

        // Reichen die Resourcen zum Bauen nicht? Dann einfach überspringen.
        if (mapObjectsToBuildMode == MapObjectToBuild::BUILD && !resultBit.resourcesEnoughToBuildThis) {
            continue;
        }

        // Bestehendes Objekt soll überbaut werden?
        if (resultBit.mapObjectToReplaceWith) {
            // Über alle Kacheln loopen. z.B. ein 2x2-Gebäude kann 4 1x1-Felder überschreiben
            const int mapHeight = resultBit.mapObjectToReplaceWith->getMapHeight();
            const int mapWidth = resultBit.mapObjectToReplaceWith->getMapWidth();
            const MapCoords& mapCoordsToReplaceWith = resultBit.mapObjectToReplaceWith->getMapCoords();

            for (int my = mapCoordsToReplaceWith.y(); my < mapCoordsToReplaceWith.y() + mapHeight; my++) {
                for (int mx = mapCoordsToReplaceWith.x(); mx < mapCoordsToReplaceWith.x() + mapWidth; mx++) {
                    MapObjectFixed* mapObjectFixedThere = map->getMapObjectFixedAt({mx, my});
                    if (mapObjectFixedThere != nullptr) {
                        context.game->deleteMapObject(mapObjectFixedThere);
                    }
                }
            }
        }

        // Bestehendes Objekt soll abgerissen werden?
        if (resultBit.deleteMapObjectThere) {
            MapObjectFixed* mapObjectFixedThere = map->getMapObjectFixedAt(mapCoords);
            assert (mapObjectFixedThere != nullptr);

            // Kacheln als erledigt markieren
            for (int my = mapObjectFixedThere->getMapCoords().y();
                 my < mapObjectFixedThere->getMapCoords().y() + mapObjectFixedThere->getMapHeight(); my++) {

                for (int mx = mapObjectFixedThere->getMapCoords().x();
                     mx < mapObjectFixedThere->getMapCoords().x() + mapObjectFixedThere->getMapWidth(); mx++) {

                    mapCoordsDone.insert(MapCoords(mx, my));
                }
            }

            // jetzt erst löschen
            context.game->deleteMapObject(mapObjectFixedThere);
        }

        // Neues Objekt hinzufügen
        if (resultBit.mapObjectToReplaceWith) {
            const MapObjectFixed& mapObjectFixedToBuild = *resultBit.mapObjectToReplaceWith;
            const MapObjectType* mapObjectTypeToBuild = mapObjectFixedToBuild.getMapObjectType();

            if (mapObjectTypeToBuild->type == MapObjectTypeClass::HARVESTABLE) {
                context.game->addHarvestable(
                    mapObjectFixedToBuild.getMapCoords(), mapObjectTypeToBuild,
                    mapObjectFixedToBuild.getView());
            }
            else if (mapObjectTypeToBuild->type == MapObjectTypeClass::STREET) {
                context.game->addStreet(
                    mapObjectFixedToBuild.getMapCoords(), mapObjectTypeToBuild,
                    mapObjectFixedToBuild.getView(), &player,
                    dynamic_cast<const Street&>(mapObjectFixedToBuild).streetConnections);
            }
            else {
                context.game->addStructure(
                    mapObjectFixedToBuild.getMapCoords(), mapObjectTypeToBuild,
                    mapObjectFixedToBuild.getView(), &player);
            }

            // Kacheln als erledigt markieren
            for (int my = mapObjectFixedToBuild.getMapCoords().y();
                 my < mapObjectFixedToBuild.getMapCoords().y() + mapObjectFixedToBuild.getMapHeight(); my++) {

                for (int mx = mapObjectFixedToBuild.getMapCoords().x();
                     mx < mapObjectFixedToBuild.getMapCoords().x() + mapObjectFixedToBuild.getMapWidth(); mx++) {

                    mapCoordsDone.insert(MapCoords(mx, my));
                }
            }

            // Baukosten merken, wenn der Bau was kostet
            if (!resultBit.costsNothingBecauseOfChange) {
                buildingCostsEffective += mapObjectTypeToBuild->buildingCosts;
            }
        }

    }

    // Resourcen bezahlen
    GoodsSlot& colonyGoodsSlotTools = colony->getGoods(context.configMgr->getGood("tools"));
    GoodsSlot& colonyGoodsSlotWood = colony->getGoods(context.configMgr->getGood("wood"));
    GoodsSlot& colonyGoodsSlotBricks = colony->getGoods(context.configMgr->getGood("bricks"));

    assert((player.coins - buildingCostsEffective.coins >= 0) &&
           (colonyGoodsSlotTools.inventory - buildingCostsEffective.tools >= 0) &&
           (colonyGoodsSlotWood.inventory - buildingCostsEffective.wood >= 0) &&
           (colonyGoodsSlotBricks.inventory - buildingCostsEffective.bricks >= 0));

    player.coins -= buildingCostsEffective.coins;
    colonyGoodsSlotTools.inventory -= buildingCostsEffective.tools;
    colonyGoodsSlotWood.inventory -= buildingCostsEffective.wood;
    colonyGoodsSlotBricks.inventory -= buildingCostsEffective.bricks;
}