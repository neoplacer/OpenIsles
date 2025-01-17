#ifndef _GAME_H
#define _GAME_H

#ifdef TESTS_ENABLED
#include <gtest/gtest.h>
#endif

#include <array>
#include <cassert>
#include <map>
#include <vector>
#include "global.h"
#include "game/Colony.h"
#include "game/GameIO.h"
#include "game/Player.h"
#include "map/coords/MapCoords.h"
#include "map/Building.h"
#include "map/Direction.h"
#include "map/Harvestable.h"
#include "map/Isle.h"
#include "map/Map.h"
#include "map/MapObject.h"
#include "map/Ship.h"
#include "map/Street.h"
#include "map/Structure.h"

/**
 * @page gameTicks Zeiteinheiten
 *
 * Game-Ticks
 * ==========
 *
 * Alle Zeit im Spiel wird in `Game-Ticks` angegeben. Dies sind (spielgeschwindigkeit-abhängige) Millisekunden,
 * seitdem das Spiel gestartet wurde. Wenn von "Spielzeit" oder kurz "Ticks" gesprochen wird, so sind immer Game-Ticks
 * gemeint.
 *
 * Das Spiel beginnt bei `ticks = 0`. Danach erhöht sich dieser Wert um Vielfache der Spielgeschwindigkeit der
 * vergangenen Zeit in Millisekunden. Die Aktualisierung läuft wiefolgt ab:
 * - Die Hauptschleife misst die physikalisch vergangene Zeit in Millisekunden.
 * - Sie ruft Game::update() auf und teilt ihr die vergangene Zeit mit.
 * - Game::update() multipliziert die physikalisch vergangene Zeit mit der Spielgeschwindigkeit und erhöht den
 *   internen Zähler Game#ticks
 * - Game-Objekte orientieren sich ausschließlich an den Game-Ticks indem sie Game::getTicks() aufrufen.
 *
 * Beispiele:
 * - `ticks = 5000`.
 *   40 Millisekunden Zeit verstreichen. Spielgeschwindigkeit ist 1x.
 *   Danach ist `ticks = 5040`.
 * - `ticks = 5000`.
 *   30 Millisekunden Zeit verstreichen.
 *   Spielgeschwindigkeit ist 2x.
 *   Danach ist `ticks = 5060`.
 *
 * Zyklus
 * ======
 *
 * Ein `Zyklus` ist das Intervall, in welchem bestimmte Spiellogiken ausgeführt werden:
 * - Finanzen: Steuereinnahmen, Betriebskosten
 * - Nahrungsmittel- und Güterverbrauch, Aktualisierung der Zufriedenheit
 *
 * Ein Zyklus ist fix als 10.000 Game-Ticks angesetzt, d.&nbsp;h. alle 10 Sekunden bei Normalspielgeschwindigkeit.
 */

/**
 * @brief Klasse, die den Zustand des Spiels beinhaltet und die zentrale Anlaufstelle ist, um Änderungen an diesem
 * durchzuführen. Sie benachrichtigt die anderen Klassen (z.B. GUI-Elemente oder die Map), um die nötigen Änderungen
 * an den Datenstrukturen durchzuführen.
 */
class Game : public ContextAware {

    // "Spiel laden" darf direkt auf die Interna zugreifen
    friend class GameIO;

#ifdef TESTS_ENABLED
    FRIEND_TEST(FoodConsumptionTest, checkThatFoodInConsumed);
    FRIEND_TEST(FoodConsumptionTest, checkThatFoodInConsumedOverAllPopulationTiers);
    FRIEND_TEST(InCatchmentAreaFinderInvisibleGoodsTest, checkThatRouteToGrassIsFound);
    FRIEND_TEST(InCatchmentAreaFinderInvisibleGoodsTest, checkThatNoRouteIsFoundWhenAllWasHarvested);
    FRIEND_TEST(TaxesOnlyFinancesTest, checkThatTaxesIncomeWorksWithDifferentPopulationTiers);
#endif

private:
    /**
     * @brief Gibt den Zeitpunkt in Millisekunden an, in welchem sich das Spiel grade befindet.
     * Jedes Spiel beginnt bei 0.
     *
     * @ref gameTicks
     */
    unsigned long ticks;

