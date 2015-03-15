#include <gtest/gtest.h>
#include "graphics/graphic/nosdl/NoSDLGraphic.h"
#include "graphics/mgr/nosdl/NoSDLGraphicsMgr.h"
#include "graphics/renderer/nosdl/NoSDLRenderer.h"
#include "map/Direction.h"
#include "map/Map.h"


/**
 * @brief Fake-Objekt, was eine 10x10-Kacheln große Map vorgaukelt,
 * damit wir die Koordinatenumrechnung testen können.
 */
class MapCoordUtilsTestMap : public Map {

public:
    MapCoordUtilsTestMap(const Context* const context) : Map(context) {
    }

    int getWidth() const override {
        return 10;
    }
    int getHeight() const override {
        return 10;
    }

};


class MapCoordUtilsTest : public ::testing::Test {

protected:
    Context context;

    IGraphicsMgr* noSdlGraphicsMgr;

    MapCoordUtilsTestMap* map;

    static const int countTestCases = 7 * 4;
    struct {
        MapCoords mapCoords;
        ScreenCoords screenCoords;
        FourthDirection view;
    } testCases[countTestCases] = {
        { MapCoords(0, 0), ScreenCoords(0, 0), Direction::SOUTH },
        { MapCoords(1, 0), ScreenCoords(32, 16), Direction::SOUTH },
        { MapCoords(0, 1), ScreenCoords(-32, 16), Direction::SOUTH },
        { MapCoords(1, 1), ScreenCoords(0, 32), Direction::SOUTH },
        { MapCoords(9, 9), ScreenCoords(0, 288), Direction::SOUTH },
        { MapCoords(0, 9), ScreenCoords(-288, 144), Direction::SOUTH },
        { MapCoords(9, 0), ScreenCoords(288, 144), Direction::SOUTH },

        { MapCoords(0, 0), ScreenCoords(0, 0), Direction::EAST },
        { MapCoords(1, 0), ScreenCoords(-32, 16), Direction::EAST },
        { MapCoords(0, 1), ScreenCoords(-32, -16), Direction::EAST },
        { MapCoords(1, 1), ScreenCoords(-64, 0), Direction::EAST },
        { MapCoords(9, 9), ScreenCoords(-576, 0), Direction::EAST },
        { MapCoords(0, 9), ScreenCoords(-288, -144), Direction::EAST },
        { MapCoords(9, 0), ScreenCoords(-288, 144), Direction::EAST },

        { MapCoords(0, 0), ScreenCoords(0, 0), Direction::NORTH },
        { MapCoords(1, 0), ScreenCoords(-32, -16), Direction::NORTH },
        { MapCoords(0, 1), ScreenCoords(32, -16), Direction::NORTH },
        { MapCoords(1, 1), ScreenCoords(0, -32), Direction::NORTH },
        { MapCoords(9, 9), ScreenCoords(0, -288), Direction::NORTH },
        { MapCoords(0, 9), ScreenCoords(288, -144), Direction::NORTH },
        { MapCoords(9, 0), ScreenCoords(-288, -144), Direction::NORTH },

        { MapCoords(0, 0), ScreenCoords(0, 0), Direction::WEST },
        { MapCoords(1, 0), ScreenCoords(32, -16), Direction::WEST },
        { MapCoords(0, 1), ScreenCoords(32, 16), Direction::WEST },
        { MapCoords(1, 1), ScreenCoords(64, 0), Direction::WEST },
        { MapCoords(9, 9), ScreenCoords(576, 0), Direction::WEST },
        { MapCoords(0, 9), ScreenCoords(288, 144), Direction::WEST },
        { MapCoords(9, 0), ScreenCoords(288, -144), Direction::WEST }
    };

protected:
    virtual void SetUp() {
        map = new MapCoordUtilsTestMap(&context);
    }

    virtual void TearDown() {
        delete map;
    }

};

TEST_F(MapCoordUtilsTest, mapToScreenCoords) {
    map->setMapCoordsCentered(MapCoords(0, 0));

    for (int i = 0; i < countTestCases; i++) {
        ScreenCoords actual = MapCoordUtils::mapToScreenCoords(testCases[i].mapCoords, testCases[i].view, *map);
        ASSERT_EQ(testCases[i].screenCoords, actual);
    }
}

