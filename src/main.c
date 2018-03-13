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
				map->geom.O.z += 0.1;
			if (e.key.keysym.scancode == SDL_SCANCODE_DOWN)
				map->geom.O.z -= 0.1;
			if (e.key.keysym.scancode == SDL_SCANCODE_W)
				map->geom.O.y += 0.1;
			if (e.key.keysym.scancode == SDL_SCANCODE_S)
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

// void	kernel_init()
// {
// 	cl_device_id		device_id = NULL;
// 	cl_context			context = NULL;
// 	cl_command_queue	command_queue = NULL;
// 	cl_mem				memobj = NULL;
// 	cl_program			program = NULL;
// 	cl_kernel			kernel = NULL;
// 	cl_platform_id		platform_id = NULL;
// 	cl_uint				ret_num_devices;
// 	cl_uint				ret_num_platforms;
// 	cl_int				ret;

// 	FILE *fp;
// 	char string[MEM_SIZE];
// 	char fileName[] = "./kernel/kernel.cl";
// 	char *source_str;
// 	size_t source_size;

// 	if (!(fp = fopen(fileName, "r")))
// 	{
// 		fprintf(stderr, "Failed to load kernel.\n");
// 		exit(1);
// 	}
// 	source_str = (char*)malloc(MAX_SOURCE_SIZE);
// 	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
// 	fclose(fp);
// 	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
// 	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
// 	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
// 	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
// 	memobj = clCreateBuffer(context, CL_MEM_READ_WRITE,MEM_SIZE * sizeof(char), NULL, &ret);
// 	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
// 		(const size_t *)&source_size, &ret);
// 	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
// 	kernel = clCreateKernel(program, "hello", &ret);
// 	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);
// 	ret = clEnqueueTask(command_queue, kernel, 0, NULL,NULL);
// 	ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0, 
// 		MEM_SIZE, string, 0, NULL, NULL);
// 	printf("%s\n", string);
// 	ret = clFlush(command_queue);
// 	ret = clFinish(command_queue);
// 	ret = clReleaseKernel(kernel);
// 	ret = clReleaseProgram(program);
// 	ret = clReleaseMemObject(memobj);
// 	ret = clReleaseCommandQueue(command_queue);
// 	ret = clReleaseContext(context);
// 	free(source_str);
// }

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

t_matrix4	matrix_mult(const t_matrix4 m, const t_matrix4 rhs)
{
	t_matrix4	mult;
	int			i;
	int			j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			mult.m[i][j] = m.m[i][0] * rhs.m[0][j] +
							m.m[i][1] * rhs.m[1][j] +
							m.m[i][2] * rhs.m[2][j] +
							m.m[i][3] * rhs.m[3][j];
			j++;
		}
		i++;
	}
	return (mult);
}

t_matrix4	rotate_x(t_map *m)
{
	t_matrix4 rhs;

	rhs.m[0][0] = 1;
	rhs.m[0][1] = 0;
	rhs.m[0][2] = 0;
	rhs.m[0][3] = 0;
	rhs.m[1][0] = 0;
	rhs.m[1][1] = cos(m->geom.camera_rot.x);
	rhs.m[1][2] = sin(m->geom.camera_rot.x);
	rhs.m[1][3] = 0;
	rhs.m[2][0] = 0;
	rhs.m[2][1] = -sin(m->geom.camera_rot.x);
	rhs.m[2][2] = cos(m->geom.camera_rot.x);
	rhs.m[2][3] = 0;
	rhs.m[3][0] = 0;
	rhs.m[3][1] = 0;
	rhs.m[3][2] = 0;
	rhs.m[3][3] = 1;
	return (rhs);
}

t_matrix4	rotate_y(t_map *m)
{
	t_matrix4 rhs;

	rhs.m[0][0] = cos(m->geom.camera_rot.y);
	rhs.m[0][1] = 0;
	rhs.m[0][2] = -sin(m->geom.camera_rot.y);
	rhs.m[0][3] = 0;
	rhs.m[1][0] = 0;
	rhs.m[1][1] = 1;
	rhs.m[1][2] = 0;
	rhs.m[1][3] = 0;
	rhs.m[2][0] = sin(m->geom.camera_rot.y);
	rhs.m[2][1] = 0;
	rhs.m[2][2] = cos(m->geom.camera_rot.y);
	rhs.m[2][3] = 0;
	rhs.m[3][0] = 0;
	rhs.m[3][1] = 0;
	rhs.m[3][2] = 0;
	rhs.m[3][3] = 1;
	return (rhs);
}

t_matrix4	rotate_z(t_map *m)
{
	t_matrix4 rhs;

	rhs.m[0][0] = cos(m->geom.camera_rot.z);
	rhs.m[0][1] = sin(m->geom.camera_rot.z);
	rhs.m[0][2] = 0;
	rhs.m[0][3] = 0;
	rhs.m[1][0] = -sin(m->geom.camera_rot.z);
	rhs.m[1][1] = cos(m->geom.camera_rot.z);
	rhs.m[1][2] = 0;
	rhs.m[1][3] = 0;
	rhs.m[2][0] = 0;
	rhs.m[2][1] = 0;
	rhs.m[2][2] = 1;
	rhs.m[2][3] = 0;
	rhs.m[3][0] = 0;
	rhs.m[3][1] = 0;
	rhs.m[3][2] = 0;
	rhs.m[3][3] = 1;
	return (rhs);
}

