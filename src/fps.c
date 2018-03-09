/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 21:27:42 by omiroshn          #+#    #+#             */
/*   Updated: 2018/03/09 21:27:43 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	lsync(void)
{
	int					delay;
	static unsigned int	time;
	static char			vsync = 1;

	vsync == 1 ? SDL_GL_SetSwapInterval(1) : 0;
	if (vsync)
	{
		delay = 16 - (SDL_GetTicks() - time);
		if (delay < 0)
			SDL_Delay(0);
		else
			SDL_Delay(delay);
		time = SDL_GetTicks();
	}
}

void	display_fps(t_map *m)
{
	static t_uint	fps;
	static t_uint	time_current;
	static t_uint	time_past;
	static t_uint	ticks;
	TTF_Font		*ttf;
	static char		*fps_str;

	time_current = time(NULL);
	if (time_current - time_past && (time_past = time_current))
	{
		fps = 1000 / (SDL_GetTicks() - ticks);
		ft_memdel((void**)&fps_str);
		fps_str = ft_itoa(fps);
	}
	ticks = SDL_GetTicks();
	if (!(ttf = TTF_OpenFont(FONTS_FOLDER"arcadeclassic.regular.ttf", 50)))
		put_error(IMG_GetError());
	m->fps = TTF_RenderText_Solid(ttf, fps_str,
					(SDL_Color){255, 156, 127, 255});
	SDL_BlitSurface(m->fps, NULL, m->screen,
		&(SDL_Rect){ 10, 0, m->fps->w, m->fps->h});
	TTF_CloseFont(ttf);
	SDL_FreeSurface(m->fps);
}
