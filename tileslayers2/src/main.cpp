#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#undef main

#include <expected>
#include <iostream>
#include <span>
#include "defines.h"


#define PrintAndRerunErr(err)                                                                                                                        \
    auto e_er{ static_cast<Errors::ALLERRORS>((err)) };                                                                                              \
    std::cerr << e_er << '\n';                                                                                                                       \
    return e_er

namespace Errors {
enum ALLERRORS : u8 {
    SDLInitErr,
    SDLCreateWindowErr,
    SDLCreateRendererErr,
    SDLIMGInitErr,
    SDLIMGLoadTextureErr,
    SDLRenderSetLogicalSizeErr,
};

enum class RendererInitErr : u8 {
    SDLInitErr = ALLERRORS::SDLInitErr,
    SDLCreateWindowErr = ALLERRORS::SDLCreateWindowErr,
    SDLCreateRendererErr = ALLERRORS::SDLCreateRendererErr,
    SDLRenderSetLogicalSizeErr = ALLERRORS::SDLRenderSetLogicalSizeErr,
};

enum class IMGInitErr : u8 {
    SDLIMGInitErr = ALLERRORS::SDLIMGInitErr,
};

enum class IMGLoadTextureErr : u8 {
    SDLIMGLoadTextureErr = ALLERRORS::SDLIMGLoadTextureErr,
};


auto operator<<(std::ostream& out, const ALLERRORS value) -> std::ostream& {
    const char* s = nullptr;
#define PROCESS_VAL(p)                                                                                                                               \
    case (p):                                                                                                                                        \
        s = #p;                                                                                                                                      \
        break
    switch (value) {
        PROCESS_VAL(ALLERRORS::SDLInitErr);
        PROCESS_VAL(ALLERRORS::SDLCreateWindowErr);
        PROCESS_VAL(ALLERRORS::SDLCreateRendererErr);
        PROCESS_VAL(ALLERRORS::SDLIMGInitErr);
        PROCESS_VAL(ALLERRORS::SDLIMGLoadTextureErr);
        PROCESS_VAL(ALLERRORS::SDLRenderSetLogicalSizeErr);
    }
#undef PROCESS_VAL

    return out << s;
}


} // namespace Errors

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

static const constexpr i32 LOGICAL_RES_1280 = 1280;
static const constexpr i32 LOGICAL_RES_720 = 720;

struct KeyboardState {
    std::span<const u8> state;

    static_fn init() noexcept -> KeyboardState {
        int len{};
        let ptr{ SDL_GetKeyboardState(&len) };

        return KeyboardState{
            .state = std::span<const u8>(ptr, len),
        };
    }

    fn is_scancode_pressed(this KeyboardState const& self, const SDL_Scancode sc) noexcept -> bool {
        return self.state[static_cast<usize>(sc)] != 0;
    }
};

struct Vector2f {
    f32 x;
    f32 y;


    constexpr static_fn zero()->Vector2f {
        return Vector2f{
            .x = 0.0,
            .y = 0.0,
        };
    }
};

struct FpsCap {
    u32 m_frame_delay;
    f32 dt;
    u64 m_last_time;

    explicit FpsCap(const u32 fps_cap) : m_frame_delay(1000 / fps_cap), dt(0.0f), m_last_time(SDL_GetPerformanceCounter()) {
    }

    fn start(this FpsCap& self) noexcept -> void {
        const u64 now = SDL_GetPerformanceCounter();
        const u64 elapsed = now - self.m_last_time;
        self.dt = static_cast<f32>(elapsed) / static_cast<f32>(SDL_GetPerformanceFrequency());
        printf("FPS: %f | dt %f\n", 1.0F / self.dt, self.dt);
        self.m_last_time = now;
    }