    /**
     * @brief die Karte. Enthält die Inseln und alles was drauf is
     */
    Map* map;
    
    /**
     * @brief Array aller Spieler
     */
    std::array<Player, Player::MAX_PLAYERS> players;
    
    /**
     * @brief Zeiger auf den Spieler, den dieser Client kontrolliert
     */
    Player* currentPlayer = nullptr;
    
    /**
     * @brief Siedlungen im Spiel. Jeder Spieler kann auf jeder Insel nur maximal eine Siedlung haben.
     */
    std::map<std::pair<const Player*, const Isle*>, Colony*> colonies;

    /**
     * @brief Spielgeschwindigkeit in Vielfachen der Normalgeschwindigkeit
     */
    unsigned char speed;

    /**
     * @brief gibt an, ob die offizielle FPS-Anzeige sichtbar ist
     */
    bool fpsCounterEnabled = false;

    /**
     * @brief [Zeitpunkt](@ref gameTicks), wann das nächste Mal ein Zyklus standfindet.
     */
    unsigned long nextCycleTicks;

public:
    Game(const Context& context);
    ~Game();

    /**
     * @brief Liefert die aktuelle [Spielzeit](@ref gameTicks) zurück.
     * @ref gameTicks
     *
     * @return aktuelle Spielzeit
     */
    unsigned long getTicks() const {
        return ticks;
    }

    Map* getMap() const {
        return map;
    }
    
    /**
     * @brief Fügt einen neuen Spieler hinzu. Sinnvollerweise sollte das nur zu Beginns des Spiels gemacht werden.
     * @param color Spielerfarbe
     * @param type Typ des Spielers
     * @param name Name des Spielers
     * @param coins Münzguthaben
     * @return liefert den Player zurück
     */
    Player* addPlayer(PlayerColor color, const PlayerType& type, const std::string& name, long coins) {
        assert (type != PlayerType::NONE);

        Player* player = &players[int(color)];

        player->initPlayer(color, type, name, coins);
        return player;
    }
    
    /**
     * @brief Liefert einen bestimmten Spieler zurück
     * @param playerIndex Index des Spielers (im Bereich 0 bis Player::MAX_PLAYERS-1)
     * @return Spieler (oder `nullptr`, wenn ein ungültiger Index übergeben wurde)
     */
    Player* getPlayer(int playerIndex) {
        if ((playerIndex >= 0) && (playerIndex < Player::MAX_PLAYERS)) {
            return &players[playerIndex];
        }

        return nullptr;
    }

    /**
     * @brief Setzt den aktuellen Spieler. Das muss man wohl nur ein einziges Mal machen.
     * @param currentPlayer Spieler, der als aktueller Spieler gesetzt werden soll.
     */
    void setCurrentPlayer(Player* currentPlayer) {
        assert(currentPlayer->isHuman());
        this->currentPlayer = currentPlayer;
    }
    
    /**
     * @brief Liefert den aktueller Spieler zurück
     * @return aktueller Spieler
     */
    Player* getCurrentPlayer() const {
        return currentPlayer;
    }
    
    /**
     * @brief Gründet eine neue Siedlung
     *
     * @param player Spieler, der die Siedlung gründet
     * @param colonyName Name, den die Siedlung erhalten soll
     * @param isle Insel, auf der die Siedlung gegründet wird
     * @return Zeiger auf die neu angelegte Siedlung
     */
    Colony* foundNewColony(const Player* player, const std::string& colonyName, const Isle* isle);
    
    /**
     * @brief Liefert die Siedlung eines Spielers auf einer Insel zurück
     *
     * @param player Spieler
     * @param isle Insel
     * @return Colony oder `nullptr`, wenn keine Siedlung da ist
     */
    Colony* getColony(const Player* player, const Isle* isle);

    /**
     * @brief Liefert die Siedlung an einer bestimmten Kachel zurück
     *
     * @param mapObject MapObject
     * @return Colony oder `nullptr`, wenn keine Siedlung da ist
     */
    Colony* getColony(const MapCoords& mapCoords);

    /**
     * @brief Liefert die Map mit allen Kolonien zurück
     * @return alle Kolonien als Map, die Spieler-Insel-Paar auf Kolonie abbildet
     */
    const std::map<std::pair<const Player*, const Isle*>, Colony*>& getColonies() const {
        return colonies;
    };

