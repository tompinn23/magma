#include "Console.h"

#include "SDLBackend.h"
#include "SDL.h"
#include "fmt/format.h"
namespace magma {

	void quit_sdl()
	{
		SDL_QuitSubSystem(SDL_INIT_TIMER);
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		SDL_QuitSubSystem(SDL_INIT_EVENTS);
		SDL_Quit();
	}


	bool init()
	{
		
#if defined(USE_SDL)
		atexit(quit_sdl);
		if (SDL_Init(0) < 0)
		{
			set_error(fmt::format("Failed to initialize SDL: {}", SDL_GetError()));
			return false;
		}
		SDL_InitSubSystem(SDL_INIT_TIMER);
		SDL_InitSubSystem(SDL_INIT_VIDEO);
		SDL_InitSubSystem(SDL_INIT_EVENTS);
		return true;
#endif
	}



	Console * Console::init_root(int width, int height, std::string font, int tile_size)
	{
		auto console = new Console(width, height, font, tile_size);
		return console;
	}

	Console * Console::init(int width, int height)
	{
		auto console = new Console(width, height);
		return console;
	}

	void Console::destroy(Console* con)
	{
		delete con;
	}

	Console::Console(int width, int height, std::string font, int tile_size)
	{
#if defined(USE_SDL)
		con = SDLBackend::init_root(width, height, font, tile_size);
#endif
	}
	
	Console::Console(int width, int height)
	{
#if defined(USE_SDL)
		con = SDLBackend::init(width, height);
#endif
	}

	void Console::putc(int x, int y, int c)
	{
		con->putc(x, y, c);
	}

	void Console::sub_putc(int pixelX, int pixelY, int c)
	{
		con->sub_putc(pixelX, pixelY, c);
	}

	void Console::print(int x, int y, std::string str)
	{
		con->print(x, y, str);
	}

	void Console::sub_print(int pixelX, int pixelY, std::string str)
	{
		con->sub_print(pixelX, pixelY, str);
	}

	void Console::set_fg(int r, int g, int b)
	{
		con->set_fg(r, g, b);
	}

	void Console::set_bg(int r, int g, int b)
	{
		con->set_bg(r, g, b);
	}

	void Console::refresh()
	{
		con->refresh();
	}

	void Console::clear()
	{
		con->clear();
	}

	void Console::blit(Console* src, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY)
	{
		con->blit(src->con, srcX, srcY, srcW, srcH, dstX, dstY);
	}

}