#include <string.h>
#include <game/Colony.h>
#include "config/BuildingConfigMgr.h"
#include "map/Structure.h"


BuildingConfigMgr::BuildingConfigMgr() {
    configs = new BuildingConfig*[StructureType::MAX_STRUCTURE];
    memset(configs, 0, StructureType::MAX_STRUCTURE * sizeof(BuildingConfig*));
    
    configs[StructureType::STREET_STRAIGHT_0] = new BuildingConfig();
    configs[StructureType::STREET_STRAIGHT_0]->name = "Pflasterstraße";
    configs[StructureType::STREET_STRAIGHT_0]->catchmentArea = nullptr;
    configs[StructureType::STREET_STRAIGHT_0]->buildingCosts = { 5, 0, 0, 0 };
    configs[StructureType::STREET_STRAIGHT_0]->buildingProduction = { GoodsType::NO_GOODS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };
    
    configs[StructureType::STREET_STRAIGHT_90] = new BuildingConfig();
    configs[StructureType::STREET_STRAIGHT_90]->name = "Pflasterstraße";
    configs[StructureType::STREET_STRAIGHT_90]->catchmentArea = nullptr;
    configs[StructureType::STREET_STRAIGHT_90]->buildingCosts = { 5, 0, 0, 0 };
    configs[StructureType::STREET_STRAIGHT_90]->buildingProduction = { GoodsType::NO_GOODS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };
    
    configs[StructureType::STREET_CURVE_0] = new BuildingConfig();
    configs[StructureType::STREET_CURVE_0]->name = "Pflasterstraße";
    configs[StructureType::STREET_CURVE_0]->catchmentArea = nullptr;
    configs[StructureType::STREET_CURVE_0]->buildingCosts = { 5, 0, 0, 0 };
    configs[StructureType::STREET_CURVE_0]->buildingProduction = { GoodsType::NO_GOODS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };
    
    configs[StructureType::STREET_CURVE_90] = new BuildingConfig();
    configs[StructureType::STREET_CURVE_90]->name = "Pflasterstraße";
    configs[StructureType::STREET_CURVE_90]->catchmentArea = nullptr;
    configs[StructureType::STREET_CURVE_90]->buildingCosts = { 5, 0, 0, 0 };
    configs[StructureType::STREET_CURVE_90]->buildingProduction = { GoodsType::NO_GOODS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };
    
    configs[StructureType::STREET_CURVE_180] = new BuildingConfig();
    configs[StructureType::STREET_CURVE_180]->name = "Pflasterstraße";
    configs[StructureType::STREET_CURVE_180]->catchmentArea = nullptr;
    configs[StructureType::STREET_CURVE_180]->buildingCosts = { 5, 0, 0, 0 };
    configs[StructureType::STREET_CURVE_180]->buildingProduction = { GoodsType::NO_GOODS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };
    
    configs[StructureType::STREET_CURVE_270] = new BuildingConfig();
    configs[StructureType::STREET_CURVE_270]->name = "Pflasterstraße";
    configs[StructureType::STREET_CURVE_270]->catchmentArea = nullptr;
    configs[StructureType::STREET_CURVE_270]->buildingCosts = { 5, 0, 0, 0 };
    configs[StructureType::STREET_CURVE_270]->buildingProduction = { GoodsType::NO_GOODS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };

    configs[StructureType::STREET_TEE_0] = new BuildingConfig();
    configs[StructureType::STREET_TEE_0]->name = "Pflasterstraße";
    configs[StructureType::STREET_TEE_0]->catchmentArea = nullptr;
    configs[StructureType::STREET_TEE_0]->buildingCosts = { 5, 0, 0, 0 };
    configs[StructureType::STREET_TEE_0]->buildingProduction = { GoodsType::NO_GOODS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };

    configs[StructureType::STREET_TEE_90] = new BuildingConfig();
    configs[StructureType::STREET_TEE_90]->name = "Pflasterstraße";
    configs[StructureType::STREET_TEE_90]->catchmentArea = nullptr;
    configs[StructureType::STREET_TEE_90]->buildingCosts = { 5, 0, 0, 0 };
    configs[StructureType::STREET_TEE_90]->buildingProduction = { GoodsType::NO_GOODS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };

    configs[StructureType::STREET_TEE_180] = new BuildingConfig();
    configs[StructureType::STREET_TEE_180]->name = "Pflasterstraße";
    configs[StructureType::STREET_TEE_180]->catchmentArea = nullptr;
    configs[StructureType::STREET_TEE_180]->buildingCosts = { 5, 0, 0, 0 };
    configs[StructureType::STREET_TEE_180]->buildingProduction = { GoodsType::NO_GOODS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };

    configs[StructureType::STREET_TEE_270] = new BuildingConfig();
    configs[StructureType::STREET_TEE_270]->name = "Pflasterstraße";
    configs[StructureType::STREET_TEE_270]->catchmentArea = nullptr;
    configs[StructureType::STREET_TEE_270]->buildingCosts = { 5, 0, 0, 0 };
    configs[StructureType::STREET_TEE_270]->buildingProduction = { GoodsType::NO_GOODS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };

    configs[StructureType::STREET_CROSS] = new BuildingConfig();
    configs[StructureType::STREET_CROSS]->name = "Pflasterstraße";
    configs[StructureType::STREET_CROSS]->catchmentArea = nullptr;
    configs[StructureType::STREET_CROSS]->buildingCosts = { 5, 0, 0, 0 };
    configs[StructureType::STREET_CROSS]->buildingProduction = { GoodsType::NO_GOODS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };
    
    configs[StructureType::CHAPEL] = new BuildingConfig();
    configs[StructureType::CHAPEL]->name = "Kapelle";
    configs[StructureType::CHAPEL]->catchmentArea = new RectangleData<char>(6, 6);
    memcpy(configs[StructureType::CHAPEL]->catchmentArea->data, "011110111111111111111111111111011110", 36);
    configs[StructureType::CHAPEL]->buildingCosts = { 100, 2, 5, 0 };
    configs[StructureType::CHAPEL]->buildingProduction = { GoodsType::NO_GOODS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };
    
    configs[StructureType::PIONEERS_HOUSE1] = new BuildingConfig();
    configs[StructureType::PIONEERS_HOUSE1]->name = "Haus (Pioniere)";
    configs[StructureType::PIONEERS_HOUSE1]->catchmentArea = new RectangleData<char>(14, 14);
    memcpy(configs[StructureType::PIONEERS_HOUSE1]->catchmentArea->data, "0000111111000000011111111000001111111111000111111111111011111111111111111111111111111111111111111111111111111111111111111111111111111111111101111111111110001111111111000001111111100000001111110000", 196);
    configs[StructureType::PIONEERS_HOUSE1]->buildingCosts = { 0, 0, 3, 0 };
    configs[StructureType::PIONEERS_HOUSE1]->buildingProduction = { GoodsType::NO_GOODS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };
    
    configs[StructureType::SIGNALFIRE] = new BuildingConfig();
    configs[StructureType::SIGNALFIRE]->name = "Signalturm";
    configs[StructureType::SIGNALFIRE]->catchmentArea = new RectangleData<char>(5, 5);
    memcpy(configs[StructureType::SIGNALFIRE]->catchmentArea->data, "0010001110111110111000100", 25);
    configs[StructureType::SIGNALFIRE]->buildingCosts = { 50, 1, 7, 0 };
    configs[StructureType::SIGNALFIRE]->buildingProduction = { GoodsType::BRICKS, GoodsType::TOOLS, GoodsType::WOOD };
    
    configs[StructureType::HERBARY] = new BuildingConfig();
    configs[StructureType::HERBARY]->name = "Kräuterfeld";
    configs[StructureType::HERBARY]->catchmentArea = new RectangleData<char>(11, 11);
    memcpy(configs[StructureType::HERBARY]->catchmentArea->data, "0000111000000011111000011111111100111111111011111111111111111111111111111111101111111110011111111100001111100000001110000", 121);
    configs[StructureType::HERBARY]->buildingCosts = { 100, 0, 0, 0 };
    configs[StructureType::HERBARY]->buildingProduction = { GoodsType::TOOLS, GoodsType::BRICKS, GoodsType::NO_GOODS };
    
    configs[StructureType::BRICKYARD] = new BuildingConfig();
    configs[StructureType::BRICKYARD]->name = "Steinbruch";
    configs[StructureType::BRICKYARD]->catchmentArea = new RectangleData<char>(10, 6);
    memcpy(configs[StructureType::BRICKYARD]->catchmentArea->data, "001111110001111111101111111111111111111101111111100011111100", 60);
    configs[StructureType::BRICKYARD]->buildingCosts = { 100, 5, 5, 0 };
    configs[StructureType::BRICKYARD]->buildingProduction = { GoodsType::BRICKS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };
    
    configs[StructureType::BRICKYARD2] = new BuildingConfig();
    configs[StructureType::BRICKYARD2]->name = "Steinbruch (gedreht)";
    configs[StructureType::BRICKYARD2]->catchmentArea = new RectangleData<char>(6, 10);
    memcpy(configs[StructureType::BRICKYARD2]->catchmentArea->data, "001100011110111111111111111111111111111111111111011110001100", 60);
    configs[StructureType::BRICKYARD2]->buildingCosts = { 100, 5, 5, 0 };
    configs[StructureType::BRICKYARD2]->buildingProduction = { GoodsType::BRICKS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };
    
    configs[StructureType::OFFICE1] = new BuildingConfig();
    configs[StructureType::OFFICE1]->name = "Kontor";
    configs[StructureType::OFFICE1]->catchmentArea = new RectangleData<char>(38, 38);
    memcpy(configs[StructureType::OFFICE1]->catchmentArea->data, "0000000000000001111111100000000000000000000000000111111111111111100000000000000000000111111111111111111110000000000000000011111111111111111111110000000000000011111111111111111111111111000000000001111111111111111111111111111000000000111111111111111111111111111111000000001111111111111111111111111111110000000111111111111111111111111111111110000011111111111111111111111111111111110000111111111111111111111111111111111100011111111111111111111111111111111111100111111111111111111111111111111111111001111111111111111111111111111111111110011111111111111111111111111111111111101111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111011111111111111111111111111111111111100111111111111111111111111111111111111001111111111111111111111111111111111110011111111111111111111111111111111111100011111111111111111111111111111111110000111111111111111111111111111111111100000111111111111111111111111111111110000000111111111111111111111111111111000000001111111111111111111111111111110000000001111111111111111111111111111000000000001111111111111111111111111100000000000000111111111111111111111100000000000000000111111111111111111110000000000000000000011111111111111110000000000000000000000000011111111000000000000000", 1444);
    configs[StructureType::OFFICE1]->buildingCosts = { 100, 3, 6, 0 };
    configs[StructureType::OFFICE1]->buildingProduction = { GoodsType::NO_GOODS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };
    
    configs[StructureType::MARKETPLACE] = new BuildingConfig();
    configs[StructureType::MARKETPLACE]->name = "Marktplatz";
    configs[StructureType::MARKETPLACE]->catchmentArea = new RectangleData<char>(38, 38);
    memcpy(configs[StructureType::MARKETPLACE]->catchmentArea->data, "0000000000000001111111100000000000000000000000000111111111111111100000000000000000000111111111111111111110000000000000000011111111111111111111110000000000000011111111111111111111111111000000000001111111111111111111111111111000000000111111111111111111111111111111000000001111111111111111111111111111110000000111111111111111111111111111111110000011111111111111111111111111111111110000111111111111111111111111111111111100011111111111111111111111111111111111100111111111111111111111111111111111111001111111111111111111111111111111111110011111111111111111111111111111111111101111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111011111111111111111111111111111111111100111111111111111111111111111111111111001111111111111111111111111111111111110011111111111111111111111111111111111100011111111111111111111111111111111110000111111111111111111111111111111111100000111111111111111111111111111111110000000111111111111111111111111111111000000001111111111111111111111111111110000000001111111111111111111111111111000000000001111111111111111111111111100000000000000111111111111111111111100000000000000000111111111111111111110000000000000000000011111111111111110000000000000000000000000011111111000000000000000", 1444);
    configs[StructureType::MARKETPLACE]->buildingCosts = { 200, 4, 10, 0 };
    configs[StructureType::MARKETPLACE]->buildingProduction = { GoodsType::NO_GOODS, GoodsType::NO_GOODS, GoodsType::NO_GOODS };
    
    configs[StructureType::FORESTERS] = new BuildingConfig();
    configs[StructureType::FORESTERS]->name = "Forsthaus";
    configs[StructureType::FORESTERS]->catchmentArea = new RectangleData<char>(8, 8);
    memcpy(configs[StructureType::FORESTERS]->catchmentArea->data, "0011110001111110111111111111111111111111111111110111111000111100", 64);
    configs[StructureType::FORESTERS]->buildingCosts = { 50, 2, 0, 0 };
    configs[StructureType::FORESTERS]->buildingProduction = { GoodsType::WOOD, GoodsType::NO_GOODS, GoodsType::NO_GOODS };
}

BuildingConfigMgr::~BuildingConfigMgr() {
    for(int i = 0; i < StructureType::MAX_STRUCTURE; i++) {
        if (configs[i] != nullptr) {
            delete configs[i];
        }
    }
    delete[] configs;
}