    /**
     * @brief Ändert die Spielgeschwindigkeit
     * @param speed neue Spielgeschwindigkeit
     */
    void setSpeed(unsigned char speed) {
        this->speed = speed;
    }

    /**
     * @brief Liefert zurück, ob die offizielle FPS-Anzeige aktiviert ist
     * @return `true`, wenn die offizielle FPS-Anzeige aktiviert ist
     */
    bool isFpsCounterEnabled() const {
        return fpsCounterEnabled;
    }

    /**
     * @brief Schaltet die Anzeige der offiziellen FPS-Anzeige um
     */
    void toggleFpsCounter() {
        this->fpsCounterEnabled = !this->fpsCounterEnabled;
    }

    /**
     * @brief Fügt eine neue Landschaftkachel, die abgeerntet werden kann, der Karte hinzu.
     * Das Startalter wird zufällig minimal bestimmt.
     *
     * @param mapCoords Map-Koordinaten der Kachel
     * @param mapObjectType Typ der Landschaftkachel
     * @param view Ansicht, mit der die Kachel positioniert werden soll
     * @return Zeiger auf das neu angelegte Harvestable-Objekt
     */
    Harvestable* addHarvestable(
        const MapCoords& mapCoords, const MapObjectType* mapObjectType,const FourthDirection& view);

    /**
     * @brief Fügt eine neue Struktur der Karte hinzu.
     * @param mapCoords Map-Koordinaten des Objekts
     * @param mapObjectType Typ des Objekt
     * @param view Ansicht, mit der das Objekt positioniert werden soll
     * @param player Spieler, dem das Objekt gehören soll
     * @return Zeiger auf die neu angelegte Struktur
     */
    Structure* addStructure(
        const MapCoords& mapCoords, const MapObjectType* mapObjectType, const FourthDirection& view, Player* player);

    /**
     * @brief Fügt eine neue Straße der Karte hinzu.
     * @param mapCoords Map-Koordinaten des Objekts
     * @param mapObjectType Typ des Objekt
     * @param view Ansicht, mit der das Objekt positioniert werden soll
     * @param player Spieler, dem das Objekt gehören soll
     * @param streetConnections Bitmaske, von welcher Seite andere Straßen angrenzen
     * @return Zeiger auf die neu angelegte Straße
     */
    Street* addStreet(
        const MapCoords& mapCoords, const MapObjectType* mapObjectType, const FourthDirection& view,
        Player* player, const StreetConnections& streetConnections);

    /**
     * @brief Fügt ein neues Schiff der Karte hinzu
     * @param mapCoords Map-Koordinaten des Objekts
     * @param mapObjectType Typ des Objekt
     * @param direction Richtung, in die das Schiff zeigt
     * @param player Spieler, dem das Schiff gehören soll
     * @return Zeiger auf das neu angelegte Schiff
     */
    Ship* addShip(
        const DoubleMapCoords& mapCoords, const MapObjectType* mapObjectType,
        const EighthDirection& direction, Player* player);

    /**
     * @brief Entfernt ein Map-Objekt und führt alle notwendigen Logiken hierzu aus (z.&nbsp;B. Träger entfernen,
     * Bevölkerung aktualisieren und vieles mehr).
     *
     * @param mapObject zu löschendes Map-Objekt
     */
    void deleteMapObject(MapObject* mapObject);

    /**
     * @brief Schaltet das aktuell gewählte MapObject um.
     * Diese Methode sagt dem GuiMgr Bescheid, damit das Panel rechts angepasst wird.
     *
     * @param selectedMapObject Das neue MapObject, was gewählt sein soll oder `nullptr`, um abzuwählen
     */
    void setSelectedMapObject(const MapObject* selectedMapObject);

    /**
     * @brief Lädt einen Spielstand von einer tiled-TMX-Datei
     * @param filename Dateiname mit den zu ladenden Spielstand-Daten
     */
    void loadGameFromTMX(char const* filename);