    fn end(this FpsCap const& self) noexcept -> void {
        const u64 now = SDL_GetPerformanceCounter();
        const u64 elapsed = now - self.m_last_time;
        const u32 cap_frame_end = static_cast<Uint32>((static_cast<f32>(elapsed) * 1000.0F) / static_cast<f32>(SDL_GetPerformanceFrequency()));

        if (cap_frame_end < self.m_frame_delay) {
            SDL_Delay((self.m_frame_delay - cap_frame_end));
        }
    }
};


struct PLayer {
    Vector2f position;

    static const constexpr i32 WIDTH = 24;
    static const constexpr i32 HEIGHT = 24;
    static const constexpr f32 SPEED = 200.0F;


    constexpr static_fn init(const f32 x, const f32 y)->PLayer {
        return PLayer{
            .position = Vector2f{ .x = x, .y = y },
        };
    }
};


struct Camera {
    Vector2f offset_float;

    static const constexpr i32 MOUSE_MOVE_LIMIT_X = 360;
    static const constexpr i32 MOUSE_MOVE_LIMIT_Y = 250;

    fn folow_player(this Camera& self, Vector2f& player_pos, Vector2f& map_pos, const i32 mouse_x, const i32 mouse_y) noexcept -> void {
        i32 mouse_pos_x{ mouse_x - (LOGICAL_RES_1280 / 2) - (PLayer::WIDTH / 2) };
        i32 mouse_pos_y{ mouse_y - (LOGICAL_RES_720 / 2) - (PLayer::HEIGHT / 2) };

        if (mouse_pos_x < -MOUSE_MOVE_LIMIT_X) {
            mouse_pos_x = -MOUSE_MOVE_LIMIT_X;
        }
        else if (mouse_pos_x > MOUSE_MOVE_LIMIT_X) {
            mouse_pos_x = MOUSE_MOVE_LIMIT_X;
        }
        if (mouse_pos_y < -MOUSE_MOVE_LIMIT_Y) {
            mouse_pos_y = -MOUSE_MOVE_LIMIT_Y;
        }
        else if (mouse_pos_y > MOUSE_MOVE_LIMIT_Y) {
            mouse_pos_y = MOUSE_MOVE_LIMIT_Y;
        }

        self.offset_float.x += player_pos.x - self.offset_float.x - (LOGICAL_RES_1280 / 2) + mouse_pos_x;
        self.offset_float.y += player_pos.y - self.offset_float.y - (LOGICAL_RES_720 / 2) + mouse_pos_y;

        // const i32 offset_int_x = static
        player_pos.x = player_pos.x - self.offset_float.x;
        player_pos.y = player_pos.y - self.offset_float.y;

        map_pos.x += -self.offset_float.x;
        map_pos.y += -self.offset_float.y;
    }
};


enum class ScreensID {
    StartMenu,
    FirstLevel,
};


struct GameStatePersistent {
    bool is_game_running;
    ScreensID curent_screen;
    PLayer pla;
    Camera camera;

    constexpr static_fn init() noexcept -> GameStatePersistent {
        return GameStatePersistent{
            .is_game_running = true,
            .curent_screen = ScreensID::StartMenu,
            .pla = PLayer::init(0, 0),
            .camera = Camera{ .offset_float = Vector2f::zero() },

        };
    }
};

struct Texture {
    SDL_Texture* raw_texure;

    fn deinit(this Texture const& self) noexcept -> void {
        SDL_DestroyTexture(self.raw_texure);
    }
};


struct Renderer {
    SDL_Window* raw_window;
    SDL_Renderer* raw_renderer;

