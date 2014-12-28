#ifdef WINDOWS
#define SDL_MAIN_HANDLED
#endif
#include <iostream>
#include "Context.h"
#include "config/ConfigMgr.h"
#include "economics/EconomicsMgr.h"
#include "game/Game.h"
#include "game/GameIO.h"
#include "graphics/mgr/sdl/SDLFontMgr.h"
#include "graphics/mgr/sdl/SDLGraphicsMgr.h"
#include "graphics/renderer/sdl/SDLRenderer.h"
#include "gui/GuiMgr.h"
#include "map/Map.h"
#include "sound/sdl/SDLSoundMgr.h"
#include "utils/Color.h"
#include "utils/FpsCounter.h"
#include "utils/StringFormat.h"

#ifdef DEBUG_A_STAR
#include "pathfinding/AStar.h"
#endif

static Color colorWhite = Color(255, 255, 255, 255);


/*********************************************************************************************************************
 * globale Variablen                                                                                                 *
 *********************************************************************************************************************/

#ifdef DEBUG
/**
 * @brief Zeichenketten-Puffer für 7 Zeilen Debug-Ausgabe
 */
static std::string debugOutput[7];
#endif

/*********************************************************************************************************************
 * Prototypen                                                                                                        *
 *********************************************************************************************************************/

int main(int argc, char** argv);
void drawFrame(const Context& context, IRenderer* renderer);

/*********************************************************************************************************************
 * Implementierung                                                                                                   *
 *********************************************************************************************************************/

void drawFrame(const Context& context, IRenderer* renderer) {
    Map* map = context.game->getMap();

	// UI rendern
    context.guiMgr->render();
    
    // Statuszeile
    const MapObject* selectedMapObject = map->getSelectedMapObject();
    if (selectedMapObject != nullptr) {
        const Building* selectedBuilding = reinterpret_cast<const Building*>(selectedMapObject);
        if (selectedBuilding != nullptr) {
            const BuildingConfig* buildingConfig =
                context.configMgr->getBuildingConfig(selectedBuilding->getStructureType());
            context.fontMgr->renderText(renderer, buildingConfig->name, 753, 744,
                &colorWhite, nullptr, "DroidSans-Bold.ttf", 14, RENDERTEXT_HALIGN_RIGHT);
        }
    }

#ifdef DEBUG
	// Debugging-Infos rendern
	for (int i = 0; i < 7; i++) {
		if (debugOutput[i].empty()) {
			continue;
		}

        context.fontMgr->renderText(renderer, debugOutput[i], 10, 40 + 15 * i,
            &colorWhite, nullptr, "DroidSans-Bold.ttf", 14, RENDERTEXT_HALIGN_LEFT);
	}
#endif
}

