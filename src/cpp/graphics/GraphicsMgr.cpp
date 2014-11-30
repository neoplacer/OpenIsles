#ifdef WINDOWS
#include <cstring>
#endif
#include <SDL.h>
#include "config/ConfigMgr.h"
#include "graphics/GraphicsMgr.h"

// Aus main.cpp importiert
extern ConfigMgr* configMgr;


GraphicsMgr::GraphicsMgr(SDL_Renderer* const renderer) : renderer(renderer) {
	loadTiles();

    // anisotropic filtering für die Gebäude aktivieren, damit beim Skalieren das Mask-Overlay ordentlich is
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	structures = new MapObjectGraphic*[256];
    memset(structures, 0, 256 * sizeof(MapObjectGraphic*));
    
	structures[StructureType::CHAPEL] = new MapObjectGraphic(renderer, "data/img/objects/chapel.png", 2, 1);
	structures[StructureType::PIONEERS_HOUSE1] = new MapObjectGraphic(renderer, "data/img/objects/pioneers-house1.png", 2, 2);
    structures[StructureType::PIONEERS_HOUSE2] = new MapObjectGraphic(renderer, "data/img/objects/pioneers-house2.png", 2, 2);
    structures[StructureType::PIONEERS_HOUSE3] = new MapObjectGraphic(renderer, "data/img/objects/pioneers-house3.png", 2, 2);
    structures[StructureType::PIONEERS_HOUSE4] = new MapObjectGraphic(renderer, "data/img/objects/pioneers-house4.png", 2, 2);
    structures[StructureType::PIONEERS_HOUSE5] = new MapObjectGraphic(renderer, "data/img/objects/pioneers-house5.png", 2, 2);
	structures[StructureType::STONEMASON] = new MapObjectGraphic(renderer, "data/img/objects/stonemason.png", 2, 2);
    structures[StructureType::OFFICE1] = new MapObjectGraphic(renderer, "data/img/objects/office1.png", 3, 2);
    structures[StructureType::MARKETPLACE] = new MapObjectGraphic(renderer, "data/img/objects/marketplace.png", 4, 3);
    structures[StructureType::FORESTERS] = new MapObjectGraphic(renderer, "data/img/objects/foresters.png", 2, 2);
    structures[StructureType::SHEEP_FARM] = new MapObjectGraphic(renderer, "data/img/objects/sheep-farm.png", 2, 2);
    structures[StructureType::WEAVING_MILL1] = new MapObjectGraphic(renderer, "data/img/objects/weaving-mill1.png", 2, 2);
    structures[StructureType::CATTLE_FARM] = new MapObjectGraphic(renderer, "data/img/objects/cattle-farm.png", 2, 2);
    structures[StructureType::BUTCHERS] = new MapObjectGraphic(renderer, "data/img/objects/butchers.png", 2, 2);
    structures[StructureType::TOOLSMITHS] = new MapObjectGraphic(renderer, "data/img/objects/toolsmiths.png", 2, 2);

    structures[StructureType::STREET] = new MapObjectGraphic(renderer, "data/img/objects/street-straight90.png", 1, 1); // damit was drinsteht. Fürs Malen werden die nachfolgenden Grafiken verwendet
	structures[StructureType::STREET_STRAIGHT_0] = new MapObjectGraphic(renderer, "data/img/objects/street-straight0.png", 1, 1);
	structures[StructureType::STREET_STRAIGHT_90] = new MapObjectGraphic(renderer, "data/img/objects/street-straight90.png", 1, 1);
	structures[StructureType::STREET_CURVE_0] = new MapObjectGraphic(renderer, "data/img/objects/street-curve0.png", 1, 1);
	structures[StructureType::STREET_CURVE_90] = new MapObjectGraphic(renderer, "data/img/objects/street-curve90.png", 1, 1);
	structures[StructureType::STREET_CURVE_180] = new MapObjectGraphic(renderer, "data/img/objects/street-curve180.png", 1, 1);
	structures[StructureType::STREET_CURVE_270] = new MapObjectGraphic(renderer, "data/img/objects/street-curve270.png", 1, 1);
    structures[StructureType::STREET_TEE_0] = new MapObjectGraphic(renderer, "data/img/objects/street-tee0.png", 1, 1);
    structures[StructureType::STREET_TEE_90] = new MapObjectGraphic(renderer, "data/img/objects/street-tee90.png", 1, 1);
    structures[StructureType::STREET_TEE_180] = new MapObjectGraphic(renderer, "data/img/objects/street-tee180.png", 1, 1);
    structures[StructureType::STREET_TEE_270] = new MapObjectGraphic(renderer, "data/img/objects/street-tee270.png", 1, 1);
    structures[StructureType::STREET_CROSS] = new MapObjectGraphic(renderer, "data/img/objects/street-cross.png", 1, 1);

    
    goodsIcons = new PlainGraphic*[GoodsType::MAX_GOOD];
    memset(goodsIcons, 0, GoodsType::MAX_GOOD * sizeof(PlainGraphic*));
    
    goodsIcons[GoodsType::TOOLS] = new PlainGraphic(renderer, "data/img/goods/icon/tools.png");
    goodsIcons[GoodsType::WOOD] = new PlainGraphic(renderer, "data/img/goods/icon/wood.png");
    goodsIcons[GoodsType::BRICKS] = new PlainGraphic(renderer, "data/img/goods/icon/bricks.png");

    goodsMarketplaceIcons = new PlainGraphic*[GoodsType::MAX_GOOD];
    memset(goodsMarketplaceIcons, 0, GoodsType::MAX_GOOD * sizeof(PlainGraphic*));

    goodsMarketplaceIcons[GoodsType::WOOL] = new PlainGraphic(renderer, "data/img/goods/marketplace-icon/wool.png");
    goodsMarketplaceIcons[GoodsType::CATTLE] = new PlainGraphic(renderer, "data/img/goods/marketplace-icon/cattle.png");
    goodsMarketplaceIcons[GoodsType::FOOD] = new PlainGraphic(renderer, "data/img/goods/marketplace-icon/food.png");
    goodsMarketplaceIcons[GoodsType::CLOTH] = new PlainGraphic(renderer, "data/img/goods/marketplace-icon/cloth.png");
    goodsMarketplaceIcons[GoodsType::TOOLS] = new PlainGraphic(renderer, "data/img/goods/marketplace-icon/tools.png");
    goodsMarketplaceIcons[GoodsType::WOOD] = new PlainGraphic(renderer, "data/img/goods/marketplace-icon/wood.png");
    goodsMarketplaceIcons[GoodsType::BRICKS] = new PlainGraphic(renderer, "data/img/goods/marketplace-icon/bricks.png");


    otherGraphics = new PlainGraphic*[OtherGraphic::MAX_GRAPHIC];
    memset(otherGraphics, 0, OtherGraphic::MAX_GRAPHIC * sizeof(PlainGraphic*));
    
    otherGraphics[OtherGraphic::COINS] = new PlainGraphic(renderer, "data/img/coin.png");
    otherGraphics[OtherGraphic::PANEL] = new PlainGraphic(renderer, "data/img/gui/panel.png");
    otherGraphics[OtherGraphic::STATUSBAR] = new PlainGraphic(renderer, "data/img/gui/statusbar.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_GRID] = new PlainGraphic(renderer, "data/img/gui/add-building/add-building-grid.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_CHAPEL] = new PlainGraphic(renderer, "data/img/gui/add-building/chapel.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_MARKETPLACE] = new PlainGraphic(renderer, "data/img/gui/add-building/marketplace.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_OFFICE1] = new PlainGraphic(renderer, "data/img/gui/add-building/office1.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_STREET] = new PlainGraphic(renderer, "data/img/gui/add-building/street.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_SHEEP_FARM] = new PlainGraphic(renderer, "data/img/gui/add-building/sheep-farm.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_WEAVING_MILL1] = new PlainGraphic(renderer, "data/img/gui/add-building/weaving-mill1.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_CATTLE_FARM] = new PlainGraphic(renderer, "data/img/gui/add-building/cattle-farm.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_BUTCHERS] = new PlainGraphic(renderer, "data/img/gui/add-building/butchers.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_TOOLSMITHS] = new PlainGraphic(renderer, "data/img/gui/add-building/toolsmiths.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_HOUSE] = new PlainGraphic(renderer, "data/img/gui/add-building/house.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_STONEMASON] = new PlainGraphic(renderer, "data/img/gui/add-building/stonemason.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_DUMMY] = new PlainGraphic(renderer, "data/img/gui/add-building/dummy.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_GROUP_CRAFTSMAN] = new PlainGraphic(renderer, "data/img/gui/button-add-building-craftsman.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_GROUP_CRAFTSMAN_PRESSED] = new PlainGraphic(renderer, "data/img/gui/button-add-building-craftsman-pressed.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_GROUP_FARM] = new PlainGraphic(renderer, "data/img/gui/button-add-building-farm.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_GROUP_FARM_PRESSED] = new PlainGraphic(renderer, "data/img/gui/button-add-building-farm-pressed.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_GROUP_PORT] = new PlainGraphic(renderer, "data/img/gui/button-add-building-port.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_GROUP_PORT_PRESSED] = new PlainGraphic(renderer, "data/img/gui/button-add-building-port-pressed.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_GROUP_PUBLIC] = new PlainGraphic(renderer, "data/img/gui/button-add-building-public.png");
    otherGraphics[OtherGraphic::ADD_BUILDING_GROUP_PUBLIC_PRESSED] = new PlainGraphic(renderer, "data/img/gui/button-add-building-public-pressed.png");
    otherGraphics[OtherGraphic::PRODUCTION_ARROW] = new PlainGraphic(renderer, "data/img/gui/production-arrow.png");
    otherGraphics[OtherGraphic::PRODUCTION_PLUS] = new PlainGraphic(renderer, "data/img/gui/production-plus.png");
    otherGraphics[OtherGraphic::COAT_OF_ARMS_POPULATION_RED] = new PlainGraphic(renderer, "data/img/gui/coat-of-arms/population/red.png");
    otherGraphics[OtherGraphic::COAT_OF_ARMS_POPULATION_BLUE] = new PlainGraphic(renderer, "data/img/gui/coat-of-arms/population/blue.png");
    otherGraphics[OtherGraphic::COAT_OF_ARMS_POPULATION_YELLOW] = new PlainGraphic(renderer, "data/img/gui/coat-of-arms/population/yellow.png");
    otherGraphics[OtherGraphic::COAT_OF_ARMS_POPULATION_WHITE] = new PlainGraphic(renderer, "data/img/gui/coat-of-arms/population/white.png");
    otherGraphics[OtherGraphic::MUSIC] = new PlainGraphic(renderer, "data/img/gui/button-music.png");
    otherGraphics[OtherGraphic::MUSIC_PRESSED] = new PlainGraphic(renderer, "data/img/gui/button-music-pressed.png");


    animations = new Animation*[AnimationType::MAX_ANIMATION];
    memset(animations, 0, AnimationType::MAX_ANIMATION * sizeof(Animation*));

    animations[AnimationType::CARRIER] = new Animation(renderer, "data/img/objects/carrier.png", 1, 1, 31, Animation::DEFAULT_FPS);
    animations[AnimationType::CART_WITHOUT_CARGO] = new Animation(renderer, "data/img/objects/cart-without-cargo.png", 1, 1, 32, Animation::DEFAULT_FPS);
    animations[AnimationType::CART_WITH_CARGO] = new Animation(renderer, "data/img/objects/cart-with-cargo.png", 1, 1, 32, Animation::DEFAULT_FPS);
}

