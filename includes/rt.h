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
# ifdef __APPLE__
#  include <OpenCL/opencl.h>
#  include "SDL_image.h"
#  include "SDL_mixer.h"
#  include "SDL_ttf.h"
#  define WIDTH 1280
#  define HEIGHT 1280
# elif __linux__
#  include <SDL2/SDL_image.h>
#  include <SDL2/SDL_mixer.h>
#  include <SDL2/SDL_ttf.h>
#  include <CL/cl.h>
#  define WIDTH 1000
#  define HEIGHT 1000
# endif

# define MEM_SIZE 128
# define MAX_SOURCE_SIZE 0x40000

# define MAPS_FOLDER "resources/maps/"
# define FONTS_FOLDER "resources/fonts/"

typedef unsigned int	t_uint;

typedef	struct			s_vec2
{
	float				x;
	float				y;
}						t_vec2;

typedef	struct			s_vec3
{
	float				x;
	float				y;
	float				z;
	float				w;
	float				reflection;
}						t_vec3;

typedef	struct			s_light
{
	int					type;
	double				intensity;
	t_vec3				position;
	t_vec3				direction;
}						t_light;

typedef	struct			s_object
{
	int					name;
	t_vec3				center;
	t_vec3				direction;
	t_vec3				color;
	float				specular;
	float				radius;
	float				reflection;
	float				angle;
}						t_object;

typedef struct			s_viewport
{
	float				w;
	float				h;
	float				dist;
}						t_viewport;

typedef	struct			s_traceray
{
	double				closest_t;
	t_object			closest_obj;
	t_vec2				t;
}						t_traceray;

typedef	struct			s_geom
{
	t_vec3				o;
	t_vec3				d;
	t_vec3				camera_rot;
	int					color;
	float				t_min;
	float				t_max;
	int					objects;
	int					lights;
	t_object			*obj;
	t_light				*light;
	t_viewport			vp;
	int					screenw;
	int					screenh;
}						t_geom;

typedef	struct			s_cl
{
	cl_device_id		device;
	cl_context			context;
	cl_command_queue	queue;
	cl_mem				image;
	cl_mem				objects;
	cl_mem				lights;
	cl_program			program;
	cl_kernel			kernel;
	cl_platform_id		platform_id;
	cl_uint				ret_num_devices;
	cl_uint				ret_num_platforms;
}						t_cl;

typedef	struct			s_map
{
	SDL_Window			*window;
	SDL_Surface			*screen;
	SDL_Surface			*fps;
	TTF_Font			*ttf;
	t_cl				cl;
	t_uint				*image;
	t_geom				geom;
	t_traceray			tr;
	int					flag;
	char				*tmp;
	char				*tmp2;
	char				*tmp3;
}						t_map;

/*
**						opencl.c
*/

void					kernel(t_map *m);
void					kernel_init(t_map *m);

/*
**						read.c
*/

void					read_map(t_map *m, char *filename);
void					read_camera(t_map *m, int fd, char *lol,
													char **numbers);
void					read_objects(t_map *m, char *objects, int i);
void					read_lights(t_map *m, char *objects, int i);

/*
**						init.c
*/

void					init_geometry(t_map *m);
void					init(t_map *m);

/*
**						system.c
*/

float					deg_to_rad(float degree);
void					number_action(char **numbers, char *line);
char					*get_from_brackets(char *str, char c1, char c2);
float					ft_ftoi(char *str);

/*
**						fps.c
*/

void					display_fps(t_map *m);
void					lsync(void);

/*
**						put_error.c
*/

void					put_error(const char *msg);
void					put_usage(void);
int						quit(t_map *m);

#endif
