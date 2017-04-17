#ifndef RECTS_H
#define RECTS_H
#include <SDL2/SDL.h>

namespace RECTS
{
	namespace PLAYER
	{
		namespace UP
		{
			const SDL_Rect stand = {116,0,29,64};
			namespace WALK
			{
				const SDL_Rect r1 = {87,0,29,63};
				const SDL_Rect r2 = {145,0,29,62};
			}
		}
		namespace DOWN
		{
			const SDL_Rect stand = {29,0,29,64};
			namespace WALK
			{
				const SDL_Rect r1 = {0,0,29,61};
				const SDL_Rect r2 = {58,0,29,62};
			}
		}
		namespace RIGHT
		{
			const SDL_Rect stand = {0,61,19,64};
			namespace WALK
			{
				const SDL_Rect r1 = {250,0,27,66};
				const SDL_Rect r2 = {19,64,30,64};
			}
		}
		namespace LEFT
		{
			const SDL_Rect stand = {204,0,19,64};
			namespace WALK
			{
				const SDL_Rect r1 = {174,0,30,64};
				const SDL_Rect r2 = {223,0,27,66};
			}
		}
	}
}

#endif
