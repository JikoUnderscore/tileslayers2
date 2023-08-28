#pragma once
#include <SDL2/SDL.h>
#undef main

#include "./../defines.h"

#include <span>


namespace CORE {


struct KeyboardState {
    std::span<const u8> state;

    static_fn init() noexcept -> KeyboardState;

    fn is_scancode_pressed(this KeyboardState const& self, const SDL_Scancode sc) noexcept -> bool;
};

struct Vector2f {
    f32 x;
    f32 y;
    explicit Vector2f();

    static_fn zero()->Vector2f;

    static_fn init(f32 x, f32 y) noexcept -> Vector2f;

    explicit Vector2f(f32 x, f32 y);
};


struct FpsCap {
    u32 m_frame_delay;
    f32 dt;
    u64 m_last_time;


    static_fn init(u32 fps_cap) noexcept -> FpsCap;

    fn start(this FpsCap& self) noexcept -> void;

    fn end(this FpsCap const& self) noexcept -> void;
};

struct PLayer {
    Vector2f position;

    static const constexpr i32 WIDTH = 60;
    static const constexpr i32 HEIGHT = 60;
    static const constexpr f32 SPEED = 200.0F;
    static_assert(WIDTH == 60);
    static_assert(HEIGHT == 60);
    static_assert(SPEED == 200.0F);


    static_fn init(f32 x, f32 y) noexcept -> PLayer;

    fn center_x(this PLayer const& self) noexcept -> f32;
    fn center_y(this PLayer const& self) noexcept -> f32;
};


struct Timer {
    f32 frenclency_ms;
    f32 end_time_ms;

    Timer();
    Timer(const f32 frenclency_ms, const f32 end_time_ms);

    fn update_nonstoper(this Timer& self, f32 dt) noexcept -> void;

    fn update(this Timer& self, f32 dt) noexcept -> void;
};


struct Bullet {
    Vector2f pos;
    f32 dx;
    f32 dy;
    Timer timer;

    static const constexpr i32 SIZE = 20;

    Bullet();
    Bullet(f32 x, f32 y, f32 dx, f32 dy);


    static_fn init(f32 x, f32 y, f32 dx, f32 dy) noexcept -> Bullet;
};


struct Camera {
    Vector2f offset_float;

    static const constexpr i32 MOUSE_MOVE_LIMIT_X = 360;
    static const constexpr i32 MOUSE_MOVE_LIMIT_Y = 250;

    fn folow_player(this Camera& self, Vector2f& player_pos, Vector2f& map_pos, const i32 mouse_x, const i32 mouse_y) noexcept -> void;
};


} // namespace CORE
