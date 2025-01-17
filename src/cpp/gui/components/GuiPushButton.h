#ifndef _GUI_PUSH_BUTTON_H
#define _GUI_PUSH_BUTTON_H

#include "global.h"
#include "gui/components/GuiButton.h"

class SDL_Renderer;


/**
 * Button, den man an- und ausschalten kann.
 * Dieser Button nutzt für "heruntergedrückt" und "angeschaltet" dieselben Grafiken.
 */
class GuiPushButton : public GuiButton {

private:
    /**
     * @brief gibt an, ob der Button an- (true) oder ausgeschaltet (false) ist.
     */
    bool active = false;

public:
    GuiPushButton(const Context& context);
    virtual ~GuiPushButton();
    
    /**
     * @return true, wenn der Button angeschaltet ist, sonst false
     */
    bool isActive() {
        return active;
    }
    
    /**
     * @brief Setzt den Zustand des Buttons
     * @param active true, um den Button anzuschalten, false, um ihn auszuschalten
     */
    void setActive(bool active) {
        this->active = active;
    }
    
    /**
     * @brief Zeichnet das Element (ohne Kinder)
     */
    virtual void renderElement(IRenderer* renderer);
    
    /**
     * @brief Callback, der ein Event handelt
     * @param event SDL-Event
     * @return `true` um das Event an weitere GUI-Elemente zu reichen,
     *         `false` um anzudeuten, dass das Event bereits verarbeitet wurde. Es wird dann nicht weiter zugestellt.
     */
    virtual bool onEventElement(SDL_Event& event);

};

#endif
