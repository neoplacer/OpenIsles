#include "gui/FontMgr.h"
#include "gui/panel-widgets/GuiDummyWidget.h"


static SDL_Color colorRed = {255, 0, 0, 255};
static SDL_Color colorYellow = {255, 255, 0, 255};


GuiDummyWidget::GuiDummyWidget(const Context* const context) : GuiPanelWidget(context) {
}

void GuiDummyWidget::renderElement(SDL_Renderer* renderer) {
    int windowX, windowY;
    getWindowCoords(windowX, windowY);

    context->fontMgr->renderText(renderer, "Dummy-Widget", windowX + width/2, windowY + height/2,
        &colorYellow, &colorRed, "DroidSans.ttf", 15, RENDERTEXT_HALIGN_CENTER | RENDERTEXT_VALIGN_MIDDLE);
}
