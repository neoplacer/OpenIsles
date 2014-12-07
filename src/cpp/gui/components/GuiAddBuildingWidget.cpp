#include <SDL.h>
#include "config/ConfigMgr.h"
#include "game/Game.h"
#include "gui/FontMgr.h"
#include "gui/GuiMgr.h"
#include "gui/components/GuiAddBuildingWidget.h"
#include "utils/StringFormat.h"


static SDL_Color colorLightBrown = {223, 216, 183, 255};
static SDL_Color colorBlack = {0, 0, 0, 255};
static SDL_Color colorWhite = {255, 255, 255, 255};



GuiAddBuildingWidget::GuiAddBuildingWidget(const Context* const context) : GuiBase(context) {
    setCoords(13, 8, 210, 320);

    // TODO Child-Buttons zum Drehen hinzufügen
}

void GuiAddBuildingWidget::renderElement(SDL_Renderer* renderer) {
    int windowX, windowY;
    getWindowCoords(windowX, windowY);

    StructureType structureType = context->guiMgr->getPanelState().addingStructure;
    const BuildingConfig* buildingConfig = context->configMgr->getBuildingConfig(structureType);

    // Gebäudename
    context->fontMgr->renderText(renderer, buildingConfig->getName(), windowX + width/2, windowY + 15,
        &colorLightBrown, &colorBlack, "DroidSans-Bold.ttf", 15, RENDERTEXT_HALIGN_CENTER);

    // Gebäude-Grafik
    MapObjectGraphic* buildingGraphic = context->graphicsMgr->getGraphicForStructure(structureType);

    double scale; // ggf. verkleinert zeichnen, wenn das Gebäude zu groß is
    if (buildingGraphic->getWidth() <= 160) {
        scale = 1;
    } else if (buildingGraphic->getWidth() <= 240) {
        scale = 0.666666666;
    } else {
        scale = 0.5;
    }

    int x = windowX + 110 - (int)(buildingGraphic->getWidth() * scale / 2);
    int y = windowY + 40;
    buildingGraphic->drawScaledAt(x, y, scale);

    // produzierte Waren
    const ProductionSlots* buildingProduction = buildingConfig->getBuildingProduction();
    int productionY = windowY + 160;
    if (buildingProduction->input2.isUsed()) {
        // input + input2 -> output

        context->guiMgr->drawGoodsBox(windowX + 29, productionY, buildingProduction->input.goodsType, -1, -1);
        context->graphicsMgr->getOtherGraphic(OtherGraphic::PRODUCTION_PLUS)->drawAt(windowX + 75, productionY);
        context->guiMgr->drawGoodsBox(windowX + 90, productionY, buildingProduction->input2.goodsType, -1, -1);
        context->graphicsMgr->getOtherGraphic(OtherGraphic::PRODUCTION_ARROW)->drawAt(windowX + 136, productionY);
        context->guiMgr->drawGoodsBox(windowX + 150, productionY, buildingProduction->output.goodsType, -1, -1);
    }
    else if (buildingProduction->input.isUsed()) {
        // input -> output

        context->guiMgr->drawGoodsBox(windowX + 60, productionY, buildingProduction->input.goodsType, -1, -1);
        context->graphicsMgr->getOtherGraphic(OtherGraphic::PRODUCTION_ARROW)->drawAt(windowX + 106, productionY);
        context->guiMgr->drawGoodsBox(windowX + 120, productionY, buildingProduction->output.goodsType, -1, -1);
    }
    else if (buildingProduction->output.isUsed()) {
        // output

        context->guiMgr->drawGoodsBox(windowX + 90, productionY, buildingProduction->output.goodsType, -1, -1);
    }

    // Baukosten
    int costsY = productionY + 60;

    context->graphicsMgr->getOtherGraphic(OtherGraphic::COINS)->drawAt(windowX, costsY);
    context->fontMgr->renderText(renderer, toString(buildingConfig->getBuildingCosts()->coins), windowX + 35, costsY + 12,
        &colorWhite, &colorBlack, "DroidSans-Bold.ttf", 14, RENDERTEXT_HALIGN_LEFT | RENDERTEXT_VALIGN_MIDDLE);

    costsY += 30;

    context->graphicsMgr->getGraphicForGoodsIcon(GoodsType::TOOLS)->drawAt(windowX, costsY);
    context->fontMgr->renderText(renderer, toString(buildingConfig->getBuildingCosts()->tools), windowX + 35, costsY + 16,
        &colorWhite, &colorBlack, "DroidSans-Bold.ttf", 14, RENDERTEXT_HALIGN_LEFT | RENDERTEXT_VALIGN_MIDDLE);

    context->graphicsMgr->getGraphicForGoodsIcon(GoodsType::WOOD)->drawAt(windowX + 60, costsY);
    context->fontMgr->renderText(renderer, toString(buildingConfig->getBuildingCosts()->wood), windowX + 95, costsY + 16,
        &colorWhite, &colorBlack, "DroidSans-Bold.ttf", 14, RENDERTEXT_HALIGN_LEFT | RENDERTEXT_VALIGN_MIDDLE);

    context->graphicsMgr->getGraphicForGoodsIcon(GoodsType::BRICKS)->drawAt(windowX + 120, costsY);
    context->fontMgr->renderText(renderer, toString(buildingConfig->getBuildingCosts()->bricks), windowX + 155, costsY + 16,
        &colorWhite, &colorBlack, "DroidSans-Bold.ttf", 14, RENDERTEXT_HALIGN_LEFT | RENDERTEXT_VALIGN_MIDDLE);

}
