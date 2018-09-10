#include "SDLBackend.h"

#include "fmt/format.h"
#include "lodepng.h"


namespace magma {
	SDLBackend* SDLBackend::root = NULL;

	SDLBackend* SDLBackend::init_root(int width, int height, std::string font, int tile_size)
	{
		if (root == NULL)
		{
			try {
				root = new SDLBackend(width, height, font, tile_size);
			}
			catch (const std::runtime_error &error)
			{
				set_error(fmt::format("Failed to create root window: {}", error.what()));
			}
		}
		return root;
	}

	SDLBackend* SDLBackend::init(int width, int height)
	{
		try {
			SDLBackend* con = new SDLBackend(width, height);
			return con;
		}
		catch (const std::runtime_error &error)
		{
			set_error(fmt::format("Failed to create console window: {}", error.what()));
		}
		return NULL;
	}

	SDLBackend::SDLBackend(int width, int height, std::string font, int tile_size)
	{
		mWidth = width;
		mHeight = height;
		mFontFile = font;
		mTileSize = tile_size;
		mWindow = std::shared_ptr<SDL_Window>(SDL_CreateWindow("rl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWidth * tile_size, mHeight * tile_size, SDL_WINDOW_SHOWN), SDL_DestroyWindow);
		if(mWindow == NULL)
		{
			throw std::runtime_error(fmt::format("SDL Window creation failed with error: {} ", SDL_GetError()));
		}
		mRenderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(mWindow.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE), SDL_DestroyRenderer);
		if(mRenderer == NULL)
		{
			throw std::runtime_error(fmt::format("SDL Renderer creation failed with error: {}", SDL_GetError()));
		}
		SDL_SetRenderDrawColor(mRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_SetRenderDrawBlendMode(mRenderer.get(), SDL_BLENDMODE_BLEND);
		SDL_Surface* image;
		unsigned error;
		std::vector<unsigned char> imgBuf;
		unsigned w, h, x, y;
		error = lodepng::decode(imgBuf, w, h, mFontFile);
		if (error)
		{
			throw std::runtime_error(fmt::format("Decoding errror: {} ", lodepng_error_text(error)));
		}
		//Transparent Colour
		uint8_t r_t, g_t, b_t, a_t;
		r_t = imgBuf[0];
		g_t = imgBuf[1];
		b_t = imgBuf[2];
		a_t = imgBuf[3];
		image = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);
		if (image == NULL)
		{
			throw std::runtime_error(fmt::format("Failed to create SDL surface: {}", SDL_GetError()));
		}
		for (y = 0; y < h; y++)
			for (x = 0; x < w; x++)
			{
				uint32_t* bufp;
				uint32_t r, g, b, a;
				/* get RGBA components */
				r = imgBuf[4 * y * w + 4 * x + 0]; // Red
				g = imgBuf[4 * y * w + 4 * x + 1]; // Green
				b = imgBuf[4 * y * w + 4 * x + 2]; // Blue
				a = imgBuf[4 * y * w + 4 * x + 3]; // Alpha
				/* If color == background then set as transparent. */
				if (r == r_t && g == g_t && b == b_t && a == a_t)
					a = 0;
				bufp = (Uint32 *)image->pixels + (y * image->pitch / 4) + x;
				*bufp = SDL_MapRGBA(image->format, r, g, b, a);
			}
		mFont = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(mRenderer.get(), image), SDL_DestroyTexture);
		SDL_SetTextureBlendMode(mFont.get(), SDL_BLENDMODE_BLEND);
		mFontClips.resize(MAX_LETTER_CODE + 1);
		int idx = 0;
		for(int j = 0; j < (w / tile_size); j++)
		for (int i = 0; i < (h / tile_size); i++)
		{
			int x = i * tile_size;
			int y = j * tile_size;
			SDL_Rect renderQuad = { x, y, tile_size, tile_size };
			mFontClips[idx] = renderQuad;
			idx++;
		}
		set_fg(255, 255, 255);
		set_bg(0, 0, 0);
	}



