/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 23:12:44 by omiroshn          #+#    #+#             */
/*   Updated: 2018/03/09 23:12:45 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	init(t_map *m)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		put_error(SDL_GetError());
	if (TTF_Init() < 0)
		put_error(SDL_GetError());
	m->window = SDL_CreateWindow("RTv1",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
	if (!m->window)
		exit(1);
	m->screen = SDL_GetWindowSurface(m->window);
	m->image = m->screen->pixels;
}

void	init_geometry(t_map *m)
{
	m->flag = 0;

	//*camera
	m->geom.O.x = 0;
	m->geom.O.y = 0;
	m->geom.O.z = -5;
	m->geom.camera_rot.x = 0;
	m->geom.camera_rot.y = 0;
	m->geom.camera_rot.z = 0;
	//*

	//*viewport
	m->vp.w = 1.0f;
	m->vp.h = 1.0f;
	m->vp.dist = 1.0f;
	//*

	m->geom.t_min = 0.001f;
	m->geom.t_max = INFINITY;
}
