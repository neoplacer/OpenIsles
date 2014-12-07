#include "map/Structure.h"
#include "gui/Identifiers.h"
#include "gui/components/GuiAddBuildingWidget.h"
#include "gui/components/GuiButton.h"
#include "gui/components/GuiPushButton.h"
#include "gui/components/GuiStaticElement.h"
#include "gui/panel-widgets/GuiBuildMenuWidget.h"


GuiBuildMenuWidget::GuiBuildMenuWidget(const Context* const context) : GuiPanelWidget(context) {
    // TODO in Config auslagern
    static struct {
        BuildingGroup buildingGroup;
        const char* name;
        OtherGraphic graphic;
        OtherGraphic graphicPressed;

        struct {
            StructureType structureType;
            const char* name;
            OtherGraphic graphic;
        } buildings[16];

    } buildingGroups[4] = {
        {
            BuildingGroup::CRAFTSMAN,
            "Handwerksbetriebe",
            OtherGraphic::ADD_BUILDING_GROUP_CRAFTSMAN,
            OtherGraphic::ADD_BUILDING_GROUP_CRAFTSMAN_PRESSED, {
            { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
            { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
            { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
            { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
            {
                StructureType::STONEMASON,
                "Steinmetz",
                OtherGraphic::ADD_BUILDING_STONEMASON
            },
            { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
            { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
            {
                StructureType::TOOLSMITHS,
                "Werkzeugschmiede",
                OtherGraphic::ADD_BUILDING_TOOLSMITHS
            },
            { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
            { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
            { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
            { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
            {
                StructureType::WEAVING_MILL1,
                "Webstube",
                OtherGraphic::ADD_BUILDING_WEAVING_MILL1
            },
            {
                StructureType::BUTCHERS,
                "Fleischerei",
                OtherGraphic::ADD_BUILDING_BUTCHERS
            },
            { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
            { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY }
        }
        }, {
            BuildingGroup::FARM,
            "Farmen & Plantagen",
            OtherGraphic::ADD_BUILDING_GROUP_FARM,
            OtherGraphic::ADD_BUILDING_GROUP_FARM_PRESSED, {
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                {
                    StructureType::SHEEP_FARM,
                    "Schaffarm",
                    OtherGraphic::ADD_BUILDING_SHEEP_FARM
                },
                {
                    StructureType::CATTLE_FARM,
                    "Rinderfarm",
                    OtherGraphic::ADD_BUILDING_CATTLE_FARM
                },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                {
                    StructureType::FORESTERS,
                    "Förster",
                    OtherGraphic::ADD_BUILDING_DUMMY
                },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY }
            }
        }, {
            BuildingGroup::PORT,
            "Hafenanlagen",
            OtherGraphic::ADD_BUILDING_GROUP_PORT,
            OtherGraphic::ADD_BUILDING_GROUP_PORT_PRESSED, {
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                {
                    StructureType::OFFICE1,
                    "Kontor I",
                    OtherGraphic::ADD_BUILDING_OFFICE1
                },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY }
            }
        }, {
            BuildingGroup::PUBLIC,
            "Öffentliche Gebäude",
            OtherGraphic::ADD_BUILDING_GROUP_PUBLIC,
            OtherGraphic::ADD_BUILDING_GROUP_PUBLIC_PRESSED, {
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY },
                {
                    StructureType::STREET,
                    "Plasterstraße",
                    OtherGraphic::ADD_BUILDING_STREET
                }, {
                    StructureType::PIONEERS_HOUSE1,
                    "Haus",
                    OtherGraphic::ADD_BUILDING_HOUSE
                }, {
                    StructureType::MARKETPLACE,
                    "Marktplatz",
                    OtherGraphic::ADD_BUILDING_MARKETPLACE
                }, {
                    StructureType::CHAPEL,
                    "Kapelle",
                    OtherGraphic::ADD_BUILDING_CHAPEL
                },
                { StructureType::NO_STRUCTURE, nullptr, OtherGraphic::ADD_BUILDING_DUMMY }
            }
        }
    };

    for (int groupIndex = 0; groupIndex < 4; groupIndex++) {
        // Grid
        GuiStaticElement* addBuildingGrid = new GuiStaticElement(context);
        PlainGraphic* graphicAddBuildingGrid = context->graphicsMgr->getOtherGraphic(OtherGraphic::ADD_BUILDING_GRID);
        addBuildingGrid->setCoords(775, 450, graphicAddBuildingGrid->getWidth(), graphicAddBuildingGrid->getHeight());
        addBuildingGrid->setGraphic(graphicAddBuildingGrid);
        addBuildingGrid->setVisible(false);
        context->guiMgr->registerElement(GUI_ID_ADD_BUILDING_GRID_BASE + groupIndex, addBuildingGrid);

        // Buttons im Grid
        for (int gridY = 0; gridY < 4; gridY++) {
            for (int gridX = 0; gridX < 4; gridX++) {
                int buildingIndex = gridY * 4 + gridX;
                StructureType structureType = buildingGroups[groupIndex].buildings[buildingIndex].structureType;

                if (structureType == NO_STRUCTURE) {
                    continue;
                }

                GuiButton* addBuildingButton = new GuiButton(context);
                PlainGraphic* graphicAddBuildingButton =
                    context->graphicsMgr->getOtherGraphic(buildingGroups[groupIndex].buildings[buildingIndex].graphic);
                addBuildingButton->setCoords(
                    12 + 58 * gridX, 13 + 58 * gridY, graphicAddBuildingButton->getWidth(), graphicAddBuildingButton->getHeight());
                addBuildingButton->setGraphic(graphicAddBuildingButton);
                addBuildingButton->setGraphicPressed(graphicAddBuildingButton);
                addBuildingButton->setOnClickFunction([ this, context, structureType ]() {
                    context->guiMgr->panelState.addingStructure = structureType;
                    context->guiMgr->panelState.buildingMenuOpen = false;
                    context->guiMgr->updateGuiFromPanelState();
                });
                context->guiMgr->registerElement(GUI_ID_ADD_BUILDING_GRID_BUTTON_BASE + groupIndex * 16 + buildingIndex, addBuildingButton);
                addBuildingGrid->addChildElement(addBuildingButton);
            }
        }

        // Button für die Gruppe
        GuiPushButton* addBuildingPushButton = new GuiPushButton(context);
        addBuildingPushButton->setGraphic(context->graphicsMgr->getOtherGraphic(buildingGroups[groupIndex].graphic));
        addBuildingPushButton->setGraphicPressed(context->graphicsMgr->getOtherGraphic(buildingGroups[groupIndex].graphicPressed));
        addBuildingPushButton->setCoords(12 + groupIndex * 55, 378, 52, 64);
        addBuildingPushButton->setOnClickFunction([ this, context, groupIndex ]() {
            // Wenn man die Gruppe nochmal klickt, die bereits ausgewählt ist und das ausgewählte Gebäude nicht
            // aus dieser Gruppe ist, wird das Gebäude gewechselt und eins aus der Gruppe genommen
            if (context->guiMgr->panelState.selectedBuildingGroup == (BuildingGroup) groupIndex &&
                context->guiMgr->panelState.buildingMenuOpen) {

                bool addingStructureInSelectedBuildingGroup = false;
                for (int i = 0; i < 16; i++) {
                    if (buildingGroups[groupIndex].buildings[i].structureType == context->guiMgr->panelState.addingStructure) {
                        addingStructureInSelectedBuildingGroup = true;
                        break;
                    }
                }

                if (!addingStructureInSelectedBuildingGroup) {
                    // Gebäude unten links nehmen (= Index 12 von 16)
                    context->guiMgr->panelState.addingStructure = buildingGroups[groupIndex].buildings[12].structureType;
                }
            }
            else {
                context->guiMgr->panelState.selectedBuildingGroup = (BuildingGroup) groupIndex;
                context->guiMgr->panelState.buildingMenuOpen = true;
            }
            context->guiMgr->updateGuiFromPanelState();
        });
        context->guiMgr->registerElement(GUI_ID_ADD_BUILDING_PUSH_BUTTON_BASE + groupIndex, addBuildingPushButton);
        addChildElement(addBuildingPushButton);
    }

    // Gebäudebau: Infos, über zu platzierendes Gebäude
    GuiAddBuildingWidget* addBuildingWidget = new GuiAddBuildingWidget(context);
    context->guiMgr->registerElement(GUI_ID_ADD_BUILDING_WIDGET, addBuildingWidget);
    addChildElement(addBuildingWidget);
}