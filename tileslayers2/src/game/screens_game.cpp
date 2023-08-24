#include "screens.h"

namespace CORE {


fn GameStatePersistent::init() noexcept -> GameStatePersistent {
    return GameStatePersistent{
        .is_game_running = true,
        .curent_screen = ScreensID::StartMenu,
        .pla = PLayer::init(0, 0),
        .camera = Camera{ .offset_float = Vector2f::zero() },

    };
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

TileFloorRAII::~TileFloorRAII() noexcept {
    this->tex_dark.deinit();
}


/// has no `noexept` it will return a std::expecter in the future.
/// `GameStatePersistent::deinit()` will save the data in the future.
fn level_one_screen(Renderer& corr, GameStatePersistent& game)->void {
    bool is_running_screen{ true };

    auto fps{ FpsCap::init(60) };
    SDL_SetRenderDrawColor(corr.raw_renderer, 30, 30, 30, 255);

    let pla_tex{ corr.load_texture_from_file("./assets/fish_idle_0.png") };
    if (not pla_tex) {
        printf("%s\n", SDL_GetError());
        printf("failed to load texutre\n");
        // is_running_screen = false;
        game.is_game_running = false;
        return;
    }


    auto map_pos{ Vector2f::zero() };
    // let map_tex{ corr.load_texture_from_file("./assets/map.png") };
    // if (not map_tex) {
    //     printf("%s\n", SDL_GetError());
    //     printf("failed to load texutre\n");
    //     // is_running_screen = false;
    //     game.is_game_running = false;
    //     return;
    // }


    auto tilefloor{ TileFloorRAII::init(corr) };


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

        // printf("pla {x %f , y %f} map {x %f, y %f}\n", game.pla.position.x, game.pla.position.y, map_pos.x, map_pos.y);

        game.camera.folow_player(game.pla.position, map_pos, mouse_x, mouse_y);

        {
            // shit render code
            (void)SDL_RenderClear(corr.raw_renderer);


            usize y{ 0 };
            usize x{ 0 };


            for (usize i{ 0 }; i < tilefloor.tiles.size(); ++i) {
                tilefloor.tiles[i].x += -game.camera.offset_float.x;
                tilefloor.tiles[i].y += -game.camera.offset_float.y;

                // printf("%d tilefloor.tiles.size() %d\n", i * TileFloor::H, tilefloor.tiles.size());
                ++x;
                if (x >= TileFloorRAII::N_OF_TILES) {
                    x = 0;
                    ++y;
                }


                let src{
                    SDL_Rect{
                    .x = static_cast<i32>(x * TileFloorRAII::W),
                    .y = static_cast<i32>(y * TileFloorRAII::H),
                    .w = TileFloorRAII::W,
                    .h = TileFloorRAII::H,
                    },
                };

                let dest{
                    SDL_Rect{
                    .x = static_cast<i32>(tilefloor.tiles[i].x),
                    .y = static_cast<i32>(tilefloor.tiles[i].y),
                    .w = TileFloorRAII::W,
                    .h = TileFloorRAII::H,
                    },
                };

                (void)SDL_RenderCopy(corr.raw_renderer, tilefloor.tex_dark.raw_texure, &src, &dest);
            }

            // corr.copy_with_size(*map_tex, map_pos, 800, 800);
            corr.copy_player(*pla_tex, game.pla.position);

            corr.present();
        }
        fps.end();
    }
}


} // namespace CORE
