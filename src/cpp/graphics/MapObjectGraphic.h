#ifndef _MAP_OBJECT_GRAPHIC_H
#define _MAP_OBJECT_GRAPHIC_H

#include <SDL.h>
#include <SDL_image.h>
#include "graphics/PlainGraphic.h"

/**
* @brief Grafik eines MapObjects. Die Grafik hat eine fixe Größe in Map-Koordinaten zugeordnet, sodass sie in das
* Kachelsystem der Map gezeichnet werden kann
*/
class MapObjectGraphic : public PlainGraphic {

public:
    /**
     * @brief zeichnet das Objekt maskiert, d.h. gelbem Overlay für Gebäudeplatzierung
     * @sa #drawAt()
     */
    static const int DRAWING_FLAG_MASKED   = (1 << 0);

    /**
     * @brief zeichnet das Objekt rot eingefärbt
     * Info: Rot und abgedunkelt schließen sich gegenseitig aus.
     * @sa #drawAt()
     */
    static const int DRAWING_FLAG_RED      = (1 << 1);

    /**
     * @brief zeichnet das Objekt blinkend, d.h. entweder wird die Grafik gezeichnet oder nicht
     * @sa #drawAt()
     */
    static const int DRAWING_FLAG_BLINK    = (1 << 2);

    /**
     * @brief zeichnet das Objekt abgedunkelt
     * Info: Rot und abgedunkelt schließen sich gegenseitig aus.
     * @sa #drawAt()
     */
    static const int DRAWING_FLAG_DARKENED = (1 << 3);


private:
    /**
     * @brief Breite (X-Richtung) der Grafik in Map-Koordinaten
     */
    unsigned char mapWidth;

    /**
     * @brief Höhe (Y-Richtung) der Grafik in Map-Koordinaten
     */
    unsigned char mapHeight;

    /**
     * @brief SDL-Texture der maskierten Grafik zurück. Diese wird verwendet, wenn ein neues Gebäude
     * positioniert wird.
     */
    SDL_Texture* textureMasked;

public:
    /**
     * @brief Konstruktor. Lädt die Grafik.
     * @param filename Dateiname der zu ladenden Grafik
     * @param mapWidth Breite in mapCoords
     * @param mapHeight Höhe in mapCoords
     */
    MapObjectGraphic(const char* filename, unsigned char mapWidth, unsigned char mapHeight);

    /**
     * @brief Destruktor. Entlädt die Grafik und gibt Speicher frei.
     */
    virtual ~MapObjectGraphic();

    /**
     * @brief Liefert die Breite der Grafik in Map-Koordinaten zurück
     * @return Breite der Grafik in Map-Koordinaten
     */
    unsigned char getMapWidth() const {
        return mapWidth;
    }

    /**
     * @brief Liefert die Höhe der Grafik in Map-Koordinaten zurück
     * @return Breite der Höhe in Map-Koordinaten
     */
    unsigned char getMapHeight() const {
        return mapHeight;
    }


    /**
     * @brief Zeichnet die Grafik mit bestimmten Drawing-Flags. Quell- und Zielbereich können beliebig gewählt werden.
     * @param rectSource Quell-Rechteck der Zeichenoperation oder nullptr, wenn die komplette Grafik verwendet werden soll
     * @param rectDestination Ziel-Rechteck der Zeichenoperation oder nullptr, wenn ganzflächig gemalt werden soll
     * @param drawingFlags Bitmaske aus DRAWING_FLAG_*-Konstanten, die angeben, wie die Grafik gezeichnet werden soll
     */
    void draw(SDL_Rect* rectSource, SDL_Rect* rectDestination, int drawingFlags);

private:
    /**
     * @brief Erzeugt @ref textureMasked. Erwartet, dass @ref surface bereits mit der Grafik befüllt ist.
     */
    void createMaskedTexture();

};

#endif