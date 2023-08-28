#include "./../engine/wrappers.h"
#include "core.h"
#include "screens.h"

#include <cmath>
#include <numbers>
#include <vector>

namespace CORE {


/// has no `noexept` it will return a std::expecter in the future.
/// `GameStatePersistent::deinit()` will save the data in the future.
fn level_one_screen(Renderer& corr, GameStatePersistent& game)->void {
    bool is_running_screen{ true };

    auto fps{ FpsCap::init(60) };
    (void)SDL_SetRenderDrawColor(corr.raw_renderer, 30, 30, 30, 255);

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
    auto bullets{ Vec<Bullet>() };
    auto bullets_cleanup{ Vec<usize>() };


    let keyboard{ KeyboardState::init() };
    i32 mouse_x{};
    i32 mouse_y{};
    static constexpr f32 RADIAN_90_DEGREES{ 90.0F * (f32(std::numbers::pi) / 180.0F) };


    while (is_running_screen) {
        fps.start();
        for (event::Event_t evnt{}; event::poll_event(&evnt);) {
            switch (evnt.type) {
                case event::Quit: {
                    printf("quiting game\n");

                    is_running_screen = false;
                    game.is_game_running = false;

                    goto skip_and_quit;
                } break;

                case event::MouseButtonDown: {
                    // auto a = e.button;
                } break;

                case event::KeyDown: {
                    switch (evnt.key.keysym.scancode) {
                        case SDL_Scancode::SDL_SCANCODE_SPACE: {
                            const f32 center_x = game.pla.center_x() - static_cast<f32>(Bullet::SIZE / 2);
                            const f32 center_y = game.pla.center_y() - static_cast<f32>(Bullet::SIZE / 2);


                            const f32 dx{ static_cast<f32>(mouse_x - (Bullet::SIZE / 2)) - center_x };
                            const f32 dy{ static_cast<f32>(mouse_y - (Bullet::SIZE / 2)) - center_y };

                            // const f32 radian{ std::atan2f(dy, dx) };
                            // // const f32 angle {(radian * 180.0F) / f32(std::numbers::pi)};

                            // const f32 cosfdx{ std::cosf(radian) };
                            // const f32 sinfdy{ std::sinf(radian) };
                            // // printf("radian %f cosfdx %f sinfdy %f angle %f\n", radian, cosfdx, sinfdy, angle);


                            // printf("dx %f dy %f\n", dx, dy);

                            if (dx != 0.0 and dy != 0.0) {
                                // Normalization
                                const f32 dist{ std::sqrtf((dx * dx) + (dy * dy)) };


                                let normal{ Vector2f::init(dx / dist, dy / dist) };

                                printf("normal_x %f normal_y %f dist %f\n", normal.x, normal.y, dist);

                                bullets.emplace_back(center_x, center_y, normal.x, normal.y);
                            }


                        } break;

                        default:
                            break;
                    }

                } break;

                case event::MouseMotion: {
                    mouse_x = evnt.motion.x;
                    mouse_y = evnt.motion.y;
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


            for (usize i = 0; i < bullets.size(); ++i) {
                bullets[i].pos.x += ((300.0F * bullets[i].dx) * fps.dt) - game.camera.offset_float.x;
                bullets[i].pos.y += ((300.0F * bullets[i].dy) * fps.dt) - game.camera.offset_float.y;
                bullets[i].timer.update(fps.dt);
                // printf("bullets[i].timer{ %f , %f}\n", bullets[i].timer.frenclency_ms, bullets[i].timer.end_time_ms);
                // printf("bullets[i].pos{ %f , %f}\n", bullets[i].pos.x, bullets[i].pos.y);
                if (bullets[i].timer.frenclency_ms == 0.0F) {
                    bullets_cleanup.push_back(i);
                }


                // printf("bullet {x %F y %F}\n", bullets[i].pos.x, bullets[i].pos.y);


                corr.copy(tilefloor.tex_light, bullets[i].pos, Vector2f::zero());
            }

            for (auto i : bullets_cleanup) {
                bullets.swap_remove(i);
            }
            if (not bullets_cleanup.empty()) {
                bullets_cleanup.clear();
            }


            corr.present();
        }

    skip_and_quit:;
        fps.end();
    }
}


} // namespace CORE
