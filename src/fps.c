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

static void	color_fps(t_map *m, t_uint fps, TTF_Font *ttf, char *fps_str)
{
	int r;
	int g;
	int b;
	
	if (fps >= 40)
	{
		r = 0;
		g = 255;
		b = 0;
	}
	else if (fps >= 20 && fps < 40)
	{
		r = 255;
		g = 156;
		b = 127;
	}
	else if (fps < 20)
	{
		r = 255;
		g = 0;
		b = 0;
	}
	m->fps = TTF_RenderText_Solid(ttf, fps_str,
					(SDL_Color){r, g, b, 255});
}

void	display_fps(t_map *m)
{
	static t_uint	fps;
	static t_uint	time_current;
	static t_uint	time_past;
	static t_uint	ticks;
	TTF_Font		*ttf;
	static char		*fps_str;
	t_vec3			rgb;

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
	color_fps(m, fps, ttf, fps_str);
	SDL_BlitSurface(m->fps, NULL, m->screen,
		&(SDL_Rect){ 10, 0, m->fps->w, m->fps->h});
	TTF_CloseFont(ttf);
	SDL_FreeSurface(m->fps);
}
