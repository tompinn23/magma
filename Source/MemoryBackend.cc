#include "MemoryBackend.h"

namespace magma {

	MemoryBackend * MemoryBackend::init(int width, int height)
	{
		return nullptr;
	}

	void MemoryBackend::putc(int x, int y, int c)
	{
		mConsole[y * width + x] = Char(c, mFgColour, mBgColour);
	}

	void MemoryBackend::sub_putc(int pixelX, int pixelY, int c)
	{
		//TODO: HUH.
	}

	void MemoryBackend::print(int x, int y, std::string str)
	{
		for (int i = 0; i < str.length(); i++)
		{
			putc(x + i, y, str[i]);
		}
	}

	void MemoryBackend::sub_print(int pixelX, int pixelY, std::string str)
	{
		//TODO: INVALID.
	}

	void MemoryBackend::set_fg(int r, int g, int b)
	{
		mFgColour = r << 16 | g << 8 | b;
	}

	void MemoryBackend::set_bg(int r, int g, int b)
	{
		mBgColour = r << 16 | g << 8 | b;
	}

	void MemoryBackend::refresh()
	{
		//Does Nothing. Uneeded for in memory console.
	}

	void MemoryBackend::blit(Backend * src, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY)
	{
		//TODO: Needs doing.
	}

	MemoryBackend::MemoryBackend(int width, int height)
	{
		mWidth = width;
		mHeight = height;
		mConsole.resize(width * height);
	}

}