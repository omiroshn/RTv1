/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/02 18:10:14 by omiroshn          #+#    #+#             */
/*   Updated: 2018/03/02 18:10:16 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include <time.h>
# include <fcntl.h>
# include <SDL2/SDL.h>
# include <math.h>
# include "libft.h"
# include "SDL_ttf.h"
# include "SDL_image.h"
# include "SDL_mixer.h"
# include "opencl.h"

# define MAPS_FOLDER "resources/maps/"
# define FONTS_FOLDER "resources/fonts/"

# define WIDTH 1280
# define HEIGHT 1280
# define THREADS 8
# define TRUE 1
# define BACKGROUND_COLOR 0

# define AMBIENT 1
# define POINT 2
# define DIRECTIONAL 3
# define DEEP 1

typedef unsigned int	t_uint;

typedef	struct	s_vec2
{
	float		x;
	float		y;
}				t_vec2;

typedef	struct	s_vec3
{
	float		x;
	float		y;
	float		z;
	float		w;
	float		reflection;
}				t_vec3;

typedef struct	s_matrix4
{
	float		m[4][4];
}				t_matrix4;

typedef	struct	s_light
{
	int			type;
	t_vec3		direction;
	t_vec3		position;
	double		intensity;
}				t_light;

typedef	struct	s_sphere
{
	t_vec3		center;
	t_vec3		color;
	float		radius;
	float		specular;
	float		reflection;
}				t_sphere;

typedef	struct	s_obj
{
	t_sphere	sphere;
	t_light		light;
}				t_obj;

typedef struct	s_viewport
{
	float		w;
	float		h;
	float		dist;
}				t_viewport;

typedef	struct	s_traceray
{
	double		closest_t;
	t_sphere	closest_sphere;
	t_vec2		t;
}				t_traceray;

typedef	struct	s_geom
{
	t_vec3		O;
	t_vec3		D;
	t_vec3		camera_rot;
	int			color;
}				t_geom;

typedef	struct	s_map
{
	SDL_Window	*window;
	SDL_Surface	*screen;
	SDL_Surface *fps;
	
	t_uint		*image;
	t_uint		*bufp;
	t_obj		obj;
	t_geom		geom;
	t_traceray	tr;
	t_viewport	vp;
	int			flag;
}				t_map;

void	ClosestIntersection(t_traceray *tr, t_sphere *sphere, 
								t_vec3 O, t_vec3 D, float t_min, float t_max);

/*
**		init.c
*/

void	init_geometry(t_map *m);
void	init(t_map *m);

/*
**		fps.c
*/

void	display_fps(t_map *m);
void	lsync(void);

/*
**		put_error.c
*/

void	put_error(const char *msg);
void	put_usage(void);
int		quit(t_map *m);

#endif
