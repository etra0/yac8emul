#include <yac8emul/yac8emul.h>
#include <SDL.h>

// Based off from https://stackoverflow.com/questions/29424877/couple-of-questions-about-sdl-window-and-unique-ptr
template <typename sdl_type>
struct SDLDestroyer
{
    void operator()(sdl_type* w) const
    {
      if constexpr(std::is_same_v<sdl_type, SDL_Window>)
        SDL_DestroyWindow(w);
      else if constexpr(std::is_same_v<sdl_type, SDL_Renderer>)
        SDL_DestroyRenderer(w);
    }
};

class display {
  public:
    display() {};
  private:
    std::unique_ptr<SDL_Window, SDLDestroyer<SDL_Window>> window;
    std::unique_ptr<SDL_Renderer, SDLDestroyer<SDL_Renderer>> renderer;
};
