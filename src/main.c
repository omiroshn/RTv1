/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/07 18:50:22 by omiroshn          #+#    #+#             */
/*   Updated: 2018/02/07 18:50:24 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	key_arrows(t_map *map, SDL_Scancode scancode)
{
	if (scancode == SDL_SCANCODE_RIGHT)
		map->geom.o.x += 0.1;
	if (scancode == SDL_SCANCODE_LEFT)
		map->geom.o.x -= 0.1;
	if (scancode == SDL_SCANCODE_UP)
		map->geom.o.z += 0.1;
	if (scancode == SDL_SCANCODE_DOWN)
		map->geom.o.z -= 0.1;
}

static void	key_keyboard(t_map *map, SDL_Scancode scancode)
{
	if (scancode == SDL_SCANCODE_S)
		map->geom.camera_rot.x += 0.04;
	if (scancode == SDL_SCANCODE_W)
		map->geom.camera_rot.x -= 0.04;
	if (scancode == SDL_SCANCODE_D)
		map->geom.camera_rot.y += 0.04;
	if (scancode == SDL_SCANCODE_A)
		map->geom.camera_rot.y -= 0.04;
	if (scancode == SDL_SCANCODE_Q)
		map->geom.o.y += 0.1;
	if (scancode == SDL_SCANCODE_E)
		map->geom.o.y -= 0.1;
}

int			key_function(t_map *map)
{
	SDL_Event	e;

	while (SDL_PollEvent(&e))
	{
		if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN
			&& e.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
			return (0);
		else if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.scancode >= 79 && e.key.keysym.scancode <= 82)
				key_arrows(map, e.key.keysym.scancode);
			else
				key_keyboard(map, e.key.keysym.scancode);
		}
		else if (e.type == SDL_KEYUP)
		{
			if (e.key.keysym.scancode == SDL_SCANCODE_R)
			{
				map->geom.camera_rot.x = 0;
				map->geom.camera_rot.y = 0;
			}
		}
	}
	return (1);
}

int			main(int argc, char **argv)
{
	t_map	map;

	if (argc != 2)
		put_usage();
	read_map(&map, argv[1]);
	init(&map);
	init_geometry(&map);
	kernel_init(&map);
	while (TRUE)
	{
		if (!key_function(&map))
			break ;
		kernel(&map);
		display_fps(&map);
		lsync();
		SDL_UpdateWindowSurface(map.window);
	}
	return (quit(&map));
}
