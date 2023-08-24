#pragma once

#include <iostream>
#include "./../defines.h"

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


inline auto operator<<(std::ostream& out, const ALLERRORS value) -> std::ostream& {
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
