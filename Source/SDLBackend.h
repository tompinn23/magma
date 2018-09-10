#pragma once

#include "Backend.h"

#include <iostream>
#include <vector>
#include <cinttypes>
#include <stdexcept>

#include "SDL.h"

#define MAX_LETTER_CODE 255

namespace magma {
	struct SDLTexDeleter {
		void operator() (SDL_Texture* tex) const {
			SDL_DestroyTexture(tex);
		}
	};

	class SDLBackend : public Backend {
	public:
		static SDLBackend* root;
		static SDLBackend* init_root(int width, int height, std::string font, int tile_size);
		static SDLBackend* init(int width, int height);
		int get_width() { return mWidth; }
		int get_height() { return mHeight; }
		void putc(int x, int y, int c);
		void sub_putc(int pixelX, int pixelY, int c);
		void print(int x, int y, std::string str);
		void sub_print(int pixelX, int pixelY, std::string str);
		void set_fg(int r, int g, int b);
		void set_bg(int r, int g, int b);
		void refresh();
		void clear();
		void blit(Backend* src, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY);
		virtual BackendType get_type() { return BackendType::SDL; }
	protected:
		SDLBackend(int width, int height, std::string font, int tile_size);
		SDLBackend(int width, int height);
		std::string mFontFile = "";
		int mTileSize = 0;
		std::vector<SDL_Rect> mFontClips;
		std::shared_ptr<SDL_Window> mWindow;
		std::shared_ptr<SDL_Renderer> mRenderer;
		std::shared_ptr<SDL_Texture> mFont;
		std::unique_ptr<SDL_Texture, SDLTexDeleter> mConsole;
	};
}
