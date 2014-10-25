#include <string.h>
#include "config/BuildingConfigMgr.h"
#include "game/Colony.h"
#include "map/Structure.h"


BuildingConfigMgr::BuildingConfigMgr() {
    configs = new BuildingConfig*[StructureType::MAX_STRUCTURE];
    memset(configs, 0, StructureType::MAX_STRUCTURE * sizeof(BuildingConfig*));

    for (int i = StructureType::STREET; i <= StructureType::STREET_CROSS; i++) {
        configs[i] = new BuildingConfig();
        configs[i]->name = "Pflasterstraße";
        configs[i]->catchmentArea = nullptr;
        configs[i]->buildingCosts = { 5, 0, 0, 0 };
        configs[i]->buildingProduction = { GoodsSlot(), GoodsSlot(), GoodsSlot() };
    }

    configs[StructureType::CHAPEL] = new BuildingConfig();
    configs[StructureType::CHAPEL]->name = "Kapelle";
    configs[StructureType::CHAPEL]->catchmentArea = new RectangleData<char>(18, 17);
    memcpy(configs[StructureType::CHAPEL]->catchmentArea->data, "000000111111000000000011111111110000000111111111111000001111111111111100011111111111111110011111111111111110111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111011111111111111110011111111111111110001111111111111100000111111111111000000011111111110000000000111111000000", 306);
    configs[StructureType::CHAPEL]->buildingCosts = { 100, 2, 5, 0 };
    configs[StructureType::CHAPEL]->buildingProduction = { GoodsSlot(), GoodsSlot(), GoodsSlot() };

    configs[StructureType::PIONEERS_HOUSE1] = new BuildingConfig();
    configs[StructureType::PIONEERS_HOUSE1]->name = "Haus (Pioniere)";
    configs[StructureType::PIONEERS_HOUSE1]->catchmentArea = new RectangleData<char>(14, 14);
    memcpy(configs[StructureType::PIONEERS_HOUSE1]->catchmentArea->data, "0000111111000000011111111000001111111111000111111111111011111111111111111111111111111111111111111111111111111111111111111111111111111111111101111111111110001111111111000001111111100000001111110000", 196);
    configs[StructureType::PIONEERS_HOUSE1]->buildingCosts = { 0, 0, 3, 0 };
    configs[StructureType::PIONEERS_HOUSE1]->buildingProduction = { GoodsSlot(), GoodsSlot(), GoodsSlot() };

    configs[StructureType::SIGNALFIRE] = new BuildingConfig();
    configs[StructureType::SIGNALFIRE]->name = "Signalturm";
    configs[StructureType::SIGNALFIRE]->catchmentArea = new RectangleData<char>(5, 5);
    memcpy(configs[StructureType::SIGNALFIRE]->catchmentArea->data, "0010001110111110111000100", 25);
    configs[StructureType::SIGNALFIRE]->buildingCosts = { 50, 1, 7, 0 };
    configs[StructureType::SIGNALFIRE]->buildingProduction = {
        GoodsSlot(GoodsType::BRICKS, 5),
        GoodsSlot(GoodsType::TOOLS, 10),
        GoodsSlot(GoodsType::WOOD, 3)
    };
    configs[StructureType::SIGNALFIRE]->productionRate = 1.5;
    configs[StructureType::SIGNALFIRE]->inputConsumptionRate = 1.5;
    configs[StructureType::SIGNALFIRE]->input2ConsumptionRate = 1.5;

    configs[StructureType::HERBARY] = new BuildingConfig();
    configs[StructureType::HERBARY]->name = "Kräuterfeld";
    configs[StructureType::HERBARY]->catchmentArea = new RectangleData<char>(11, 11);
    memcpy(configs[StructureType::HERBARY]->catchmentArea->data, "0000111000000011111000011111111100111111111011111111111111111111111111111111101111111110011111111100001111100000001110000", 121);
    configs[StructureType::HERBARY]->buildingCosts = { 100, 0, 0, 0 };
    configs[StructureType::HERBARY]->buildingProduction = {
        GoodsSlot(GoodsType::TOOLS, 4),
        GoodsSlot(GoodsType::BRICKS, 4),
        GoodsSlot()
    };
    configs[StructureType::HERBARY]->productionRate = 2.4;
    configs[StructureType::HERBARY]->inputConsumptionRate = 3.6;

    configs[StructureType::BRICKYARD] = new BuildingConfig();
    configs[StructureType::BRICKYARD]->name = "Steinbruch";
    configs[StructureType::BRICKYARD]->catchmentArea = new RectangleData<char>(10, 6);
    memcpy(configs[StructureType::BRICKYARD]->catchmentArea->data, "001111110001111111101111111111111111111101111111100011111100", 60);
    configs[StructureType::BRICKYARD]->buildingCosts = { 100, 5, 5, 0 };
    configs[StructureType::BRICKYARD]->buildingProduction = {
        GoodsSlot(GoodsType::BRICKS, 10),
        GoodsSlot(),
        GoodsSlot()
    };
    configs[StructureType::BRICKYARD]->productionRate = 0.7;

    configs[StructureType::BRICKYARD2] = new BuildingConfig();
    configs[StructureType::BRICKYARD2]->name = "Steinbruch (gedreht)";
    configs[StructureType::BRICKYARD2]->catchmentArea = new RectangleData<char>(6, 10);
    memcpy(configs[StructureType::BRICKYARD2]->catchmentArea->data, "001100011110111111111111111111111111111111111111011110001100", 60);
    configs[StructureType::BRICKYARD2]->buildingCosts = { 100, 5, 5, 0 };
    configs[StructureType::BRICKYARD2]->buildingProduction = {
        GoodsSlot(GoodsType::BRICKS, 10),
        GoodsSlot(),
        GoodsSlot()
    };
    configs[StructureType::BRICKYARD2]->productionRate = 0.7;

    configs[StructureType::OFFICE1] = new BuildingConfig();
    configs[StructureType::OFFICE1]->name = "Kontor";
    configs[StructureType::OFFICE1]->catchmentArea = new RectangleData<char>(38, 38);
    memcpy(configs[StructureType::OFFICE1]->catchmentArea->data, "0000000000000001111111100000000000000000000000000111111111111111100000000000000000000111111111111111111110000000000000000011111111111111111111110000000000000011111111111111111111111111000000000001111111111111111111111111111000000000111111111111111111111111111111000000001111111111111111111111111111110000000111111111111111111111111111111110000011111111111111111111111111111111110000111111111111111111111111111111111100011111111111111111111111111111111111100111111111111111111111111111111111111001111111111111111111111111111111111110011111111111111111111111111111111111101111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111011111111111111111111111111111111111100111111111111111111111111111111111111001111111111111111111111111111111111110011111111111111111111111111111111111100011111111111111111111111111111111110000111111111111111111111111111111111100000111111111111111111111111111111110000000111111111111111111111111111111000000001111111111111111111111111111110000000001111111111111111111111111111000000000001111111111111111111111111100000000000000111111111111111111111100000000000000000111111111111111111110000000000000000000011111111111111110000000000000000000000000011111111000000000000000", 1444);
    configs[StructureType::OFFICE1]->buildingCosts = { 100, 3, 6, 0 };
    configs[StructureType::OFFICE1]->buildingProduction = { GoodsSlot(), GoodsSlot(), GoodsSlot() };

    configs[StructureType::MARKETPLACE] = new BuildingConfig();
    configs[StructureType::MARKETPLACE]->name = "Marktplatz";
    configs[StructureType::MARKETPLACE]->catchmentArea = new RectangleData<char>(38, 38);
    memcpy(configs[StructureType::MARKETPLACE]->catchmentArea->data, "0000000000000001111111100000000000000000000000000111111111111111100000000000000000000111111111111111111110000000000000000011111111111111111111110000000000000011111111111111111111111111000000000001111111111111111111111111111000000000111111111111111111111111111111000000001111111111111111111111111111110000000111111111111111111111111111111110000011111111111111111111111111111111110000111111111111111111111111111111111100011111111111111111111111111111111111100111111111111111111111111111111111111001111111111111111111111111111111111110011111111111111111111111111111111111101111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111011111111111111111111111111111111111100111111111111111111111111111111111111001111111111111111111111111111111111110011111111111111111111111111111111111100011111111111111111111111111111111110000111111111111111111111111111111111100000111111111111111111111111111111110000000111111111111111111111111111111000000001111111111111111111111111111110000000001111111111111111111111111111000000000001111111111111111111111111100000000000000111111111111111111111100000000000000000111111111111111111110000000000000000000011111111111111110000000000000000000000000011111111000000000000000", 1444);
    configs[StructureType::MARKETPLACE]->buildingCosts = { 200, 4, 10, 0 };
    configs[StructureType::MARKETPLACE]->buildingProduction = { GoodsSlot(), GoodsSlot(), GoodsSlot() };

    configs[StructureType::FORESTERS] = new BuildingConfig();
    configs[StructureType::FORESTERS]->name = "Forsthaus";
    configs[StructureType::FORESTERS]->catchmentArea = new RectangleData<char>(8, 8);
    memcpy(configs[StructureType::FORESTERS]->catchmentArea->data, "0011110001111110111111111111111111111111111111110111111000111100", 64);
    configs[StructureType::FORESTERS]->buildingCosts = { 50, 2, 0, 0 };
    configs[StructureType::FORESTERS]->buildingProduction = {
        GoodsSlot(GoodsType::WOOD, 10),
        GoodsSlot(),
        GoodsSlot()
    };
    configs[StructureType::FORESTERS]->productionRate = 3.5;

    configs[StructureType::SHEEP_FARM] = new BuildingConfig();
    configs[StructureType::SHEEP_FARM]->name = "Schaffarm";
    configs[StructureType::SHEEP_FARM]->catchmentArea = new RectangleData<char>(8, 8);
    memcpy(configs[StructureType::SHEEP_FARM]->catchmentArea->data, "0011110001111110111111111111111111111111111111110111111000111100", 64);
    configs[StructureType::SHEEP_FARM]->buildingCosts = { 200, 2, 4, 0 };
    configs[StructureType::SHEEP_FARM]->buildingProduction = {
        GoodsSlot(GoodsType::WOOL, 3),
        GoodsSlot(),
        GoodsSlot()
    };
    configs[StructureType::SHEEP_FARM]->productionRate = 30;

    configs[StructureType::WEAVING_MILL1] = new BuildingConfig();
    configs[StructureType::WEAVING_MILL1]->name = "Webstube";
    configs[StructureType::WEAVING_MILL1]->catchmentArea = new RectangleData<char>(32, 32);
    memcpy(configs[StructureType::WEAVING_MILL1]->catchmentArea->data, "0000000000001111111100000000000000000000011111111111111000000000000000011111111111111111100000000000001111111111111111111100000000000111111111111111111111100000000011111111111111111111111100000001111111111111111111111111100000111111111111111111111111111100001111111111111111111111111111000111111111111111111111111111111001111111111111111111111111111110011111111111111111111111111111101111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111011111111111111111111111111111100111111111111111111111111111111001111111111111111111111111111110001111111111111111111111111111000011111111111111111111111111110000011111111111111111111111111000000011111111111111111111111100000000011111111111111111111110000000000011111111111111111111000000000000011111111111111111100000000000000001111111111111100000000000000000000011111111000000000000", 1024);
    configs[StructureType::WEAVING_MILL1]->buildingCosts = { 200, 3, 6, 0 };
    configs[StructureType::WEAVING_MILL1]->buildingProduction = {
        GoodsSlot(GoodsType::CLOTH, 7),
        GoodsSlot(GoodsType::WOOL, 7),
        GoodsSlot()
    };
    configs[StructureType::WEAVING_MILL1]->productionRate = 10.0;
    configs[StructureType::WEAVING_MILL1]->inputConsumptionRate = 5.0;
}

BuildingConfigMgr::~BuildingConfigMgr() {
    for(int i = 0; i < StructureType::MAX_STRUCTURE; i++) {
        if (configs[i] != nullptr) {
            delete configs[i];
        }
    }
    delete[] configs;
}