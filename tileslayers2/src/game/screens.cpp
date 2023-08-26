#include "screens.h"

#include "./../engine/render.h"
#include "./../engine/events.h"
#include "./../defines.h"


namespace CORE {
fn GameStatePersistent::init() noexcept -> GameStatePersistent {
    return GameStatePersistent{
        .is_game_running = true,
        .curent_screen = ScreensID::StartMenu,
        .pla = PLayer::init(0, 0),
        .camera = Camera{ .offset_float = Vector2f::zero() },

    };
}


TileFloorRAII::~TileFloorRAII() noexcept {
    this->tex_dark.deinit();
}


fn TileFloorRAII::init(Renderer const& corr) noexcept -> TileFloorRAII {
    auto v{ std::vector<Vector2f>() };
    v.reserve(N_OF_TILES * N_OF_TILES);

    for (size_t y = 0; y < N_OF_TILES; y++) {

        for (size_t x = 0; x < N_OF_TILES; x++) {
            v.push_back(Vector2f::init(f32(x * W), f32(y * H)));
        }
    }
    let tex{ *corr.load_texture_from_file("./assets/woodbing.png") };
    let tex2{ *corr.load_texture_from_file("./assets/woodbinginverted.png") };


    return TileFloorRAII{
        .tiles = std::move(v),
        .tex_dark = tex,
        .tex_light = tex2,
    };
}






}
