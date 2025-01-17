#ifndef _GUI_MGR_H
#define _GUI_MGR_H

#ifndef NO_SDL
#include <SDL.h>
#endif

#include <map>
#include "global.h"
#include "game/GoodsSlot.h"
#include "map/coords/MapCoords.h"
#include "map/coords/WindowCoords.h"
#include "map/Direction.h"
#include "map/Map.h"
#include "map/MapObjectType.h"
#include "graphics/renderer/IRenderer.h"

class GuiBase;
class GuiButton;
class GuiPushButton;
class GuiPanelWidget;
class GuiStaticGraphicElement;
class MapObject;
class SDL_Renderer;

enum BuildingGroup : unsigned char;


/**
 * Enum für 4 Buttons, um die Panels umzuschalten
 */
typedef
enum PanelButton : unsigned char {

    /**
     * @brief Baumenü
     */
    BUILD = 0,

    /**
     * @brief Kampfmenü
     */
    MILITARY = 1,

    /**
     * @brief Info-Panel
     */
    PLAYER_STATUS = 2,

    /**
     * @brief Einstellungs-Panel
     */
    OPTIONS = 3

} PanelButton;


/**
 * @brief Struktur für den Zustand des Panels auf der rechten Seite
 */
typedef
struct PanelState {
    // 4 Buttons oben /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief ausgewählter Button (4 Buttons, die die Panels umschalten)
     */
    PanelButton selectedPanelButton;

    // Panel Baumenü //////////////////////////////////////////////////////////////////////////////

    /**
     * @brief zuletzt/aktuell gewählte Kategorie im Baumenü
     */
    BuildingGroup selectedBuildingGroup;

    /**
     * @brief zuletzt/aktuell gewähltes Map-Objekt im Baumenü
     * `nullptr` im Abreißmodus
     */
    const MapObjectType* addingMapObject;

    /**
     * @brief zuletzt/aktuell gewählte Ausrichtung des Map-Objekts im Baumenü
     */
    FourthDirection addingMapObjectView;

    /**
     * @brief Flag, was angibt, ob das Popup-Menü mit den einzelnen Gebäuden aufgeklappt ist
     */
    bool buildingMenuOpen;

    // gezeigtes Panel ////////////////////////////////////////////////////////////////////////////

    /**
     * @brief Aktives GUI-Panel-Widget, was gerade gezeigt wird. Es kann immer nur maximal ein solches Widget
     * sichtbar sein. Ist gar nix gezeigt, so kann hier `nullptr` gesetzt werden.
     */
    GuiPanelWidget* activeGuiPanelWidget;

} PanelState;


/**
 * GUI-Manager. Verwaltet alle GUI-Elemente.
 */
class GuiMgr : public ContextAware {

    // Die Widgets dürfen den inneren Zustand des GuiMgrs manipulieren
    friend class GuiBuildMenuWidget;
    friend class GuiOptionsMenuWidget;

private:
    // Dependencies
    IRenderer* const renderer;

    /**
     * @brief Fenster-Koordinaten, an dem der letzte Left-Click gestartet wurde.
     * Gültig, bis die Maustaste losgelassen wurde.
     */
    WindowCoords startClickWindowCoords;

    /**
     * @brief Map-Koordinaten, an dem der letzte Left-Click gestartet wurde.
     * Gültig, bis die Maustaste losgelassen wurde.
     */
    MapCoords startClickMapCoords;

    /**
     * @brief Map-Koordinaten, an denen sich der Mauszeiger grade befindet.
     * Wir aktualisieren diese Koordinaten bei jedem `SDL_MOUSEMOTION`-Event.
     */
    MapCoords mapCoordsUnderMouse;
    
    /**
     * @brief Map, die alle registrierten Gui-Elemente enthält und durch ihre Identifier referenziert.
     */
    std::map<int, GuiBase*> identifierMap;

    /**
     * @brief Zustand des Panels auf der rechten Seite
     */
    PanelState panelState;

    /**
     * Flag, was gesetzt wird, wenn die Anwendung beendet werden soll.
     */
    bool quitGame;

public:
    GuiMgr(const Context& context, IRenderer* renderer);
    ~GuiMgr();

    /**
     * @brief Legt alle GUI-Elemente an. Diese Methode muss direkt nach dem Konstrutor aufgerufen werden.
     * Der Konstruktor selber kann dies nicht erledigt, da die erzeugten GUI-Komponenten auf den guiMgr zugreifen.
     * D.h. der muss zu diesem Zeitpunkt bereits gesetzt sein.
     */
    void initGui();

    /**
     * @brief Liefert die Fenster-Koordinaten zurück, an denen die linke Maustaste gedrückt wurde.
     * Diese Methode liefert nur einen gültigen Wert, wenn die linke Maustaste gerade gedrückt ist.
     *
     * @return Fenster-Koordinaten, an denen die linke Maustaste gedrückt wurde.
     */
    const WindowCoords& getStartClickWindowCoords() const {
        return startClickWindowCoords;
    }

