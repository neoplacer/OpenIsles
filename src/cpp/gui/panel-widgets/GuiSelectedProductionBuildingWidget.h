#ifndef _GUI_SELECTED_PRODUCTION_BUILDING_WIDGET_H
#define _GUI_SELECTED_PRODUCTION_BUILDING_WIDGET_H

#include <SDL.h>
#include <string>
#include "global.h"
#include "gui/components/GuiProductionSlotsElement.h"
#include "gui/components/GuiStaticGraphicElement.h"
#include "gui/components/GuiStaticTextElement.h"
#include "gui/panel-widgets/GuiSelectedBuildingWidget.h"


/**
 * @brief GUI-Widget mit den Informationen des aktuell ausgewählten Produktionsgebäudes.
 */
class GuiSelectedProductionBuildingWidget : public GuiSelectedBuildingWidget {

private:
    GuiStaticTextElement buildingName;                   ///< Gebäudename
    GuiProductionSlotsElement guiProductionSlotsElement; ///< GUI-Element für produzierte Güter

    // childElements der guiProductionSlotsElement
    GuiGoodsSlotElement* goodsSlotInput;    ///< Gütersymbol: verbraute Güter
    GuiGoodsSlotElement* goodsSlotInput2;   ///< Gütersymbol: weitere verbraute Güter
    GuiGoodsSlotElement* goodsSlotOutput;   ///< Gütersymbol: produzierte Güter

    GuiStaticTextElement operatingCostsLabel;    ///< Betriebskosten-Label
    GuiStaticTextElement operatingCosts;         ///< variables Textfeld mit die Betriebskosten
    GuiStaticGraphicElement operatingCostsIcon;  ///< Symbol für Betriebskosten

public:
    GuiSelectedProductionBuildingWidget(const Context& context);
    virtual ~GuiSelectedProductionBuildingWidget() override {}

    /**
     * @brief Zeichnet das Element (ohne Kinder)
     */
    virtual void renderElement(IRenderer* renderer);

    /**
     * @brief Wird aufgerufen, wenn sich das markierte Map-Objekt ändert.
     * Das Widget passt darauf hin ggf. die Komponenten drauf an.
     *
     * @param newSelectedBuilding neues Gebäude, was nun selektiert ist
     */
    virtual void onSelectedMapBuildingChanged(const Building* newSelectedBuilding);

private:
    /**
     * @brief Aktualisiert den Statusleisten-Text für ein Input-GuiGoodsSlotElement.
     * Es wird der Text "...vorrat (reicht für weitere ...t ...)" gesetzt.
     *
     * @param goodsSlotElement GUI-Element, dessen Statusbar-Text geändert wird
     * @param inputAmountForProduction Info aus der Gebäude-Konfiguration,
     *                                 wie viele Güter pro 1t Output verbraucht werden
     * @param inputInventory Info aus dem konkreten Gebäude, wie viele Güter zur Verfügung stehen
     * @param inputLabel Info aus der Gebäude-Konfiguration, wie das Input-Gut heißt (msgid)
     * @param outputLabel Info aus der Gebäude-Konfiguration, wie das Output-Gut heißt (msgid)
     */
    void updateInputSlotStatusBarText(
        GuiGoodsSlotElement* goodsSlotElement, double inputAmountForProduction, unsigned int inputInventory,
        const std::string& inputLabel, const std::string& outputLabel);
};

#endif
