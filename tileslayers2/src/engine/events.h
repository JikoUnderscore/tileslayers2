#pragma once
#include <SDL2/SDL.h>
#undef main
namespace CORE {


namespace event {
    using Event_t = SDL_Event;

    typedef enum {
        First = SDL_FIRSTEVENT,

        Quit = SDL_QUIT,
        AppTerminating = SDL_APP_TERMINATING,
        AppLowMemory = SDL_APP_LOWMEMORY,
        AppWillEnterBackground = SDL_APP_WILLENTERBACKGROUND,
        AppDidEnterBackground = SDL_APP_DIDENTERBACKGROUND,
        AppWillEnterForeground = SDL_APP_WILLENTERFOREGROUND,
        AppDidEnterForeground = SDL_APP_DIDENTERFOREGROUND,

        LOCALECHANGED = SDL_LOCALECHANGED,

        Display = SDL_DISPLAYEVENT,
        Window = SDL_WINDOWEVENT,

        SysWM = SDL_SYSWMEVENT,

        KeyDown = SDL_KEYDOWN,
        KeyUp = SDL_KEYUP,
        TextEditing = SDL_TEXTEDITING,
        TextInput = SDL_TEXTINPUT,

        KEYMAPCHANGED = SDL_KEYMAPCHANGED,
        TEXTEDITING_EXT = SDL_TEXTEDITING_EXT,

        MouseMotion = SDL_MOUSEMOTION,
        MouseButtonDown = SDL_MOUSEBUTTONDOWN,
        MouseButtonUp = SDL_MOUSEBUTTONUP,
        MouseWheel = SDL_MOUSEWHEEL,

        JoyAxisMotion = SDL_JOYAXISMOTION,
        JoyBallMotion = SDL_JOYBALLMOTION,
        JoyHatMotion = SDL_JOYHATMOTION,
        JoyButtonDown = SDL_JOYBUTTONDOWN,
        JoyButtonUp = SDL_JOYBUTTONUP,
        JoyDeviceAdded = SDL_JOYDEVICEADDED,
        JoyDeviceRemoved = SDL_JOYDEVICEREMOVED,
        JOYBATTERYUPDATED = SDL_JOYBATTERYUPDATED,

        ControllerAxisMotion = SDL_CONTROLLERAXISMOTION,
        ControllerButtonDown = SDL_CONTROLLERBUTTONDOWN,
        ControllerButtonUp = SDL_CONTROLLERBUTTONUP,
        ControllerDeviceAdded = SDL_CONTROLLERDEVICEADDED,
        ControllerDeviceRemoved = SDL_CONTROLLERDEVICEREMOVED,
        ControllerDeviceRemapped = SDL_CONTROLLERDEVICEREMAPPED,
        ControllerTouchpadDown = SDL_CONTROLLERTOUCHPADDOWN,
        ControllerTouchpadMotion = SDL_CONTROLLERTOUCHPADMOTION,
        ControllerTouchpadUp = SDL_CONTROLLERTOUCHPADUP,
        ControllerSensorUpdate = SDL_CONTROLLERSENSORUPDATE,

        FingerDown = SDL_FINGERDOWN,
        FingerUp = SDL_FINGERUP,
        FingerMotion = SDL_FINGERMOTION,
        DollarGesture = SDL_DOLLARGESTURE,
        DollarRecord = SDL_DOLLARRECORD,
        MultiGesture = SDL_MULTIGESTURE,

        ClipboardUpdate = SDL_CLIPBOARDUPDATE,
        DropFile = SDL_DROPFILE,
        DropText = SDL_DROPTEXT,
        DropBegin = SDL_DROPBEGIN,
        DropComplete = SDL_DROPCOMPLETE,

        AudioDeviceAdded = SDL_AUDIODEVICEADDED,
        AudioDeviceRemoved = SDL_AUDIODEVICEREMOVED,

        SENSORUPDATE = SDL_SENSORUPDATE,

        RenderTargetsReset = SDL_RENDER_TARGETS_RESET,
        RenderDeviceReset = SDL_RENDER_DEVICE_RESET,

        POLLSENTINEL = SDL_POLLSENTINEL,

        Userevent = SDL_USEREVENT,
        Lastevent = SDL_LASTEVENT,
    } EventType;


    inline fn poll_event(Event_t* event)->bool {
        return SDL_PollEvent(event) == 1;
    }
} // namespace event


} // namespace CORE
