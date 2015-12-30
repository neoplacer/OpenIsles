#ifndef _ECONOMICS_MGR_H
#define _ECONOMICS_MGR_H

#include "Context.h"
#include "map/Building.h"


/**
 * @brief Manager, der die Wirtschaft übernimmt
 */
class EconomicsMgr : public ContextAware {

public:
    EconomicsMgr(const Context* const context);
    ~EconomicsMgr();

    /**
     * @brief Führt die Produktion für ein Gebäude durch.
     *
     * Wenn alle Eingabewaren da sind und das Ausgabewarenlager nicht voll ist, UND es an der Zeit ist,
     * etwas zu produzieren, wird eine Produktion durchgeführt.
     *
     * @param building Gebäude, für welches die Produktion durchgeführt werden soll
     */
    void updateProduction(Building* building);

    /**
     * @brief Aktualisiert die PlayerStatus-Strukturen
     *
     * Diese Methode muss immer dann aufgerufen werden, wenn sich was an Einwohnern oder Finanzen ändert.
     */
    void updatePlayerStatus();

    /**
     * @brief Aktualisiert die Finanzen.
     * Es werden die Zahlen aus den PlayerStatus-Strukturen verwendet.
     *
     * Diese Methode wird pro [Zyklus](@ref gameTicks) von Game::update() aufgerufen
     */
    void updateFinances();
};

#endif