    /// Destructor `Renderer::deinit()`
    static_fn init(char const* const title)->std::expected<Renderer, Errors::RendererInitErr> {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            // TODO(jiko) change SDL_INIT_EVERYTHING to the specific subsystems
            return std::unexpected(Errors::RendererInitErr::SDLInitErr);
        }
        let window{ SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN) };
        if (window == nullptr) {
            return std::unexpected(Errors::RendererInitErr::SDLCreateWindowErr);
        }

        let renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE) };

        if (renderer == nullptr) {
            return std::unexpected(Errors::RendererInitErr::SDLCreateRendererErr);
        }

        let res{ SDL_RenderSetLogicalSize(renderer, LOGICAL_RES_1280, LOGICAL_RES_720) };
        if (res != 0) {
            return std::unexpected(Errors::RendererInitErr::SDLCreateRendererErr);
        }


        return Renderer{
            .raw_window = window,
            .raw_renderer = renderer,
        };
    }

    /// Destructor `Renderer::deinit()`
    static_fn init_image()->std::expected<void, Errors::IMGInitErr> {
        constexpr let flags{ IMG_INIT_PNG };

        let init_flags{ IMG_Init(flags) };
        if ((init_flags & flags) != flags) {
            return std::unexpected(Errors::IMGInitErr::SDLIMGInitErr);
        }

        return {};
    }

    fn deinit(this Renderer const& self) noexcept -> void {
        SDL_DestroyRenderer(self.raw_renderer);
        SDL_DestroyWindow(self.raw_window);
        IMG_Quit();
        SDL_Quit();
    }

    fn copy_with_size(this Renderer const& self, const Texture texture, const Vector2f pos, const i32 width, const i32 height) noexcept -> void {
#ifdef _DEBUG
        SDL_assert(width > 0);
        SDL_assert(height > 0);
#endif

        let dstrect{
            SDL_Rect{
            .x = static_cast<i32>(pos.x),
            .y = static_cast<i32>(pos.y),
            .w = width,
            .h = height,
            },
        };
        (void)SDL_RenderCopy(self.raw_renderer, texture.raw_texure, nullptr, &dstrect);
    }

    fn copy_player(this Renderer const& self, const Texture texture, const Vector2f pos /*Vecotr2 src*/) noexcept -> void {
        let dstrect{
            SDL_Rect{
            .x = static_cast<i32>(pos.x),
            .y = static_cast<i32>(pos.y),
            .w = PLayer::WIDTH,
            .h = PLayer::HEIGHT,
            },
        };
        (void)SDL_RenderCopy(self.raw_renderer, texture.raw_texure, nullptr, &dstrect);
    }

    fn copy(this Renderer const& self, const Texture texture) noexcept -> void {
        (void)SDL_RenderCopy(self.raw_renderer, texture.raw_texure, nullptr, nullptr);
    }

    fn present(this Renderer const& self) noexcept -> void {
        SDL_RenderPresent(self.raw_renderer);
    }

    /// Destructor: `Texture::deinit()`
    fn load_texture_from_file(this Renderer const& self, char const* const filename)->std::expected<Texture, Errors::IMGLoadTextureErr> {
        let raw{ IMG_LoadTexture(self.raw_renderer, filename) };
        if (raw == nullptr) {
            return std::unexpected(Errors::IMGLoadTextureErr::SDLIMGLoadTextureErr);
        }

        return Texture{
            .raw_texure = raw,
        };
    }
};

namespace {

    fn start_screen(Renderer& corr, GameStatePersistent& game) noexcept -> void {
        bool is_running_screen{ true };

        auto fps{ FpsCap(60) };

        let textur{ corr.load_texture_from_file("./assets/any key.png") };
        if (not textur) {
            printf("%s\n", SDL_GetError());
            printf("failed to load texutre\n");
            // is_running_screen = false;
            game.is_game_running = false;
            return;
        }


        while (is_running_screen) {
            fps.start();

            for (event::Event_t e{}; event::poll_event(&e);) {
                switch (e.type) {
                    case event::Quit: {
                        printf("quiting game\n");
                        is_running_screen = false;
                        game.is_game_running = false;
                    } break;
                    case event::KeyDown: {
                        printf("key down pressed. Exiting start screen\n");
                        // e.key.keysym.scancode
                        is_running_screen = false;
                        game.curent_screen = ScreensID::FirstLevel;
                    } break;

                    default:
                        break;
                }
            }

            {
                // shit render core
                // SDL_SetRenderDrawColor(corr.raw_renderer, 255, 0, 0, 255);
                (void)SDL_RenderClear(corr.raw_renderer);


                corr.copy(*textur);

                corr.present();
            }


            fps.end();
        }
    }


