#include <SDL.h>
#include <yac8emul/yac8emul.h>

namespace yac8emul {
// Based off from
// https://stackoverflow.com/questions/29424877/couple-of-questions-about-sdl-window-and-unique-ptr
template <typename sdl_type> struct SDLDestroyer {
  void operator()(sdl_type *w) const {
    if constexpr (std::is_same_v<sdl_type, SDL_Window>) {
      std::cout << "using destroywindow" << std::endl;
      SDL_DestroyWindow(w);
    } else if constexpr (std::is_same_v<sdl_type, SDL_Renderer>) {
      std::cout << "using destroy_renderer" << std::endl;
      SDL_DestroyRenderer(w);
    } else {
      assert(false);
    }
  }
};

template <typename sdl_type>
using sdl_ptr = std::unique_ptr<sdl_type, SDLDestroyer<sdl_type>>;

class display {
public:
  display(){};
  void initialize_screen() {
    this->window = sdl_ptr<SDL_Window>(
        SDL_CreateWindow("test", 100, 100, 1024, 1024, SDL_WINDOW_SHOWN));
    this->renderer = sdl_ptr<SDL_Renderer>(
        SDL_CreateRenderer(this->window.get(), -1, SDL_RENDERER_ACCELERATED));
  }

private:
  sdl_ptr<SDL_Window> window;
  sdl_ptr<SDL_Renderer> renderer;
};
} // namespace yac8emul