GraphicsMgr::~GraphicsMgr() {
	for (int i = 0; i < 128; i++) {
		if (tiles[i] != nullptr) {
			delete tiles[i];
		}
	}
	delete[] tiles;

	for (int i = 0; i < 256; i++) {
        if (structures[i] != nullptr) {
			delete structures[i];
		}
	}
	delete[] structures;
    
    for (int i = 0; i < GoodsType::MAX_GOOD; i++) {
        if (goodsIcons[i] != nullptr) {
			delete goodsIcons[i];
		}
        if (goodsMarketplaceIcons[i] != nullptr) {
            delete goodsMarketplaceIcons[i];
        }
	}
	delete[] goodsIcons;
    delete[] goodsMarketplaceIcons;
    
    for (int i = 0; i < OtherGraphic::MAX_GRAPHIC; i++) {
        if (otherGraphics[i] != nullptr) {
			delete otherGraphics[i];
		}
	}
	delete[] otherGraphics;

    for (int i = 0; i < AnimationType::MAX_ANIMATION; i++) {
        if (animations[i] != nullptr) {
            delete animations[i];
        }
    }
    delete[] animations;
}

void GraphicsMgr::loadTiles() {
    // nearest pixel sampling für die Kacheln, damit die fließend ineinander übergehen
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	tiles = new MapObjectGraphic*[128];
    memset(tiles, 0, 128 * sizeof(MapObjectGraphic*));

    for (unsigned char tileIndex = 0; tileIndex < 128; tileIndex++) {
        const MapTileConfig* mapTileConfig = configMgr->getMapTileConfig(tileIndex);
        if (mapTileConfig == nullptr) {
            continue;
        }

        tiles[tileIndex] = new MapObjectGraphic(renderer, mapTileConfig->graphicsFile, 1, 1);
    }
}
