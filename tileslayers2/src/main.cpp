#include <SDL2/SDL.h>
#undef main
#include "defines.h"


#include "./engine/render.h"
#include "./game/screens.h"
#include <iostream>

#define PrintAndRerunErr(err)                                                                                                                        \
    auto e_er{ static_cast<Errors::ALLERRORS>((err)) };                                                                                              \
    std::cerr << e_er << '\n';                                                                                                                       \
    return e_er




int main() {
    defer(printf("%s\n", SDL_GetError()));
    auto corr_ok{ CORE::Renderer::init("Tile Slayer") };
    if (not corr_ok) {
        PrintAndRerunErr(corr_ok.error());
    }
    defer((*corr_ok).deinit());

    let renderer_ok{ CORE::Renderer::init_image() };
    if (not renderer_ok) {
        PrintAndRerunErr(renderer_ok.error());
    }


    auto game{ CORE::GameStatePersistent::init() };

    // auto rendr_infot{ SDL_RendererInfo() };
    // let out{ SDL_GetRendererInfo(corr_ok->raw_renderer, &rendr_infot) };
    // printf("%d = %s\n", out, rendr_infot.name);


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
