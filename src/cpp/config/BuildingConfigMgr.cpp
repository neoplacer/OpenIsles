#include <string.h>
#include "config/BuildingConfigMgr.h"
#include "map/Structure.h"


BuildingConfigMgr::BuildingConfigMgr() {
    configs = new BuildingConfig*[256];
    memset(configs, 0, 256 * sizeof(BuildingConfig*));
    
    configs[CHAPEL] = new BuildingConfig();
    configs[CHAPEL]->name = "Kapelle";
    configs[CHAPEL]->catchmentArea = new RectangleData<char>(6, 6);
    memcpy(configs[CHAPEL]->catchmentArea->data, "011110111111111111111111111111011110", 36);
    
    configs[WEAPONSMITH] = new BuildingConfig();
    configs[WEAPONSMITH]->name = "Schmied";
    configs[WEAPONSMITH]->catchmentArea = new RectangleData<char>(4, 4);
    memcpy(configs[WEAPONSMITH]->catchmentArea->data, "1111111111111111", 16);
    
    configs[SIGNALFIRE] = new BuildingConfig();
    configs[SIGNALFIRE]->name = "Signalturm";
    configs[SIGNALFIRE]->catchmentArea = new RectangleData<char>(5, 5);
    memcpy(configs[SIGNALFIRE]->catchmentArea->data, "0010001110111110111000100", 25);
    
    configs[HERBARY] = new BuildingConfig();
    configs[HERBARY]->name = "Kräuterfeld";
    configs[HERBARY]->catchmentArea = new RectangleData<char>(11, 11);
    memcpy(configs[HERBARY]->catchmentArea->data, "0000111000000011111000011111111100111111111011111111111111111111111111111111101111111110011111111100001111100000001110000", 121);
    
    configs[BRICKYARD] = new BuildingConfig();
    configs[BRICKYARD]->name = "Steinbruch";
    configs[BRICKYARD]->catchmentArea = new RectangleData<char>(10, 6);
    memcpy(configs[BRICKYARD]->catchmentArea->data, "001111110001111111101111111111111111111101111111100011111100", 60);
    
    configs[BRICKYARD2] = new BuildingConfig();
    configs[BRICKYARD2]->name = "Steinbruch (gedreht)";
    configs[BRICKYARD2]->catchmentArea = new RectangleData<char>(6, 10);
    memcpy(configs[BRICKYARD2]->catchmentArea->data, "001100011110111111111111111111111111111111111111011110001100", 60);
}

BuildingConfigMgr::~BuildingConfigMgr() {
    for(int i = 0; i < 256; i++) {
        if (configs[i] != nullptr) {
            delete configs[i];
        }
    }
    delete[] configs;
}