TEST_F(MapCoordUtilsTest, screenToMapCoords) {
    map->setMapCoordsCentered(MapCoords(0, 0));

    for (int i = 0; i < countTestCases; i++) {
        MapCoords actual = MapCoordUtils::screenToMapCoords(testCases[i].screenCoords, testCases[i].view, *map);
        ASSERT_EQ(testCases[i].mapCoords, actual);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * @brief GraphicsMgr, der immer die passende Grafik für den MapCoordUtilsTest vorgaukelt
 */
class MapCoordUtilsTestGraphicsMgr : public NoSDLGraphicsMgr {

private:
    GraphicSet* graphicSet;

public:
    MapCoordUtilsTestGraphicsMgr(GraphicSet* const graphicSet) :
        NoSDLGraphicsMgr(nullptr, nullptr), graphicSet(graphicSet) {}

    virtual std::string getGraphicSetNameForStructure(StructureType structureType) const {
        return "irrelevant :-)";
    }

    virtual const GraphicSet* getGraphicSet(std::string graphicSetName) const {
        return graphicSet;
    }
};

/**
 * @brief Wir sind auf (5, 5) zentriert und haben ein 4x3-Gebäude auf (6, 6) stehen.
 * Wir testen nun, ob je nach Ansicht die drawCoords korrekt sind.
 */
TEST_F(MapCoordUtilsTest, getDrawCoordsForBuilding) {
    // Testaufbau
    map->setMapCoordsCentered(MapCoords(5, 5));
    map->setScreenZoom(1);

    IGraphic* graphic1 = new NoSDLGraphic(nullptr, "unsere Grafik", 224, 160, 4, 3);
    IGraphic* graphic2 = new NoSDLGraphic(nullptr, "unsere Grafik", 224, 160, 3, 4);
    IGraphic* graphic3 = new NoSDLGraphic(nullptr, "unsere Grafik", 224, 160, 4, 3);
    IGraphic* graphic4 = new NoSDLGraphic(nullptr, "unsere Grafik", 224, 160, 3, 4);

    GraphicSet graphicSet;
    graphicSet.addByView(Direction::SOUTH, new Animation(graphic1));
    graphicSet.addByView(Direction::EAST, new Animation(graphic2));
    graphicSet.addByView(Direction::NORTH, new Animation(graphic3));
    graphicSet.addByView(Direction::WEST, new Animation(graphic4));

    MapCoordUtilsTestGraphicsMgr graphicsMgr(&graphicSet);

    Building building;
    building.setMapCoords(MapCoords(6, 6));
    building.setMapWidth(4);
    building.setMapHeight(3);
    building.setView(Direction::SOUTH);

    // Testdurchführung

    map->setScreenView(Direction::SOUTH);
    Rect actual = MapCoordUtils::getDrawCoordsForBuilding(*map, &graphicsMgr, &building);
    ASSERT_EQ(288, actual.x);
    ASSERT_EQ(319, actual.y);
    ASSERT_EQ(224, actual.w);
    ASSERT_EQ(160, actual.h);

    map->setScreenView(Direction::EAST);
    actual = MapCoordUtils::getDrawCoordsForBuilding(*map, &graphicsMgr, &building);
    ASSERT_EQ(128, actual.x);
    ASSERT_EQ(255, actual.y);
    ASSERT_EQ(224, actual.w);
    ASSERT_EQ(160, actual.h);

    map->setScreenView(Direction::NORTH);
    actual = MapCoordUtils::getDrawCoordsForBuilding(*map, &graphicsMgr, &building);
    ASSERT_EQ(256, actual.x);
    ASSERT_EQ(175, actual.y);
    ASSERT_EQ(224, actual.w);
    ASSERT_EQ(160, actual.h);

    map->setScreenView(Direction::WEST);
    actual = MapCoordUtils::getDrawCoordsForBuilding(*map, &graphicsMgr, &building);
    ASSERT_EQ(416, actual.x);
    ASSERT_EQ(239, actual.y);
    ASSERT_EQ(224, actual.w);
    ASSERT_EQ(160, actual.h);
}