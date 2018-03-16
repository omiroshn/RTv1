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

	//* to parse	
	m->geom.O.x = 0;
	m->geom.O.y = 2;
	m->geom.O.z = -5;
	m->geom.camera_rot.x = deg_to_rad(15);
	m->geom.camera_rot.y = deg_to_rad(0);
	m->geom.camera_rot.z = deg_to_rad(0);

	m->geom.obj = malloc(sizeof(t_object) * m->geom.objects);
	m->geom.light = malloc(sizeof(t_object) * m->geom.lights);

	m->geom.obj[0].name = SPHERE;
	m->geom.obj[0].center = (t_vec3){0, 0.99, 2};
	m->geom.obj[0].color = (t_vec3){255, 0, 0};
	m->geom.obj[0].specular = 50;
	m->geom.obj[0].radius = 1;
	m->geom.obj[0].reflection = 0.3;
	
	m->geom.obj[1].name = SPHERE;
	m->geom.obj[1].center = (t_vec3){-2, 0.99, 3};
	m->geom.obj[1].color = (t_vec3){0, 0, 255};
	m->geom.obj[1].specular = 50;
	m->geom.obj[1].radius = 1;
	m->geom.obj[1].reflection = 0.3;
	
	m->geom.obj[2].name = SPHERE;
	m->geom.obj[2].center = (t_vec3){2, 0.99, 3};
	m->geom.obj[2].color = (t_vec3){0, 255, 0};
	m->geom.obj[2].specular = 50;
	m->geom.obj[2].radius = 1;
	m->geom.obj[2].reflection = 0.3;
	
	m->geom.obj[3].name = CONE;
	m->geom.obj[3].center = (t_vec3){-2, 1, 0};
	m->geom.obj[3].direction = (t_vec3){-2, 0, 0};
	m->geom.obj[3].color = (t_vec3){0, 0, 255};
	m->geom.obj[3].specular = 1000;
	m->geom.obj[3].radius = 1;
	m->geom.obj[3].reflection = 0.3;
	m->geom.obj[3].angle = 30;

	m->geom.obj[4].name = CYLINDER;
	m->geom.obj[4].center = (t_vec3){2, 0, 0};
	m->geom.obj[4].direction = (t_vec3){2, 1, 0};
	m->geom.obj[4].color = (t_vec3){0, 255, 0};
	m->geom.obj[4].specular = 1000;
	m->geom.obj[4].radius = 0.5;
	m->geom.obj[4].reflection = 0.3;

	m->geom.obj[5].name = PLANE;
	m->geom.obj[5].center = (t_vec3){0, 0, 0};
	m->geom.obj[5].color = (t_vec3){255, 255, 255};
	m->geom.obj[5].direction = (t_vec3){0, 1, 0};
	m->geom.obj[5].specular = 1000;
	m->geom.obj[5].reflection = 0.5;

	m->geom.light[0].type = AMBIENT;
	m->geom.light[0].intensity = 0.2;
	m->geom.light[0].position = (t_vec3){0, 0, 0};
	m->geom.light[0].direction = (t_vec3){0, 0, 0};

	m->geom.light[1].type = POINT;
	m->geom.light[1].intensity = 0.6;
	m->geom.light[1].position = (t_vec3){2, 0, 0.5};
	m->geom.light[1].direction = (t_vec3){0, 0, 0};

	m->geom.light[2].type = DIRECTIONAL;
	m->geom.light[2].intensity = 0.2;
	m->geom.light[2].position = (t_vec3){0, 0, 0};
	m->geom.light[2].direction = (t_vec3){1, 4, 4};

	m->geom.light[3].type = POINT;
	m->geom.light[3].intensity = 0.6;
	m->geom.light[3].position = (t_vec3){-2, 1, 0.5};
	m->geom.light[3].direction = (t_vec3){0, 0, 0};
	//*

	//*viewport
	m->vp.w = 1.0f;
	m->vp.h = 1.0f;
	m->vp.dist = 1.0f;
	//*

	m->geom.t_min = 0.001f;
	m->geom.t_max = INFINITY;
}
