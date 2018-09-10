#pragma once

#include "Backend.h"

namespace magma {

	struct Char {
		int c;
		uint32_t fg;
		uint32_t bg;
	};

	class MemoryBackend : public Backend {
	public:
		static MemoryBackend* MemoryBackend::init(int width, int height);
		void putc(int x, int y, int c);
		void sub_putc(int pixelX, int pixelY, int c);
		void print(int x, int y, std::string str);
		void sub_print(int pixelX, int pixelY, std::string str);
		void set_fg(int r, int g, int b);
		void set_bg(int r, int g, int b);
		void refresh();
		void blit(Backend* src, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY);
		virtual BackendType get_type() { return BackendType::MEMORY; }
	protected:
		MemoryBackend(int width, int height);
		uint32_t mFgColour;
		uint32_t mBgColour;
		std::vector<Char> mConsole;
		friend class MemoryBackend;
	};

}