    /// has no `noexept` it will return a std::expecter in the future.
    /// `GameStatePersistent::deinit()` will save the data in the future.
    fn level_one_screen(Renderer& corr, GameStatePersistent& game)->void {
        bool is_running_screen{ true };

        auto fps{ FpsCap(60) };
        SDL_SetRenderDrawColor(corr.raw_renderer, 0, 0, 0, 255);

        let pla_tex{ corr.load_texture_from_file("./assets/fish_idle_0.png") };
        if (not pla_tex) {
            printf("%s\n", SDL_GetError());
            printf("failed to load texutre\n");
            // is_running_screen = false;
            game.is_game_running = false;
            return;
        }


        auto map_pos{ Vector2f::zero() };
        let map_tex{ corr.load_texture_from_file("./assets/map.png") };
        if (not map_tex) {
            printf("%s\n", SDL_GetError());
            printf("failed to load texutre\n");
            // is_running_screen = false;
            game.is_game_running = false;
            return;
        }

        let keyboard{ KeyboardState::init() };
        i32 mouse_x{};
        i32 mouse_y{};


        while (is_running_screen) {
            fps.start();
            for (event::Event_t e{}; event::poll_event(&e);) {
                switch (e.type) {
                    case event::Quit: {
                        printf("quiting game\n");

                        is_running_screen = false;
                        game.is_game_running = false;
                    } break;
                        // case event::KeyDown: {
                        // }

                    case event::MouseMotion: {
                        mouse_x = e.motion.x;
                        mouse_y = e.motion.y;

                    } break;

                    default:
                        break;
                }
            }
            if (keyboard.is_scancode_pressed(SDL_SCANCODE_A)) {
                game.pla.position.x -= PLayer::SPEED * fps.dt;
            }
            if (keyboard.is_scancode_pressed(SDL_SCANCODE_D)) {
                game.pla.position.x += PLayer::SPEED * fps.dt;
            }
            if (keyboard.is_scancode_pressed(SDL_SCANCODE_W)) {
                game.pla.position.y -= PLayer::SPEED * fps.dt;
            }
            if (keyboard.is_scancode_pressed(SDL_SCANCODE_S)) {
                game.pla.position.y += PLayer::SPEED * fps.dt;
            }


            printf("pla {x %f , y %f} map {x %f, y %f}\n", game.pla.position.x, game.pla.position.y, map_pos.x, map_pos.y);

            game.camera.folow_player(game.pla.position, map_pos, mouse_x, mouse_y);

            {
                // shit render code
                (void)SDL_RenderClear(corr.raw_renderer);
                corr.copy_with_size(*map_tex, map_pos, 800, 800);
                corr.copy_player(*pla_tex, game.pla.position);

                corr.present();
            }
            fps.end();
        }
    }
} // namespace

} // namespace CORE


int main() {
    defer(printf("%s\n", SDL_GetError()));
    auto corr_ok{ CORE::Renderer::init("Tile Slayer") };
    if (not corr_ok) {
        PrintAndRerunErr(corr_ok.error());
    }
    defer((*corr_ok).deinit());

    let ok{ CORE::Renderer::init_image() };
    if (not ok) {
        PrintAndRerunErr(corr_ok.error());
    }


    auto game{ CORE::GameStatePersistent::init() };

    auto rendr_infot{ SDL_RendererInfo() };
    let out{ SDL_GetRendererInfo(corr_ok->raw_renderer, &rendr_infot) };
    printf("%d = %s\n", out  , rendr_infot.name );


    // fmt::formatter<const GLubyte*> a ;

    while (game.is_game_running) {
        switch (game.curent_screen) {
            case CORE::ScreensID::StartMenu: {
                CORE::start_screen(*corr_ok, game);
            } break;
            case CORE::ScreensID::FirstLevel: {
                CORE::level_one_screen(*corr_ok, game);
            } break;
        }
    }
}