	SDLBackend::SDLBackend(int width, int height)
	{
		mWidth = width;
		mHeight = height;
		mRenderer = root->mRenderer;
		mTileSize = root->mTileSize;
		mFontClips = root->mFontClips;
		mFont = root->mFont;
		mConsole = std::unique_ptr<SDL_Texture, SDLTexDeleter>(SDL_CreateTexture(mRenderer.get(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, mWidth * mTileSize, mHeight * mTileSize));
		if (mConsole == NULL)
		{
			throw std::runtime_error(fmt::format("Console texture could not be created: {}", SDL_GetError()));
		}
	}

	void SDLBackend::putc(int x, int y, int c)
	{
		int i = x * mTileSize;
		int j = y * mTileSize;
		sub_putc(i, j, c);
	}

	void SDLBackend::sub_putc(int pixelX, int pixelY, int c)
	{
		if (mConsole == NULL)
		{
			SDL_SetRenderTarget(mRenderer.get(), NULL);
		}
		else {
			SDL_SetRenderTarget(mRenderer.get(), mConsole.get());
		}
		SDL_Rect renderQuad = { pixelX, pixelY, mTileSize, mTileSize };
		SDL_RenderFillRect(mRenderer.get(), &renderQuad);
		//printf("Index / Char: %d / %c", c);
		//printf("Source: X: %d Y: %d X2: %d Y2: %d", scr->font_clips[c].x, scr->font_clips[c].y, scr->font_clips[c].x + scr->tile_width, scr->font_clips[c].y + scr->tile_height);
		//printf("Destination: X: %d Y: %d X2: %d Y2: %d", i, j, i + scr->tile_width, j + scr->tile_height);
		if (c > MAX_LETTER_CODE)
		{
			c = MAX_LETTER_CODE;
		}
		SDL_RenderCopy(mRenderer.get(), mFont.get(), &mFontClips[c], &renderQuad);
	}

	void SDLBackend::print(int x, int y, std::string str)
	{
		for (int i = 0; i < str.length(); i++)
		{
			putc(x + i, y, str[i]);
		}
	}

	void SDLBackend::sub_print(int pixelX, int pixelY, std::string str)
	{
		for (int i = 0; i < str.length(); i++)
		{
			sub_putc(pixelX + i, pixelY, str[i]);
		}
	}

	void SDLBackend::set_bg(int r, int g, int b)
	{
		SDL_SetRenderDrawColor(mRenderer.get(), r, g, b, 255);
	}

	void SDLBackend::set_fg(int r, int g, int b)
	{
		SDL_SetTextureColorMod(mFont.get(), r, g, b);
	}

	void SDLBackend::refresh()
	{
		SDL_RenderPresent(mRenderer.get());
		//SDL_RenderClear(mRenderer.get());
	}

	void SDLBackend::clear()
	{
		if (mConsole == NULL)
		{
			SDL_SetRenderTarget(mRenderer.get(), NULL);
		}
		else {
			SDL_SetRenderTarget(mRenderer.get(), mConsole.get());
		}
		SDL_RenderClear(mRenderer.get());
	}

	void SDLBackend::blit(Backend* src, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY)
	{
		if (src->get_type() == BackendType::SDL)
		{
			SDLBackend* sdl_src = dynamic_cast<SDLBackend*>(src);
			if (mConsole == NULL)
			{
				SDL_SetRenderTarget(mRenderer.get(), NULL);
			}
			else {
				SDL_SetRenderTarget(mRenderer.get(), mConsole.get());
			}
			SDL_Rect srcQuad = { srcX, srcY, srcW * mTileSize, srcH * mTileSize };
			SDL_Rect dstQuad = { dstX * mTileSize, dstY * mTileSize, srcW * mTileSize, srcH * mTileSize };
			SDL_RenderCopy(mRenderer.get(), sdl_src->mConsole.get(), &srcQuad, &dstQuad);
		}
	}
}