t_vec3		vec_matrix_mult(t_vec3 vec, const t_matrix4 rhs)
{
	t_vec3	res;

	res.x = vec.x * rhs.m[0][0] + vec.y * rhs.m[1][0] + vec.z * rhs.m[2][0] + vec.w * rhs.m[3][0];
	res.y = vec.x * rhs.m[0][1] + vec.y * rhs.m[1][1] + vec.z * rhs.m[2][1] + vec.w * rhs.m[3][1];
	res.z = vec.x * rhs.m[0][2] + vec.y * rhs.m[1][2] + vec.z * rhs.m[2][2] + vec.w * rhs.m[3][2];
	res.w = vec.x * rhs.m[0][3] + vec.y * rhs.m[1][3] + vec.z * rhs.m[2][3] + vec.w * rhs.m[3][3];
	return (res);
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

t_vec3	vec_mult(t_vec3 a, t_vec3 b)
{
	t_vec3 c;

	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
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

t_vec3	normal(t_vec3 a)
{
	t_vec3 c;

	c = vec_div(a, vec_lenght(a));
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

t_vec3	ReflectRay(t_vec3 R, t_vec3 N)
{
	t_vec3 res;

	res = vec_sub(vec_f_mult(dot(N, R), vec_f_mult(2.0, N)), R);
    return (res);
}

double		ComputeLighting(t_sphere *sphere, t_vec3 P, t_vec3 N, t_vec3 V, float specular)
{
	int j;
	double i;
	t_light light[4];
	t_vec3	L;
	t_vec3	R;
	float nl;
	float rv;
	float max;
	t_traceray shadow;

	light[0].type = AMBIENT;
	light[0].intensity = 0.2;

	light[1].type = POINT;
	light[1].intensity = 0.6;
	light[1].position = (t_vec3){2, 1, 1};

	light[2].type = DIRECTIONAL;
	light[2].intensity = 0.2;
	light[2].direction = (t_vec3){1, 4, 4};

	light[3].type = POINT;
	light[3].intensity = 0.3;
	light[3].position = (t_vec3){-2, 1, 1};

	i = 0.0;
	j = -1;
	while (++j < 4)
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

			// Проверка нормали для света
			if (dot(V, N) < 0)
				continue ;
			
			// Проверка тени
			ClosestIntersection(&shadow, sphere, P, L, 0.001, max);
			if (shadow.closest_t != INFINITY)
				continue ;

			// Диффузность
			nl = dot(N, L);
			if (nl > 0)
				i += light[j].intensity * nl / (vec_lenght(N) * vec_lenght(L));

			// Зеркальность
			if (specular >= 0)
			{
				R = vec_sub(vec_f_mult(dot(N, L), vec_f_mult(2.0, N)), L); //vec_sub(vec_f_mult((dot(N, L) * 2), N), L);
				rv = dot(R, V);
				if (rv > 0)
					i += light[j].intensity * pow(rv / (vec_lenght(R) * vec_lenght(V)), specular);
			}
		}
	}
	i > 1.0 ? i = 1.0 : 0;
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

t_vec2	IntersectRayPlane(t_vec3 O, t_vec3 D, t_sphere *sphere)
{
	t_vec3	X;
	t_vec3	C;
	t_vec3	CC;
	t_vec3	N;
	t_vec2	t;
	float	k[2];

	C = sphere->center;
	N = sphere->direction;
	X = vec_sub(O, C);
	k[0] = dot(D, N);
	k[1] = dot(X, N);
	if (k[0])
	{
		t.x = -k[1] / k[0];
		t.y = INFINITY;
		return (t);		
	}
	return ((t_vec2){INFINITY, INFINITY});
}

void	ClosestIntersection(t_traceray *tr, t_sphere *sphere, 
								t_vec3 O, t_vec3 D, float t_min, float t_max)
{
	int			i;
	t_vec2		t;

	tr->closest_t = INFINITY;
	i = -1;
	while (++i < 6)
	{
		if (sphere[i].name == SPHERE)
			t = IntersectRaySphere(O, D, &sphere[i]);
		else if (sphere[i].name == PLANE)
			t = IntersectRayPlane(O, D, &sphere[i]);
		if (t.x > t_min && t.x < t_max && t.x < tr->closest_t)
		{
			tr->closest_t = t.x;
			tr->closest_sphere = sphere[i];
		}
		if (t.y > t_min && t.y < t_max && t.y < tr->closest_t)
		{
			tr->closest_t = t.y;
			tr->closest_sphere = sphere[i];
		}
	}
}

t_vec3		sum_color(t_vec3 c1, t_vec3 c2)
{
	t_vec3	ret;

	ret = vec_add(c1, c2);
	if (ret.x > 255.0F)
		ret.x = 255.0F;
	if (ret.y > 255.0F)
		ret.y = 255.0F;
	if (ret.z > 255.0F)
		ret.z = 255.0F;
	return (ret);
}

t_vec3		refl_color(t_vec3 *color, int deep)
{
	int	i;

	i = 0;
	while(i < deep)
	{
		color[i + 1] = vec_f_mult(1 - color[i + 1].w, sum_color(color[i], color[i + 1]));
		i++;
	}
	return (color[i]);
}

int		RayTrace(t_geom geom, float t_min, float t_max)
{
	t_traceray	tr;
	t_sphere	sphere[6];
	t_vec3		P;
	t_vec3		N;
	t_vec3		R;
	t_vec3		local_color[DEEP + 1];
	int			deep;
	double		intensity;

	sphere[0].name = SPHERE;
	sphere[0].center = (t_vec3){0, 0, 2};
	sphere[0].color = (t_vec3){255, 0, 0};
	sphere[0].specular = 50;
	sphere[0].radius = 1; // red
	sphere[0].reflection = 0.3;

	sphere[1].name = SPHERE;
	sphere[1].center = (t_vec3){-2, 0, 3};
	sphere[1].color = (t_vec3){0, 0, 255};
	sphere[1].specular = 50;
	sphere[1].radius = 1; // blue
	sphere[1].reflection = 0.3;

	sphere[2].name = SPHERE;
	sphere[2].center = (t_vec3){2, 0, 3};
	sphere[2].color = (t_vec3){0, 255, 0};
	sphere[2].specular = 50;
	sphere[2].radius = 1; //  green
	sphere[2].reflection = 0.3;

	sphere[3].name = CONE;
	sphere[3].center = (t_vec3){0, -101, 0};
	sphere[3].color = (t_vec3){255, 255, 255};
	sphere[3].specular = 1000;
	sphere[3].radius = 1; //  white
	sphere[3].reflection = 0.3;

	sphere[4].name = CYLINDER;
	sphere[4].center = (t_vec3){0, -101, 0};
	sphere[4].color = (t_vec3){255, 255, 255};
	sphere[4].specular = 1000;
	sphere[4].radius = 1; //  white
	sphere[4].reflection = 0.3;

	sphere[5].name = PLANE;
	sphere[5].center = (t_vec3){0, -0.99, 0};
	sphere[5].color = (t_vec3){255, 255, 255};
	sphere[5].direction = (t_vec3){0, 1, 0};
	sphere[5].specular = 1000;
	sphere[5].reflection = 0.5; //  white

	deep = DEEP;
	while (deep >= 0)
	{
		local_color[deep].x = 0;
		local_color[deep].y = 0;
		local_color[deep].z = 0;
		local_color[deep].reflection = 0;
		deep--;
	}
	deep = DEEP;
	while (deep >= 0)
	{
		ClosestIntersection(&tr, sphere, geom.O, geom.D, t_min, t_max);
		if (tr.closest_t == INFINITY)
		{
			local_color[deep] = (t_vec3){0,0,0};
			break ;
		}
		P = vec_add(geom.O, vec_f_mult(tr.closest_t, geom.D)); //compute intersection
		if (tr.closest_sphere.name == SPHERE)
		{
			N = vec_sub(P, tr.closest_sphere.center); //compute sphere normal at intersection
			N = normal(N);
		}
		else if (tr.closest_sphere.name == PLANE)
			N = tr.closest_sphere.direction;
		t_vec3 DD = (t_vec3){-geom.D.x, -geom.D.y, -geom.D.z, 0};
		intensity = ComputeLighting(sphere, P, N, DD, tr.closest_sphere.specular); // -D
		local_color[deep] = vec_f_mult(intensity, tr.closest_sphere.color);


		local_color[deep].reflection = tr.closest_sphere.reflection;
		if (tr.closest_sphere.reflection > 0)
		{
			R = ReflectRay(DD, N);
			geom = (t_geom){P, R, geom.camera_rot, geom.color};
			deep--;
		}
		else
			break ;
	}
	deep = 0;
	while (deep < DEEP)
	{
		local_color[deep + 1] = vec_add(vec_f_mult(1 - local_color[deep + 1].reflection, local_color[deep + 1]),
						vec_f_mult(local_color[deep + 1].reflection, local_color[deep]));
		deep++;
	}
	return (rgb_to_int(local_color[deep].x, local_color[deep].y, local_color[deep].z));
}

void	draw(t_map *m)
{
	int x;
	int y;
	t_matrix4 trix[4];

	trix[0] = rotate_x(m);
	trix[1] = rotate_y(m);
	trix[2] = rotate_z(m);
	// if (m->flag)
	// 	m->geom.O.y = sin(m->geom.camera_rot.y);
	trix[3] = matrix_mult(matrix_mult(trix[0], trix[1]), trix[2]);
	x = -1;
	while (++x < m->screen->w && (y = -1))
	{
		while (++y < m->screen->h)
		{
			m->geom.D = vec_matrix_mult(CanvasToViewport(m, x - m->screen->w / 2, m->screen->h / 2 - y), trix[3]);
			m->geom.color = RayTrace(m->geom, 0.001f, INFINITY);
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