    /**
     * @brief Hauptschleife des Spiels. Sie führt die Spiellogik aus, indem sie
     * - alle Map-Objekte aktualisiert
     * - bestimmte Spiellogiken (z.&nbsp;B. Finanzen) einmal pro [Zyklus](@ref gameTicks) ausführt
     *
     * Wichtig: Die auszuführende Spielzeit sollte nicht utopisch groß sein, da die Objekte alle nur einmalig
     * aktualisiert werden. Wenn man also eine Spielzeit von einer ganzen Minute angibt, so hat das Spiel danach
     * einen anderen Zustand, als hätte man 60x eine Sekunde oder 600x 100 Millisekunden angegeben.
     * Das liegt daran, dass die Objekte nicht ewig in die Zukunft aktualisieren, sondern ihre Arbeit in kleinen
     * Häppchen erledigen, z.&nbsp;B. wird ein Träger nur ein Stückchen weiterlaufen und Waren ein-/ausladen, aber
     * nicht mehrere Aufträge hintereinander ausführen.
     *
     * Die auszuführende Spielzeit sollte sich immer im Millisekunden-Bereich abspielen.
     * (Tests rufen diese Methode mehrfach hintereinander in einer Schleife auf, um einen großen Zeitfluss zu simulieren)
     *
     * @param millisecondsElapsed Millisekunden, die das Spiel fortschreiten soll
     *                            (= Zeit, die seit dem letzten Aufruf vergangen ist)
     */
    void update(unsigned long millisecondsElapsed);

    /**
     * @brief Hilfsmethode für Tests/Automatismen, die große Zeitspannen Spiellogik auslösen
     * und damit "realistischer" durchführt.
     *
     * Diese Methode löst das Problem, dass update() nicht mit zu großem Wert für `millisecondsElapsed`
     * aufgerufen werden darf. Lösung hier ist, dass die große Zeitspanne durch mehrfache kleine update()-Aufrufe
     * überbrückt wird.
     * Die Aufrufe verwenden maximal `millisecondsIncrement` als `millisecondsElapsed`-Parameter.
     *
     * Beispiel: Ein Aufruf von `update(60500, 1000)` sorgt dafür, dass
     * - 60x `update(1000)` und
     * - zum Schluss `update(500)` aufgerufen wird.
     *
     * @param millisecondsElapsedSum Millisekunden, die das Spiel fortschreiten soll
     * @param millisecondsIncrement größtes Zeitintervall in Millisekunden,
     *                              das für den `update()`-Aufruf verwendet werden soll
     */
    void update(unsigned long millisecondsElapsedSum, unsigned long millisecondsIncrement) {
        unsigned long millisecondsToDoRemaining = millisecondsElapsedSum;

        while (millisecondsToDoRemaining > 0) {
            unsigned long nextMillisecondsHopp =
                (millisecondsToDoRemaining > millisecondsIncrement) ? millisecondsIncrement : millisecondsToDoRemaining;

            update(nextMillisecondsHopp);

            millisecondsToDoRemaining -= nextMillisecondsHopp;
        }
    }

    /**
     * @brief Shortcut für einheitlichen Aufruf von update(unsigned long, unsigned long)
     * @param millisecondsElapsed Millisekunden, die das Spiel fortschreiten soll.
     */
    inline void updateRealistically(unsigned long millisecondsElapsed) {
        constexpr unsigned int fps = 50;
        constexpr unsigned long millisecondsIncrement = 1000 / fps;

        update(millisecondsElapsed, millisecondsIncrement);
    }

    /**
     * @brief Ändert die Einwohner innerhalb eines Gebäudes. Positive Werte fügen Bewohner hinzu, negative nehmen
     * welche weg.
     *
     * Diese Methode aktualisiert gleich die Einwohnerzahl der Kolonie. Es sollte immer diese Methode benutzt werden,
     * um Einwohner zu ändern.
     *
     * @param building Gebäude, dessen Einwohner geändert werden sollen
     * @param amount Bevölkerung, die hinzukommen soll
     */
    void addInhabitantsToBuilding(Building* building, char amount);

private:
    /**
     * @brief Helper, der ein neues Map-Objekt instanziiert und dabei die `lastUpdateTicks` auf die aktuelle
     * Spielzeit setzt.
     *
     * @param mapObjectType Typ von Map-Objekt, der angelegt werden soll.
     * @return instanziiertes Map-Objekt
     */
    MapObject* instantiateNewMapObject(const MapObjectType* mapObjectType) const;
};

#endif