int main(int argc, char** argv) {
	// Library-Initialisierung ///////////////////////////////////////////////////////////////////////////////////////
    IRenderer* sdlRenderer = new SDLRenderer();

	// Game-Initialisierung //////////////////////////////////////////////////////////////////////////////////////////

    Context context = Context();

	ISoundMgr* sdlSoundMgr = new SDLSoundMgr();
    context.soundMgr = sdlSoundMgr;

    ConfigMgr* configMgr = new ConfigMgr();
    context.configMgr = configMgr;

	IGraphicsMgr* sdlGraphicsMgr = new SDLGraphicsMgr(sdlRenderer, configMgr);
    context.graphicsMgr = sdlGraphicsMgr;
    sdlGraphicsMgr->loadGraphics();

    IFontMgr* sdlFontMgr = new SDLFontMgr();
    context.fontMgr = sdlFontMgr;

    GuiMgr* guiMgr = new GuiMgr(&context, sdlRenderer);
    context.guiMgr = guiMgr;
    guiMgr->initGui();

    EconomicsMgr* economicsMgr = new EconomicsMgr(&context);
    context.economicsMgr = economicsMgr;

    FpsCounter* fpsCounter = new FpsCounter(500);
    
    Game* game = new Game(&context);
    context.game = game;

    game->loadGameFromTMX("data/map/empty-map.tmx");

	// Mainloop //////////////////////////////////////////////////////////////////////////////////////////////////////

    Map* map = game->getMap();
	while (!guiMgr->hasToQuitGame()) {
		fpsCounter->startFrame();

        // während eines Frames nur einmal auf die Uhr gucken, damit z.B. beim Blinkend-Zeichnen nicht ein Schnibbel
        // eines Gebäude gezeichnet und ein anderer ne Millisekunde später nicht mehr
        context.sdlTicks = SDL_GetTicks();

		// Events handeln
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
            guiMgr->onEvent(event);
		}

        // Wirtschaft ankurbeln ;-)
        std::list<MapObject*> mapObjects = map->getMapObjects();
        for (auto iter = mapObjects.rbegin(); iter != mapObjects.rend(); iter++) {
            MapObject* mapObject = *iter;
            Structure* structure = dynamic_cast<Structure*>(mapObject);

            if (structure != nullptr) {
                economicsMgr->update(structure);
            }
        }

        // Position des Mauszeigers holen
        SDL_GetMouseState(&context.mouseCurrentX, &context.mouseCurrentY);

#ifdef DEBUG
        int screenOffsetX = map->getScreenOffsetX();
        int screenOffsetY = map->getScreenOffsetY();
        int screenZoom = map->getScreenZoom();

        int screenX, screenY;
        screenX = (context.mouseCurrentX * screenZoom) + screenOffsetX;
        screenY = (context.mouseCurrentY * screenZoom) + screenOffsetY;

        int mouseCurrentMapX, mouseCurrentMapY;
        MapCoordUtils::getMapCoordsUnderMouse(
            map, context.mouseCurrentX, context.mouseCurrentY, mouseCurrentMapX, mouseCurrentMapY);

		// Debug-Infos vorbereiten, damit wir sie später einfach nur ausgeben können
		debugOutput[0] = "FPS: average = " + toString(fpsCounter->getFpsAvg()) +
                ", current = " + toString(fpsCounter->getFpsCurrent());
        
        debugOutput[1] = "Screen: offset = (" +
                toString(screenOffsetX) + ", " + toString(screenOffsetY) + "), zoom = " +
                toString(screenZoom);

        debugOutput[2] = "mouse = (" + 
                toString(context.mouseCurrentX) + ", " + toString(context.mouseCurrentY) + "), mapElevated = (" +
                toString(mouseCurrentMapX) + ", " + toString(mouseCurrentMapY) + "), screen = (" +
                toString(screenX) + ", " + toString(screenY) + ")";
    
        if (map->getSelectedMapObject() != nullptr) {
            int mapX, mapY, mapWidth, mapHeight;
            map->getSelectedMapObject()->getMapCoords(mapX, mapY, mapWidth, mapHeight);
            
            debugOutput[3] = "selectedMapObject on mapCoords (" + 
                    toString(mapX) + ", " + toString(mapY) + "), size = (" +
                    toString(mapWidth) + ", " + toString(mapHeight) + ")";
        } else {
            debugOutput[3] = "";
        }

#ifdef DEBUG_A_STAR
        std::string buildingToUseCatchmentAreaString;
        if (AStar::debugAStar_buildingToUseCatchmentArea != nullptr) {
            int mapX, mapY;
            AStar::debugAStar_buildingToUseCatchmentArea->getMapCoords(mapX, mapY);

            buildingToUseCatchmentAreaString = "(" + toString(mapX) + ", " + toString(mapY) + ")";
        } else {
            buildingToUseCatchmentAreaString = "nullptr";
        }

        debugOutput[4] = "debugAStar: source = (" +
            toString(AStar::debugAStar_source.mapX) + ", " +
            toString(AStar::debugAStar_source.mapY) + "), destination = (" +
            toString(AStar::debugAStar_destination.mapX) + ", " +
            toString(AStar::debugAStar_destination.mapY) + "), catchmentAreaBuilding = " +
            buildingToUseCatchmentAreaString + ", useStreetOnly = " +
            (AStar::debugAStar_useStreetOnly ? "true" : "false");
#endif // DEBUG_A_STAR
#endif // DEBUG

        sdlRenderer->startFrame();
		drawFrame(context, sdlRenderer);
        sdlRenderer->endFrame();

		fpsCounter->endFrame();
	}

	// Game-Deinitialisierung ////////////////////////////////////////////////////////////////////////////////////////
    
    delete game;
    
    delete fpsCounter;
    delete economicsMgr;
	delete guiMgr;
    delete sdlFontMgr;
	delete sdlGraphicsMgr;
    delete configMgr;
	delete sdlSoundMgr;

	// Library-Deinitialisierung /////////////////////////////////////////////////////////////////////////////////////

	delete sdlRenderer;

	return EXIT_SUCCESS;
}
