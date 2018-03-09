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
		else if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.scancode == SDL_SCANCODE_RIGHT)
				map->geom.O.x += 0.1;
			if (e.key.keysym.scancode == SDL_SCANCODE_LEFT)
				map->geom.O.x -= 0.1;
			if (e.key.keysym.scancode == SDL_SCANCODE_UP)
				map->geom.O.y += 0.1;
			if (e.key.keysym.scancode == SDL_SCANCODE_DOWN)
				map->geom.O.y -= 0.1;
		}
		else if (e.type == SDL_KEYUP)
		{
			if (e.key.keysym.scancode == SDL_SCANCODE_P)
				map->flag ^= 1;
		}
	}
	return (1);
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

int		ft_lerpi(int first, int second, double p)
{
	if (first == second)
		return (first);
	return ((int)((double)first + (second - first) * p));
}

int		clerp(int c1, int c2, double p)
{
	int r;
	int g;
	int b;

	if (c1 == c2)
		return (c1);
	r = ft_lerpi((c1 >> 16) & 0xFF, (c2 >> 16) & 0xFF, p);
	g = ft_lerpi((c1 >> 8) & 0xFF, (c2 >> 8) & 0xFF, p);
	b = ft_lerpi(c1 & 0xFF, c2 & 0xFF, p);
	return (r << 16 | g << 8 | b);
}

float	vec_lenght(t_vec3 struc)
{
	return (sqrt(struc.x * struc.x +
		struc.y * struc.y + struc.z * struc.z));
}

t_vec3	vec_f_mult(double a, t_vec3 b)
{
	t_vec3 c;

	c.x = a * b.x;
	c.y = a * b.y;
	c.z = a * b.z;
	return (c);
}

t_vec3	vec_add(t_vec3 a, t_vec3 b)
{
	t_vec3 c;

	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	return (c);
}

t_vec3	vec_div(t_vec3 a, float b)
{
	t_vec3 c;

	c.x = a.x / b;
	c.y = a.y / b;
	c.z = a.z / b;
	return (c);
}

t_vec3	vec_sub(t_vec3 a, t_vec3 b)
{
	t_vec3 c;

	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	return (c);
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

t_vec3	vec_normalizing(t_vec3 struc, float lenght)
{
	float invlen;

	invlen = 0;
	if (lenght > 0)
	{
		invlen = 1 / lenght;
		struc.x *= invlen;
		struc.y *= invlen;
		struc.z *= invlen;
	}
	return (struc);
}

double		ComputeLighting(t_vec3 P, t_vec3 N)
{
	int j;
	double i;
	t_light light[3];
	t_vec3	L;
	float n_dot_l;
	float max;

	light[0].type = AMBIENT;
	light[0].intensity = 0.2;

	light[1].type = POINT;
	light[1].intensity = 0.6;
	light[1].position = (t_vec3){2, 1, 0};

	light[2].type = DIRECTIONAL;
	light[2].intensity = 0.2;
	light[2].direction = (t_vec3){1, 4, 4};

	i = 0.0;
	j = -1;
	while (++j < 3)
	{
		if (light[j].type == AMBIENT)
			i += light[j].intensity;
		else
		{
			if (light[j].type == POINT)
			{
				L = vec_sub(light[j].position, P);
				max = 1.0f;
			}
			else
			{
				L = light[j].direction;
				max = INFINITY;
			}
			n_dot_l = dot(N, L);
			if (n_dot_l > 0)
				i += light[j].intensity * n_dot_l / (vec_lenght(N) * vec_lenght(L));
		}
	}
	// i > 1.0 ? i = 1.0 : 0;
	return (i);
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
	t_sphere	sphere[4];
	t_vec2		t;
	t_vec3		P;
	t_vec3		N;
	t_vec3 		local_color;
	int			i;
	double		intensity;

	sphere[0].center = (t_vec3){0, -1, 3};
	sphere[0].color = (t_vec3){255, 0, 0};
	sphere[0].radius = 1; // red

	sphere[1].center = (t_vec3){-2, 0, 3};
	sphere[1].color = (t_vec3){0, 0, 255};
	sphere[1].radius = 1; // blue

	sphere[2].center = (t_vec3){2, 0, 3};
	sphere[2].color = (t_vec3){0, 255, 0};
	sphere[2].radius = 1; //  green

	sphere[3].center = (t_vec3){0, -5001, 0};
	sphere[3].color = (t_vec3){255, 255, 0};
	sphere[3].radius = 5000; //  yellow

	tr.closest_t = INFINITY;
	
	i = -1;
	while (++i < 4)
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
		return (0xffffff);


	P = vec_add(O, vec_f_mult(tr.closest_t, D)); //compute intersection
	N = vec_sub(P, tr.closest_sphere.center); //compute sphere normal at intersection
	N = vec_div(N, vec_lenght(N)); // N = vec_normalizing(N, vec_lenght(N));


	intensity = ComputeLighting(P, N); //(P, N, -D, 500);
	local_color = vec_f_mult(intensity, tr.closest_sphere.color);
	return (rgb_to_int(local_color.x, local_color.y, local_color.z));
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
			m->geom.D = CanvasToViewport(m, x - m->screen->w / 2, m->screen->h / 2 - y);
			m->geom.color = RayTrace(m->geom.O, m->geom.D, 0.001f, INFINITY);
			// clerp(0, m->geom.color, ComputeLighting);
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
		if (map.flag)
			map.geom.O.z -= 0.1;
		draw(&map);
		display_fps(&map);
		lsync();
		SDL_UpdateWindowSurface(map.window);
	}
	return (quit(&map));
}