    /**
     * @brief Liefert die Map-Koordinaten zurück, an denen die linke Maustaste gedrückt wurde.
     * Diese Methode liefert nur einen gültigen Wert, wenn die linke Maustaste gerade gedrückt ist.
     *
     * @return Map-Koordinaten, an denen die linke Maustaste gedrückt wurde.
     */
    const MapCoords& getStartClickMapCoords() const {
        return startClickMapCoords;
    }

    /**
     * @brief Liefert die Map-Koordinaten zurück, an denen sich der Mauszeiger grade befindet.
     * @return Map-Koordinaten, an denen sich der Mauszeiger grade befindet.
     */
    const MapCoords& getMapCoordsUnderMouse() const {
        return mapCoordsUnderMouse;
    }

    /**
     * @brief Zeichnet die Benutzeroberfläche
     */
    void render();

#ifndef NO_SDL
    /**
     * @brief Callback, der ein Event handelt
     * @param event SDL-Event
     */
    void onEvent(SDL_Event& event);
#endif

    /**
     * @brief Callback, der sich drum kümmert, wenn auf der Karte ein anderes MapObject ausgewählt wurde.
     * @param newSelectedMapObject das neu gewählte MapObject oder `nullptr`, wenn abgewählt wurde
     */
    void onSelectedMapObjectChanged(const MapObject* newSelectedMapObject);

    /**
     * @brief Callback, der in den Datenstrukturen der GUI-Komponenten notwendige Arbeiten erledigt,
     * wenn ein neues Spiel gestartet wird. Zu diesem Zeitpunkt hat das Game und die Map schon alles fertig für
     * das neue Spiel.
     */
    void onNewGame();

    /**
     * @brief Callback, wenn sich der bebaubare Bereich ändert. Wir benachrichtigen dann die Minimap,
     * die diese Info braucht.
     */
    void onBuildableAreaChanged();

    /**
     * @brief Callback, wenn sich Informationen bezüglich der Häuser geändert haben.
     * Wir aktualisieren daraufhin das GuiSelectedHouseBuildingWidget, wenn es grade aktiv ist.
     */
    void onHouseInfoChanged();

    /**
     * @brief Ändert den Text, den die Statusleiste anzeigen soll
     * @param text anzuzeigender Text
     */
    void setStatusBarText(const std::string& text);

    /**
     * @brief Liefert den aktuellen Zustand des Panels zurück
     * @return Zustand des Panels
     */
    PanelState const& getPanelState() const {
        return panelState;
    }

    /**
     * @brief Setzt das Quit-Flag, um Beenden des Spiels einzuleiten
     */
    void quit() {
        quitGame = true;
    }

    /**
     * @brief Ermittelt, ob der Benutzer das Spiel via UI beenden will. Die Hauptschleife muss dieses Flag checken
     * und falls gesetzt, die Anwendung beenden.
     *
     * @return `true`, um das Spiel zu beenden.
     */
    bool hasToQuitGame() const {
        return quitGame;
    }

    /**
     * @brief Zoomt die Karte eine Stufe rein. No-Op, wenn schon auf Anschlag reingezoomt.
     */
    void increaseMapZoom();

    /**
     * @brief Zoomt die Karte eine Stufe raus. No-Op, wenn schon auf Anschlag rausgezoomt.
     */
    void decreaseMapZoom();

    /**
     * @brief Im Baumodus. Ändert den ausgewählten MapObjectType, der gebaut werden soll und schließt das Baumenü
     * @param mapObjectType MapObjectType, der ausgewählt werden soll
     */
    void setMapObjectToBuild(const MapObjectType* mapObjectType);

    /**
     * @brief Macht einen Screenshot im Bitmap-Format.
     * @param fullMap `true`, um die komplette Karte als Riesenscreenshot zu speichern
     *                `false` für einen normalen Screenshot mit GUI
     * @param filename Dateiname, unter dem der Screenshot abgespeichert werden soll
     */
    void takeScreenshot(bool fullMap, const char* filename);

private:
    /**
     * @brief Legt alle Panel-Widgets an
     */
    void initPanelWidgets();

    /**
     * @brief Registriert ein neues GUI-Element
     * @param identifier eindeutiger Identifier, um das Element mit diesem anzusprechen
     * @param guiElement Referenz zum GUI-Element
     */
    void registerElement(int identifier, GuiBase* guiElement);
    
    /**
     * @brief Liefert ein bestimmtes GUI-Element zurück.
     * @param identifier Identifier des Elements
     * @return Referenz zum GUI-Element
     */
    GuiBase* findElement(int identifier);

    /**
     * @brief Aktualisiert die Sichtbarkeit der GUI-Komponenten des Panels ausgehend vom Stand der aktuellen
     * Variablen #panelState.
     */
    void updateGuiFromPanelState();

    /**
     * @brief Ändert die Zoom-Stufe der Karte
     * @param map (Denpedency)
     * @param newScreenZoom neue Zoom-Stufe
     */
    void changeMapZoom(Map* map, int newScreenZoom);

    /**
     * @brief Scrollt die Karte
     * @param map (Denpedency)
     * @param xDelta Veränderung in X-Map-Koordinaten
     * @param yDelta Veränderung in Y-Map-Koordinaten
     */
    void scrollMap(Map* map, int xDelta, int yDelta);
};

#endif
