#include "graphics/graphic/IGraphic.h"
#include "graphics/mgr/IGraphicsMgr.h"
#include "map/Directions.h"
#include "map/coords/MapCoords.h"
#include "map/coords/ScreenCoords.h"
#include "utils/Rect.h"

#ifndef _MAP_COORD_UTILS_H
#define _MAP_COORD_UTILS_H

class Building;
class Map;

/**
 * @brief Klasse mit statischen Helperklassen
 */
class MapCoordUtils {

public:
    /**
	 * @brief Rechnet Map- in Screen-Koordinaten um. Die Screen-Koordinaten sind die der oberen linken Ecke der Kachel.
	 * @param mapCoords Map-Koordinaten
     * @param screenView Ansicht, die für die Umrechnung benutzt werden soll
     * @param map Karte für die Umrechnung (wird für Kartengröße benötigt)
	 * @return Screen-Koordinaten
	 */
    static ScreenCoords mapToScreenCoords(
        const MapCoords& mapCoords, const FourDirectionsView& screenView, const Map& map);

    /**
	 * @brief Rechnet Map- in Screen-Koordinaten um. Die Screen-Koordinaten sind die der oberen linken Ecke der Kachel.
	 * @param mapCoords Map-Koordinaten
     * @param screenView Ansicht, die für die Umrechnung benutzt werden soll
     * @param map Karte für die Umrechnung (wird für Kartengröße benötigt)
	 * @return Screen-Koordinaten
	 */
    static ScreenCoords mapToScreenCoords(
        const DoubleMapCoords& mapCoords, const FourDirectionsView& screenView, const Map& map);

    /**
	 * @brief Rechnet Screen- in Map-Koordinaten um.
	 * @param screenCoords Screen-Koordinaten
     * @param screenView Ansicht, die für die Umrechnung benutzt werden soll
     * @param map Karte für die Umrechnung (wird für Kartengröße benötigt)
	 * @return Map-Koordinaten
	 */
    static MapCoords screenToMapCoords(
        const ScreenCoords& screenCoords, const FourDirectionsView& screenView, const Map& map);

    /**
     * @brief Berechnet ausgehend von Map-Koordinaten und einer Grafik das SDL-Rechteck, an welche Stelle
     * die Grafik gesetzt werden muss.
     *
     * @param mapCoords Map-Koordinaten
     * @param map (Dependency)
     * @param elevation Elevation, auf die die Grafik gezeichnet werden soll
     * @param graphic zu zeichnende MapObject-Grafik
     * @return Rechteck mit den Draw-Koordinaten, wo die Grafik hingezeichnet werden muss
     */
    static Rect mapToDrawCoords(
        const DoubleMapCoords& mapCoords, const Map& map, int elevation, const IGraphic& graphic);

    /**
     * @brief Berechnet ausgehend von Map-Koordinaten und einer Grafik das SDL-Rechteck, an welche Stelle
     * die Grafik gesetzt werden muss.
     * Diese Methode ist von der Funktion analog
     * mapToDrawCoords(const DoubleMapCoords&, const Map&, int, const IGraphic&), rechnet aber mit double-Eingabewerten.
     *
     * @param mapCoords Map-Koordinaten
     * @param map (Dependency)
     * @param elevation Elevation, auf die die Grafik gezeichnet werden soll
     * @param graphic zu zeichnende MapObject-Grafik
     * @return Rechteck mit den Draw-Koordinaten, wo die Grafik hingezeichnet werden muss
     */
    static Rect mapToDrawCoords(const MapCoords& mapCoords, const Map& map, int elevation, const IGraphic& graphic);

    /**
     * @brief Berechnet für ein bestimmtes Gebäude das SDL-Rechteck, an welche Stelle die Grafik gesetzt werden muss.
     * @param map (Dependency)
     * @param graphicsMgr (Dependency)
     * @param building Gebäude
     * @return Rechteck mit den Draw-Koordinaten, wo die Grafik hingezeichnet werden muss
     */
    static Rect getDrawCoordsForBuilding(const Map& map, IGraphicsMgr* graphicsMgr, const Building* building);

    /**
     * @brief Ermittelt für die aktuelle Position des Mauszeigers, welche Screen-Koordinate dort liegt.
     *
     * @param map (Dependency)
     * @param mouseCurrentX X-Koordinate des Mauszeigers
     * @param mouseCurrentY Y-Koordinate des Mauszeigers
     * @return Screen-Koordinaten
     */
    static ScreenCoords getScreenCoordsUnderMouse(const Map& map, int mouseCurrentX, int mouseCurrentY);

    /**
     * @brief Ermittelt für die aktuelle Position des Mauszeigers, welche Map-Koordinate dort liegt.
     * Es wird immer die elevatete Position genommen, selbst, wenn der Mauszeiger auf dem Wasser steht.
     *
     * @param map (Dependency)
     * @param mouseCurrentX X-Koordinate des Mauszeigers
     * @param mouseCurrentY Y-Koordinate des Mauszeigers
     * @return Map-Koordinaten
     */
    static MapCoords getMapCoordsUnderMouse(const Map& map, int mouseCurrentX, int mouseCurrentY);

    /**
     * @brief Ermittelt, welche Map-Koordinaten sind in den Ecken des Bildschirms befinden.
     *
     * @param map (Dependency)
     * @param mapCoordsTopLeft Map-Koordinate in der oberen linken Ecke (Ausgabe)
     * @param mapCoordsTopRight Map-Koordinate in der oberen rechen Ecke (Ausgabe)
     * @param mapCoordsBottomLeft Map-Koordinate in der unteren linken Ecke (Ausgabe)
     * @param mapCoordsBottomRight Map-Koordinate in der unteren rechen Ecke (Ausgabe)
     * @return Map-Koordinaten
     */
    static void getMapCoordsInScreenEdges(const Map& map,
        MapCoords& mapCoordsTopLeft, MapCoords& mapCoordsTopRight,
        MapCoords& mapCoordsBottomLeft, MapCoords& mapCoordsBottomRight);

private:
    /**
     * @brief Rechnet Screen-Koordinaten, wo ein Gebäude gezeichnet werden soll, in die entsprechenden
     * Draw-Koordinaten um.
     *
     * @param screenCoords Screen-Koordinaten
     * @param map (Dependency)
     * @param elevation Elevation, auf die die Grafik gezeichnet werden soll
     * @param graphic zu zeichnende MapObject-Grafik
     * @return Rechteck mit den Draw-Koordinaten, wo die Grafik hingezeichnet werden muss (Ausgabe)
     */
    static Rect screenToDrawCoords(
        const ScreenCoords& screenCoords, const Map& map, int elevation, const IGraphic& graphic);

};

#endif

