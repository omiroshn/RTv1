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
# include "SDL_image.h"
# include "SDL_mixer.h"
# include "SDL_ttf.h"
#elif __linux__
# include <SDL2/SDL_image.h>
# include <SDL2/SDL_mixer.h>
# include <SDL2/SDL_ttf.h>
#endif
// # include "opencl.h"

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
	int			name;
	t_vec3		center;
	t_vec3		color;
	t_vec3		direction;
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


// t_vec3		refl_color(t_vec3 *color, int deep)
// {
// 	int		i;
// 	t_vec3	ret;

// 	ret = (t_vec3){0, 0, 0, 0};
// 	i = deep;
// 	while(--i > 0)
// 	{
// 		ret = vec_f_mult(color[i].w, sum_color(ret, color[i]));
// 	}
// 	ret = sum_color(ret, color[0]);
// 	return (ret);
// }

// int		RayTrace(t_geom geom, float t_min, float t_max)
// {
// 	t_traceray	tr;
// 	t_sphere	sphere[4];
// 	t_vec3		P;
// 	t_vec3		N;
// 	t_vec3		R;
// 	t_vec3 		local_color[DEEP];
// 	t_vec3 		changed_color;
// 	int			deep = -1;
// 	double		intensity;
// 	float		r;

// 	sphere[0].center = (t_vec3){0, 0, 2};
// 	sphere[0].color = (t_vec3){255, 0, 0};
// 	sphere[0].specular = 50;
// 	sphere[0].radius = 1; // red
// 	sphere[0].reflection = 0.5;

// 	sphere[1].center = (t_vec3){-2, 0, 3};
// 	sphere[1].color = (t_vec3){0, 0, 255};
// 	sphere[1].specular = 125;
// 	sphere[1].radius = 1; // blue
// 	sphere[1].reflection = 0.5;

// 	sphere[2].center = (t_vec3){2, 0, 3};
// 	sphere[2].color = (t_vec3){0, 255, 0};
// 	sphere[2].specular = 50;
// 	sphere[2].radius = 1; //  green
// 	sphere[2].reflection = 0.5;

// 	sphere[3].center = (t_vec3){0, -5001, 0};
// 	sphere[3].color = (t_vec3){255, 255, 0};
// 	sphere[3].specular = 1000;
// 	sphere[3].radius = 5000; //  yellow
// 	sphere[3].reflection = 0.5;
 
// 	while (++deep < DEEP)
// 	{
// 		ClosestIntersection(&tr, sphere, geom.O, geom.D, t_min, t_max);
// 		if (tr.closest_t == INFINITY)
// 		{
// 			local_color[deep] = (t_vec3){0xFF,0xFF,0xFF,0xFF};
// 			break ;
// 		}
// 		P = vec_add(geom.O, vec_f_mult(tr.closest_t, geom.D)); //compute intersection
// 		N = vec_sub(P, tr.closest_sphere.center); //compute sphere normal at intersection
// 		N = vec_div(N, vec_lenght(N));
// 		t_vec3 DD = (t_vec3){-geom.D.x, -geom.D.y, -geom.D.z, 0};
// 		intensity = ComputeLighting(sphere, P, N, DD, tr.closest_sphere.specular); // -D
// 		local_color[deep] = vec_f_mult(intensity, tr.closest_sphere.color);

// 		local_color[deep].w = tr.closest_sphere.reflection;
// 		if (tr.closest_sphere.reflection > 0)
// 		{
// 			R = ReflectRay(DD, N);
// 			geom = (t_geom){P, R, geom.camera_rot, geom.color};
// 			local_color[deep] = vec_f_mult((1 - tr.closest_sphere.reflection), local_color[deep]);
// 		}
// 		else
// 			break ;
// 	}
// 	changed_color = refl_color(local_color, deep);
// 	return (rgb_to_int(changed_color.x, changed_color.y, changed_color.z));
// }
