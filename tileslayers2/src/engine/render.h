#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#undef main

#include <expected>

#include "./../game/errors.h"
#include "./../game/core.h"
#include "./../defines.h"

namespace CORE {

inline const constexpr i32 LOGICAL_RES_1280 = 1280;
inline const constexpr i32 LOGICAL_RES_720 = 720;






struct Texture {
    SDL_Texture* raw_texure;

    fn deinit(this Texture const& self) noexcept -> void ;
};








struct Renderer {
    SDL_Window* raw_window;
    SDL_Renderer* raw_renderer;

    /// Destructor `Renderer::deinit()`
    static_fn init(char const* const title)->std::expected<Renderer, Errors::RendererInitErr>;

    /// Destructor `Renderer::deinit()`
    static_fn init_image()->std::expected<void, Errors::IMGInitErr>;

    fn deinit(this Renderer const& self) noexcept -> void;

    fn copy_with_size(this Renderer const& self, const Texture texture, const Vector2f pos, const i32 width, const i32 height) noexcept -> void;

    fn copy_player(this Renderer const& self, const Texture texture, const Vector2f pos /*Vecotr2 src*/) noexcept -> void ;

    fn copy(this Renderer const& self, const Texture texture) noexcept -> void ;

    fn present(this Renderer const& self) noexcept -> void ;

    /// Destructor: `Texture::deinit()`
    fn load_texture_from_file(this Renderer const& self, char const* const filename)->std::expected<Texture, Errors::IMGLoadTextureErr> ;
};


} // namespace CORE