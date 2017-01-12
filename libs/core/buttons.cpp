#include "pxt.h"
#include "DeviceSystemTimer.h"
#include "AnalogSensor.h"

/**
* User interaction on buttons
*/
enum class ButtonEvent {
    //% block="click"
    Click = DEVICE_BUTTON_EVT_CLICK,
    //% block="double click"
    DoubleClick = DEVICE_BUTTON_EVT_DOUBLE_CLICK,
    //% block="long click"
    LongClick = DEVICE_BUTTON_EVT_LONG_CLICK,
    //% block="up"
    Up = DEVICE_BUTTON_EVT_UP,
    //% block="down"
    Down = DEVICE_BUTTON_EVT_DOWN,
    //% block="hold"
    Hold = DEVICE_BUTTON_EVT_HOLD
};

namespace pxt {

// Wrapper classes

class WButtons {
  public:
#define Button DeviceButton
    Button buttons[0];
    //% indexedInstanceNS=input indexedInstanceShim=pxt::getButton
    /**
     * Left push button.
     */
    //% block="left button"
    Button leftButton;
    /**
     * Right push button.
     */
    //% block="right button"
    Button rightButton;
    /**
     * Slide switch.
     */
    //% block="slide switch"
    Button slideSwitch;
#undef Button

    WButtons()
        : leftButton(*pxt::lookupPin(PIN_BTN_LEFT), DEVICE_ID_BUTTON_A, DEVICE_BUTTON_ALL_EVENTS, ACTIVE_HIGH,
               PullDown),
          rightButton(*pxt::lookupPin(PIN_BTN_RIGHT), DEVICE_ID_BUTTON_B, DEVICE_BUTTON_ALL_EVENTS, ACTIVE_HIGH,
                PullDown),
          slideSwitch(*pxt::lookupPin(PIN_BTN_SLIDE), DEVICE_ID_BUTTON_SLIDE, DEVICE_BUTTON_ALL_EVENTS,
                ACTIVE_LOW, PullUp)
    { }
};
SINGLETON(WButtons);

const int LastButtonID = &((WButtons*)0)->slideSwitch - ((WButtons*)0)->buttons;

//%
DeviceButton *getButton(int id) {
    if (!(0 <= id && id <= LastButtonID))
        device.panic(42);
    return &getWButtons()->buttons[id];
}

}

//% noRefCounting fixedInstances
namespace ButtonMethods {
/**
 * Do something when a button (``A``, ``B`` or both ``A+B``) is clicked, double clicked, etc...
 * @param button the button that needs to be clicked or used
 * @param event the kind of button gesture that needs to be detected
 * @param body code to run when event is raised
 */
//% help=input/on-button-event weight=85 blockGap=8
//% blockId=buttonEvent block="on %button|%event"
//% parts="buttonpair"
//% blockNamespace=input
void onEvent(Button button, ButtonEvent ev, Action body) {
    registerWithDal(button->id, (int)ev, body);
}

/**
 * Get the button state (pressed or not).
 * @param button the button to query the request
 */
//% help=input/button-is-pressed weight=60
//% block="%NAME|is pressed"
//% blockId=buttonIsPressed
//% blockGap=8
//% parts="buttonpair"
//% blockNamespace=input
bool isPressed(Button button) {
    return button->isPressed();
}

/**
 * Indicates if the button was pressed since this function was last called.
 * @param button the button to query the request
 */
//% help=input/button-was-pressed weight=60
//% block="%NAME|was pressed"
//% blockId=buttonWasPressed
//% blockGap=8
//% parts="buttonpair"
//% blockNamespace=input
bool wasPressed(Button button) {
    return button->wasPressed();
}
}