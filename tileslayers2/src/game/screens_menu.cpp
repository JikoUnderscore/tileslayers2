#include "screens.h"


namespace CORE {

fn start_screen(Renderer& corr, GameStatePersistent& game) noexcept -> void {
    bool is_running_screen{ true };

    auto fps{ FpsCap::init(60) };

    let textur{ corr.load_texture_from_file("./assets/any key.png") };
    if (not textur) {
        printf("%s\n", SDL_GetError());
        printf("failed to load texutre\n");
        // is_running_screen = false;
        game.is_game_running = false;
        return;
    }


    while (is_running_screen) {
        fps.start();

        for (event::Event_t e{}; event::poll_event(&e);) {
            switch (e.type) {
                case event::Quit: {
                    printf("quiting game\n");
                    is_running_screen = false;
                    game.is_game_running = false;
                } break;
                case event::KeyDown: {
                    printf("key down pressed. Exiting start screen\n");
                    // e.key.keysym.scancode
                    is_running_screen = false;
                    game.curent_screen = ScreensID::FirstLevel;
                } break;

                default:
                    break;
            }
        }

        {
            // shit render core
            // SDL_SetRenderDrawColor(corr.raw_renderer, 255, 0, 0, 255);
            (void)SDL_RenderClear(corr.raw_renderer);


            corr.copy(*textur);

            corr.present();
        }


        fps.end();
    }
}


} // namespace CORE
