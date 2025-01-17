#include "gui/components/GuiGoodElement.h"
#include "graphics/mgr/IFontMgr.h"
#include "graphics/mgr/IGraphicsMgr.h"
#include "utils/Color.h"
#include "utils/Rect.h"
#include "utils/StringFormat.h"


GuiGoodElement::GuiGoodElement(const Context& context) : GuiStaticGraphicElement(context) {
    // Größe ist fix
    width = 42;
    height = 42;
}

GuiGoodElement::~GuiGoodElement() {
}

void GuiGoodElement::setGood(const Good* good) {
    this->good = good;

    // Grafik aktualisieren
    const std::string graphicSetName = context.graphicsMgr->getGraphicSetNameForGoodIcons(good, true);
    setGraphic(context.graphicsMgr->getGraphicSet(graphicSetName)->getStatic()->getGraphic());
}

void GuiGoodElement::renderElement(IRenderer* renderer) {
    GuiStaticGraphicElement::renderElement(renderer);

    if (good == nullptr) {
        return; // nix weiter zu tun, wenn das Element leer ist
    }

    int windowX, windowY;
    getWindowCoords(windowX, windowY);

    // Wert anzeigen
    if (displayValue) {
        std::string inventoryOutput = string_sprintf("%ut", value);
        context.fontMgr->renderText(renderer, inventoryOutput, windowX + 40, windowY + 42, &Color::white, &Color::black,
            "DroidSans.ttf", 12, RENDERTEXT_HALIGN_RIGHT | RENDERTEXT_VALIGN_BOTTOM);
    }

    // Balken anzeigen
    else if (displayBar) {
        int barHeight = (int) (barValue * (double) height);

        renderer->setDrawColor(Color::red);

        const int barWidth = 4;
        Rect rect(windowX + width - barWidth, windowY + (height - barHeight), barWidth, barHeight);
        renderer->fillRect(rect);
    }
}
