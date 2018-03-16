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
#ifdef __APPLE__
# include "opencl.h"
# include "SDL_image.h"
# include "SDL_mixer.h"
# include "SDL_ttf.h"
# define WIDTH 1280
# define HEIGHT 1280
#elif __linux__
# include <SDL2/SDL_image.h>
# include <SDL2/SDL_mixer.h>
# include <SDL2/SDL_ttf.h>
# define WIDTH 1000
# define HEIGHT 1000
#endif

# define MAPS_FOLDER "resources/maps/"
# define FONTS_FOLDER "resources/fonts/"

# define THREADS 8
# define TRUE 1
# define BACKGROUND_COLOR 0

# define AMBIENT 1
# define POINT 2
# define DIRECTIONAL 3

# define SPHERE 1
# define PLANE 2
# define CYLINDER 3
# define CONE 4

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

typedef	struct	s_light
{
	int			type;
	double		intensity;
	t_vec3		position;
	t_vec3		direction;
}				t_light;

typedef	struct	s_object
{
	int			name;
	t_vec3		center;
	t_vec3		direction;
	t_vec3		color;
	float		specular;
	float		radius;
	float		reflection;
	float		angle;
}				t_object;

typedef struct	s_viewport
{
	float		w;
	float		h;
	float		dist;
}				t_viewport;

typedef	struct	s_traceray
{
	double		closest_t;
	t_object	closest_obj;
	t_vec2		t;
}				t_traceray;

typedef	struct	s_geom
{
	t_vec3		O;
	t_vec3		D;
	t_vec3		camera_rot;
	int			color;
	float		t_min;
	float		t_max;
}				t_geom;

typedef	struct	s_map
{
	SDL_Window	*window;
	SDL_Surface	*screen;
	SDL_Surface *fps;
	
	t_uint		*image;
	t_uint		*bufp;
	t_geom		geom;
	t_traceray	tr;
	t_viewport	vp;
	int			flag;
}				t_map;

float	deg_to_rad(float degree);
void	ClosestIntersection(t_traceray *tr, t_object *obj, 
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
