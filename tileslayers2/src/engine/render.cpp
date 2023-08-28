#include "render.h"


namespace CORE {


fn Texture::deinit(this Texture const& self) noexcept -> void {
    SDL_DestroyTexture(self.raw_texure);
}


/// Destructor `Renderer::deinit()`
fn Renderer::init(char const* const title)->std::expected<Renderer, Errors::RendererInitErr> {
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
fn Renderer::init_image()->std::expected<void, Errors::IMGInitErr> {
    constexpr let flags{ IMG_INIT_PNG };

    let init_flags{ IMG_Init(flags) };
    if ((init_flags & flags) != flags) {
        return std::unexpected(Errors::IMGInitErr::SDLIMGInitErr);
    }

    return {};
}

fn Renderer::deinit(this Renderer const& self) noexcept -> void {
    SDL_DestroyRenderer(self.raw_renderer);
    SDL_DestroyWindow(self.raw_window);
    IMG_Quit();
    SDL_Quit();
}

fn Renderer::copy_with_size(this Renderer const& self, const Texture texture, const Vector2f pos, const i32 width, const i32 height) noexcept
-> void {
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

fn Renderer::copy_player(this Renderer const& self, const Texture texture, const Vector2f pos /*Vecotr2 src*/) noexcept -> void {
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





// maybe i dont need src paramiter
fn Renderer::copy(this Renderer const& self, const Texture texture, const Vector2f pos, const Vector2f src)  noexcept -> void {
    let dstrect{
        SDL_Rect{
        .x = static_cast<i32>(pos.x),
        .y = static_cast<i32>(pos.y),
        .w = Bullet::SIZE,
        .h = Bullet::SIZE,
        },
    };

    let srcrect{
        SDL_Rect{
        .x = static_cast<i32>(src.x),
        .y = static_cast<i32>(src.y),
        .w = Bullet::SIZE,
        .h = Bullet::SIZE,
        },
    };



    (void)SDL_RenderCopy(self.raw_renderer, texture.raw_texure, &srcrect, &dstrect);
}







fn Renderer::copy(this Renderer const& self, const Texture texture) noexcept -> void {
    (void)SDL_RenderCopy(self.raw_renderer, texture.raw_texure, nullptr, nullptr);
}

fn Renderer::present(this Renderer const& self) noexcept -> void {
    SDL_RenderPresent(self.raw_renderer);
}

/// Destructor: `Texture::deinit()`
fn Renderer::load_texture_from_file(this Renderer const& self, char const* const filename)->std::expected<Texture, Errors::IMGLoadTextureErr> {
    let raw{ IMG_LoadTexture(self.raw_renderer, filename) };
    if (raw == nullptr) {
        return std::unexpected(Errors::IMGLoadTextureErr::SDLIMGLoadTextureErr);
    }

    return Texture{
        .raw_texure = raw,
    };
}


} // namespace CORE