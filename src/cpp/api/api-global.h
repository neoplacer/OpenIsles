#ifndef _API_GLOBAL_H
#define _API_GLOBAL_H

#include <lua.hpp>
#include "global.h"


/**
 * @brief Makro, um eine Funktion in Lua zur Verfügung zu stellen.
 * Das Makro generiert den Funktionskopf für die Funktion
 * @param name Name, unter dem die Funktion in Lua zur Verfügung stehen soll
 */
#define DEFINE_LUA_FUNCTION(name) int LUA_ ## name(lua_State* lua)



// TODO Doku generieren
// @see http://stackoverflow.com/questions/19711787/generating-api-documentation-that-doesnt-match-source-code-exactly

// game.cpp ///////////////////////////////////////////////////////////////////////////////////////

/**
 * @lua
 * @brief Liefert die aktuelle [Spielzeit](@ref gameTicks) zurück.
 * @ref gameTicks
 * @return aktuelle Spielzeit
 */
DEFINE_LUA_FUNCTION(getGameTicks);

/**
 * @lua
 * @brief Liefert die Karte zurück.
 * @return Table mit `width`, `height` und Elementen `x,y` pro Kachel
 */
DEFINE_LUA_FUNCTION(getMap);

/**
 * @lua
 * @brief Liefert allen Inseln zurück.
 * @return Array mit Eigenschaften `width`, `height`, `x` und `y`
 */
DEFINE_LUA_FUNCTION(getIsles);

/**
 * @lua
 * @brief Liefert alle Schiffe zurück.
 * @return Array mit Eigenschaften `x`, `y`, `player` und `name`
 */
DEFINE_LUA_FUNCTION(getShips);

/**
 * @lua
 * @brief Liefert alle Map-Objekte (das sind alle Strukturen, Harvestables und Gebäude; nicht: Träger und Schiffe) zurück.
 * @return Array mit Eigenschaften `x`, `y`, `player` und `type`
 */
DEFINE_LUA_FUNCTION(getObjects);

/**
 * @lua
 * @brief Baut etwas
 * @param playerIndex [Integer] Index des Spielers, der bauen möchte
 * @param mapObjectTypeName [String] Name des Map-Objekts
 * @param mapX [Integer] Map-X-Koordinate, an die gebaut werden soll
 * @param mapY [Integer] Map-Y-Koordinate, an die gebaut werden soll
 * @param view [String] Ausrichtung des Objekts
 */
DEFINE_LUA_FUNCTION(build);

/**
 * @lua
 * @brief Reißt etwas ab
 * @param playerIndex [Integer] Index des Spielers, der abreißen möchte
 * @param mapX [Integer] Map-X-Koordinate, an der abgerissen werden soll
 * @param mapY [Integer] Map-Y-Koordinate, an der abgerissen werden soll
 */
DEFINE_LUA_FUNCTION(demolish);


// misc.cpp ///////////////////////////////////////////////////////////////////////////////////////

/**
 * @lua
 * @brief Loggt eine Zeichenkette im Level "Debug"
 */
DEFINE_LUA_FUNCTION(debug);


// player.cpp /////////////////////////////////////////////////////////////////////////////////////

/**
 * @lua
 * @brief Liefert die Anzahl der Spieler zurück
 * @return maximale Anzahl von Spieler
 */
DEFINE_LUA_FUNCTION(getPlayerCount);

/**
 * @lua
 * @brief Liefert Informationen zu einem Spieler zurück
 * @param playerIndex [Integer] Index des Spielers i(m Bereich 1 bis getPlayerCount())
 * @return Record mit Informationen zum Spieler
 */
DEFINE_LUA_FUNCTION(getPlayer);


#endif
