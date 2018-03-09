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

int		key_function(t_map *map)
{
	SDL_Event	e;

	while (SDL_PollEvent(&e))
	{
		if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN
			&& e.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
			return (0);
	}
	return (1);
}

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
	m->geom.O.x = 0;
	m->geom.O.y = 0;
	m->geom.O.z = 0;
	m->geom.D.x = 0;
	m->geom.D.y = 0;
	m->geom.D.z = 0;
	m->vp.w = 1;
	m->vp.h = 1;
	m->vp.dist = 1;
}

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
	if (!(ttf = TTF_OpenFont(FONTS_FOLDER"arcadeclassic.regular.ttf", 30)))
		put_error(IMG_GetError());
	m->fps = TTF_RenderText_Solid(ttf, fps_str,
					(SDL_Color){255, 255, 255, 255});
	SDL_BlitSurface(m->fps, NULL, m->screen,
		&(SDL_Rect){ 10, 0, m->fps->w, m->fps->h});
	TTF_CloseFont(ttf);
	SDL_FreeSurface(m->fps);
}

void	kernel_init()
{
	cl_device_id		device_id = NULL;
	cl_context			context = NULL;
	cl_command_queue	command_queue = NULL;
	cl_mem				memobj = NULL;
	cl_program			program = NULL;
	cl_kernel			kernel = NULL;
	cl_platform_id		platform_id = NULL;
	cl_uint				ret_num_devices;
	cl_uint				ret_num_platforms;
	cl_int				ret;

	FILE *fp;
	char string[MEM_SIZE];
	char fileName[] = "./kernel/kernel.cl";
	char *source_str;
	size_t source_size;

	if (!(fp = fopen(fileName, "r")))
	{
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	memobj = clCreateBuffer(context, CL_MEM_READ_WRITE,MEM_SIZE * sizeof(char), NULL, &ret);
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
		(const size_t *)&source_size, &ret);
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	kernel = clCreateKernel(program, "hello", &ret);
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);
	ret = clEnqueueTask(command_queue, kernel, 0, NULL,NULL);
	ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0, 
		MEM_SIZE, string, 0, NULL, NULL);
	printf("%s\n", string);
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(memobj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	free(source_str);
}

t_vec3	CanvasToViewport(t_map *m, float x, float y)
{
	t_vec3 D;

	D.x = x * m->vp.w / m->screen->w;
	D.y = y * m->vp.h / m->screen->h;
	D.z = m->vp.dist;
	return (D);
}

float	dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec2	find_discriminant(float k[3])
{
	t_vec2	t;
	float	disc;

	disc = k[1] * k[1] - 4.0f * k[0] * k[2];
	if (disc < 0)
		return ((t_vec2){INFINITY, INFINITY});
	t.x = (-k[1] + sqrt(disc)) / (2.0f * k[0]);
	t.y = (-k[1] - sqrt(disc)) / (2.0f * k[0]);
	return (t);
}

t_vec2	IntersectRaySphere(t_vec3 O, t_vec3 D, t_sphere *sphere)
{
	t_vec3	C;
	t_vec3	OC;
	double	r;
	float	k[3];

	C = sphere->center;
	r = sphere->radius;
	OC.x = O.x - C.x;
	OC.y = O.y - C.y;
	OC.z = O.z - C.z;

	k[0] = dot(D, D);
	k[1] = 2.0f * dot(OC, D);
	k[2] = dot(OC, OC) - r * r;
	return ((t_vec2)find_discriminant(k));
}

int		RayTrace(t_vec3 O, t_vec3 D, float t_min, float t_max)
{
	t_traceray	tr;
	t_sphere	sphere[3];
	t_vec2		t;
	int			i;

	sphere[0].center = (t_vec3){1, 2, 3};
	sphere[0].color = (t_vec3){255, 156, 127};
	sphere[0].radius = 0.2; // red

	sphere[1].center = (t_vec3){1.7, 2, 3};
	sphere[1].color = (t_vec3){0, 0, 255};
	sphere[1].radius = 0.2; // blue

	sphere[2].center = (t_vec3){2.5, 2, 3};
	sphere[2].color = (t_vec3){126, 255, 47};
	sphere[2].radius = 0.2; //  green

	tr.closest_t = INFINITY;
	
	i = -1;
	while (++i < 3)
	{
		t = IntersectRaySphere(O, D, &sphere[i]);
		if (t.x > t_min && t.x < t_max && t.x < tr.closest_t)
		{
			tr.closest_t = t.x;
			tr.closest_sphere = sphere[i];
		}
		if (t.y > t_min && t.y < t_max && t.y < tr.closest_t)
		{
			tr.closest_t = t.y;
			tr.closest_sphere = sphere[i];
		}
	}
	if (tr.closest_t == INFINITY)
		return (BACKGROUND_COLOR);
	return (rgb_to_int(tr.closest_sphere.color.x,
		tr.closest_sphere.color.y, tr.closest_sphere.color.z));
}

void	draw(t_map *m)
{
	int x;
	int y;

	x = -1;
	while (++x < m->screen->w && (y = -1))
	{
		while (++y < m->screen->h)
		{
			m->geom.D = CanvasToViewport(m, (float)x, (float)y);
			m->geom.color = RayTrace(m->geom.O, m->geom.D, 0.001f, INFINITY);
			m->image[x + y * m->screen->w] = m->geom.color;
		}
	}
}

int		main(int argc, char **argv)
{
	t_map	map;

	init(&map);
	init_geometry(&map);
	// kernel_init();
	while (TRUE)
	{
		ft_bzero(map.screen->pixels, map.screen->w * map.screen->h * 4);
		if (!key_function(&map))
			break ;
		draw(&map);
		display_fps(&map);
		lsync();
		SDL_UpdateWindowSurface(map.window);
	}
	return (quit(&map));
}
