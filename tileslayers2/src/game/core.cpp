#include "core.h"
#include "./../engine/render.h"


namespace CORE {


fn KeyboardState::init() noexcept -> KeyboardState {
    int len{};
    let ptr{ SDL_GetKeyboardState(&len) };

    return KeyboardState{
        .state = std::span<const u8>(ptr, len),
    };
}

fn KeyboardState::is_scancode_pressed(this KeyboardState const& self, const SDL_Scancode sc) noexcept -> bool {
    return self.state[static_cast<usize>(sc)] != 0;
}

 Vector2f::Vector2f() : x(0.0), y(0.0) {
}

 fn Vector2f::zero()->Vector2f {
    return Vector2f();
}

 fn Vector2f::init(const f32 x, const f32 y) noexcept -> Vector2f {
    return Vector2f(x, y);
}

 Vector2f::Vector2f(const f32 x, const f32 y) : x(x), y(y) {
}


fn FpsCap::init(const u32 fps_cap) noexcept -> FpsCap {
    return FpsCap{
        .m_frame_delay = (1000 / fps_cap),
        .dt = 0.0F,
        .m_last_time = SDL_GetPerformanceCounter(),
    };
}

fn FpsCap::start(this FpsCap& self) noexcept -> void {
    const u64 now = SDL_GetPerformanceCounter();
    const u64 elapsed = now - self.m_last_time;
    self.dt = static_cast<f32>(elapsed) / static_cast<f32>(SDL_GetPerformanceFrequency());
    // printf("FPS: %f | dt %f\n", 1.0F / self.dt, self.dt);
    self.m_last_time = now;
}

fn FpsCap::end(this FpsCap const& self) noexcept -> void {
    const u64 now = SDL_GetPerformanceCounter();
    const u64 elapsed = now - self.m_last_time;
    const u32 cap_frame_end = static_cast<Uint32>((static_cast<f32>(elapsed) * 1000.0F) / static_cast<f32>(SDL_GetPerformanceFrequency()));

    if (cap_frame_end < self.m_frame_delay) {
        SDL_Delay((self.m_frame_delay - cap_frame_end));
    }
}


fn PLayer::init(const f32 x, const f32 y)->PLayer {
    return PLayer{
        .position = Vector2f::init(x, y),
    };
}


    fn Camera::folow_player(this Camera& self, Vector2f& player_pos, Vector2f& map_pos, const i32 mouse_x, const i32 mouse_y) noexcept -> void {
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





} // namespace CORE
