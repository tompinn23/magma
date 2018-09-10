#pragma once

#include "Error.h"

namespace magma {

	enum class BackendType {
		ABSTRACT,
		MEMORY,
		SDL
	};

	class MAGMA_DECL Backend {
		public:
			Backend();
			virtual	void putc(int x, int y, int c) = 0;
			virtual void sub_putc(int pixelX, int pixelY, int c) = 0;
			virtual void print(int x, int y, std::string str) = 0;
			virtual void sub_print(int pixelX, int pixelY, std::string str) = 0;
			virtual void set_fg(int r, int g, int b) = 0;
			virtual void set_bg(int r, int g, int b) = 0;
			virtual void refresh() = 0;
			virtual void clear() = 0;
			virtual void blit(Backend* src, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY) = 0;
			virtual BackendType get_type() { return BackendType::ABSTRACT; }
		protected:
			int mWidth;
			int mHeight;

	};
}
