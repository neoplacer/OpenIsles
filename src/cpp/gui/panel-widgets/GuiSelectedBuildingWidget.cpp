#include "config/ConfigMgr.h"
#include "game/Game.h"
#include "gui/FontMgr.h"
#include "gui/GuiMgr.h"
#include "gui/GuiUtils.h"
#include "gui/panel-widgets/GuiSelectedBuildingWidget.h"


static SDL_Color colorBlack = {0, 0, 0, 255};
static SDL_Color colorWhite = {255, 255, 255, 255};

// Aus main.cpp importiert
extern ConfigMgr* configMgr;
extern FontMgr* fontMgr;
extern Game* game;
extern GraphicsMgr* graphicsMgr;
extern GuiMgr* guiMgr;


GuiSelectedBuildingWidget::GuiSelectedBuildingWidget() {
    // TODO Child-Buttons für Stilllegen und "Abholung verbieten"
}

void GuiSelectedBuildingWidget::renderElement(SDL_Renderer* renderer) {
    const Building* selectedBuilding = nullptr;

    const MapObject* selectedMapObject = game->getMap()->getSelectedMapObject();
    if (selectedMapObject != nullptr) {
        selectedBuilding = reinterpret_cast<const Building*>(selectedMapObject);
    }

    if (selectedBuilding == nullptr) {
        // Kein Gebäude ausgewählt, dann nix zu tun
        return;
    }
    if (guiMgr->getPanelState().selectedPanelButton == PanelButton::ADD_BUILDING) {
        // Wenn wir ein Gebäude platzieren, dürfen wir dieses Widget nicht zeichnen, damit sich das nicht mit dem
        // GuiAddBuildingWidget überschneidet.
        return;
    }

    int windowX, windowY;
    getWindowCoords(windowX, windowY);

    StructureType structureType = selectedBuilding->getStructureType();
    const BuildingConfig* buildingConfig = configMgr->getBuildingConfig(structureType);

    // Gebäudename
    fontMgr->renderText(renderer, buildingConfig->getName(), windowX + width/2, windowY + 23,
        &colorWhite, &colorBlack, "DroidSans-Bold.ttf", 15, RENDERTEXT_HALIGN_CENTER);

    // produzierte Waren
    const ProductionSlots* productionSlots = &selectedBuilding->productionSlots;
    int productionY = windowY + 58;
    if (productionSlots->input2.isUsed()) {
        // input + input2 -> output

        GuiUtils::drawGoodsBox(windowX + 42, productionY,
            productionSlots->input.goodsType, productionSlots->input.inventory, -1);
        graphicsMgr->getOtherGraphic(OtherGraphic::PRODUCTION_PLUS)->drawAt(windowX + 88, productionY);
        GuiUtils::drawGoodsBox(windowX + 103, productionY,
            productionSlots->input2.goodsType, productionSlots->input2.inventory, -1);
        graphicsMgr->getOtherGraphic(OtherGraphic::PRODUCTION_ARROW)->drawAt(windowX + 149, productionY);
        GuiUtils::drawGoodsBox(windowX + 163, productionY,
            productionSlots->output.goodsType, productionSlots->output.inventory, -1);
    }
    else if (productionSlots->input.isUsed()) {
        // input -> output

        GuiUtils::drawGoodsBox(windowX + 73, productionY,
            productionSlots->input.goodsType, productionSlots->input.inventory, -1);
        graphicsMgr->getOtherGraphic(OtherGraphic::PRODUCTION_ARROW)->drawAt(windowX + 119, productionY);
        GuiUtils::drawGoodsBox(windowX + 133, productionY,
            productionSlots->output.goodsType, productionSlots->output.inventory, -1);
    }
    else if (productionSlots->output.isUsed()) {
        // output

        GuiUtils::drawGoodsBox(windowX + 103, productionY,
            productionSlots->output.goodsType, productionSlots->output.inventory, -1);
    }

    // TODO Auslastung
    // TODO Betriebskosten
}