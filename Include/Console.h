#pragma once
#include "Magma.h"
#include <string>

#define USE_SDL

namespace magma {

class Backend;

	MAGMA_DECL bool init();

	class MAGMA_DECL Console
	{
	public:
		static Console* init_root(int width, int height, std::string font, int tile_size);
		static Console* init(int width, int height);
		static void destroy(Console* con);
		void putc(int x, int y, int c);
		void sub_putc(int pixelX, int pixelY, int c);
		void print(int x, int y, std::string str);
		void sub_print(int pixelX, int pixelY, std::string str);
		void set_fg(int r, int g, int b);
		void set_bg(int r, int g, int b);
		void refresh();
		void clear();
		void blit(Console* src, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY);
	private:
		Console(int width, int height, std::string font, int tile_size);
		Console(int width, int height);
		Backend* con;
	};

}
