#pragma once
#include <vector>

#include "./../engine/render.h"
#include "./../engine/events.h"
#include "./../engine/wrappers.h"
#include "./../defines.h"



namespace CORE {
enum class ScreensID {
    StartMenu,
    FirstLevel,
};

struct GameStatePersistent {
    bool is_game_running;
    ScreensID curent_screen;
    PLayer pla;
    Camera camera;

    static_fn init() noexcept -> GameStatePersistent ;
};




struct TileFloorRAII {
    Vec<Vector2f> tiles;
    Texture tex_dark;
    Texture tex_light;


    static const constexpr usize W = 80;
    static const constexpr usize H = 80;
    static const constexpr usize N_OF_TILES = 20;



    static_fn init(Renderer const& corr) noexcept -> TileFloorRAII ;

    ~TileFloorRAII() noexcept ;
};





fn start_screen(Renderer& corr, GameStatePersistent& game) noexcept -> void ;


fn level_one_screen(Renderer& corr, GameStatePersistent& game) -> void ;
} // namespace CORE
