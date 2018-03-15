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

t_vec3	rot_matrix(double alpha, double beta, double gamma, t_vec3 r)
{
	double		mat[3][3];
	t_vec3		ret;

	
	mat[0][0] = cos(beta) * cos(gamma);
	mat[1][0] = cos(gamma) * sin(alpha) * sin(beta) - cos(alpha) * sin(gamma);
	mat[2][0] = cos(alpha) * cos(gamma) * sin(beta) + sin(alpha) * sin(gamma);
	mat[0][1] = cos(beta) * sin(gamma);
	mat[1][1] = cos(alpha) * cos(gamma) + sin(alpha) * sin(beta) * sin(gamma);
	mat[2][1] = -cos(gamma) * sin(alpha) + cos(alpha) * sin(beta) * sin(gamma);
	mat[0][2] = -sin(beta);
	mat[1][2] = cos(beta) * sin(alpha);
	mat[2][2] = cos(alpha) * cos(beta);
	ret.x = (mat[0][0] * r.x) + (mat[1][0] * r.y) + (mat[2][0] * r.z);
	ret.y = (mat[0][1] * r.x) + (mat[1][1] * r.y) + (mat[2][1] * r.z);
	ret.z = (mat[0][2] * r.x) + (mat[1][2] * r.y) + (mat[2][2] * r.z);
	return (ret);
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

float	deg_to_rad(float degree)
{
	float	radians;

	radians = (degree * M_PI) / 180;
	return (radians);
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

double		ComputeLighting(t_object *obj, t_vec3 P, t_vec3 N, t_vec3 V, float specular)
{
	double		i;
	int			j;
	t_vec3		L;
	t_vec3		R;
	float		nl;
	float		rv;
	float		max;
	t_light		light[4];
	t_traceray	shadow;

	light[0].type = AMBIENT;
	light[0].intensity = 0.2;
	light[0].position = (t_vec3){0, 0, 0}; // NULL;
	light[0].direction = (t_vec3){0, 0, 0}; // NULL;

	light[1].type = POINT;
	light[1].intensity = 0.6;
	light[1].position = (t_vec3){2, 0, 0.5};
	light[1].direction = (t_vec3){0, 0, 0}; // NULL;

	light[2].type = DIRECTIONAL;
	light[2].intensity = 0.2;
	light[2].position = (t_vec3){0, 0, 0}; // NULL;
	light[2].direction = (t_vec3){1, 4, 4};

	light[3].type = POINT;
	light[3].intensity = 0.6;
	light[3].position = (t_vec3){-2, 1, 0.5};
	light[3].direction = (t_vec3){0, 0, 0}; // NULL;

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
				max = 0.99f;
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
			ClosestIntersection(&shadow, obj, P, L, 0.001, max);
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

double		get_lim_solution(double t, t_vec3 P, t_vec3 V, t_vec3 VA, t_object *obj)
{
	t_vec3	Q;
	float	k[2];

	if (t < 0)
		return (INFINITY);
	Q = vec_add(P, vec_f_mult(t, V));
	k[0] = dot(VA, vec_sub(Q, obj->center));
	k[1] = dot(VA, vec_sub(Q, obj->direction));
	if (k[0] < 0.0 && k[1] > 0.0)
		return (t);
	return (INFINITY);
}

t_vec2	IntersectRayCone(t_vec3 P, t_vec3 V, t_object *obj)
{
	t_vec3	PA;
	t_vec3	VA;
	t_vec3	deltaP;
	t_vec3	A;
	t_vec3	B;
	t_vec2	t;
	float	k[3];
	float	angle;
	float	POWCOS;
	float	POWSIN;

	angle = deg_to_rad(obj->angle);
	PA = obj->center;
	VA = normal(vec_sub(PA, obj->direction));
	deltaP = vec_sub(P, PA);

	A = vec_sub(V, vec_f_mult(dot(V, VA), VA));
	B = vec_sub(deltaP, vec_f_mult(dot(deltaP, VA), VA));
	POWCOS = cos(angle) * cos(angle);
	POWSIN = sin(angle) * sin(angle);

	k[0] = POWCOS * dot(A, A) - POWSIN * dot(V, VA) * dot(V, VA);
	k[1] = 2.0f * POWCOS * dot(A, B) - 2.0f * POWSIN * dot(V, VA) * dot(deltaP, VA);
	k[2] = POWCOS * dot(B, B) - POWSIN * dot(deltaP, VA) * dot(deltaP, VA);
	t = find_discriminant(k);
	t.x = get_lim_solution(t.x, P, V, VA, obj);
	t.y = get_lim_solution(t.y, P, V, VA, obj);
	return (t);
}


t_vec2	IntersectRayCylinder(t_vec3 P, t_vec3 V, t_object *obj)
{
	t_vec3	PA;
	t_vec3	VA;
	t_vec3	deltaP;
	t_vec3	A;
	t_vec3	B;
	t_vec2	t;
	float	k[3];

	PA = obj->center;
	VA = normal(vec_sub(PA, obj->direction));
	deltaP = vec_sub(P, PA);

	A = vec_sub(V, vec_f_mult(dot(V, VA), VA));
	B = vec_sub(deltaP, vec_f_mult(dot(deltaP, VA), VA));

	k[0] = dot(A, A);
	k[1] = 2.0f * dot(A, B);
	k[2] = dot(B, B) - obj->radius * obj->radius;
	t = find_discriminant(k);
	t.x = get_lim_solution(t.x, P, V, VA, obj);
	t.y = get_lim_solution(t.y, P, V, VA, obj);
	return (t);
}

t_vec2	IntersectRaySphere(t_vec3 O, t_vec3 D, t_object *obj)
{
	t_vec3	C;
	t_vec3	OC;
	double	r;
	float	k[3];

	C = obj->center;
	r = obj->radius;
	OC = vec_sub(O, C);

	k[0] = dot(D, D);
	k[1] = 2.0f * dot(OC, D);
	k[2] = dot(OC, OC) - r * r;
	return (find_discriminant(k));
}

t_vec2	IntersectRayPlane(t_vec3 O, t_vec3 D, t_object *obj)
{
	t_vec3	X;
	t_vec3	C;
	t_vec3	CC;
	t_vec3	N;
	t_vec2	t;
	float	k[2];

	C = obj->center;
	N = obj->direction;
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

void	ClosestIntersection(t_traceray *tr, t_object *obj, 
								t_vec3 O, t_vec3 D, float t_min, float t_max)
{
	int			i;
	t_vec2		t;

	tr->closest_t = INFINITY;
	i = -1;
	while (++i < 6)
	{
		if (obj[i].name == SPHERE)
			t = IntersectRaySphere(O, D, &obj[i]);
		else if (obj[i].name == PLANE)
			t = IntersectRayPlane(O, D, &obj[i]);
		else if (obj[i].name == CYLINDER)
			t = IntersectRayCylinder(O, D, &obj[i]);
		else if (obj[i].name == CONE)
			t = IntersectRayCone(O, D, &obj[i]);
		if (t.x > t_min && t.x < t_max && t.x < tr->closest_t)
		{
			tr->closest_t = t.x;
			tr->closest_obj = obj[i];
		}
		if (t.y > t_min && t.y < t_max && t.y < tr->closest_t)
		{
			tr->closest_t = t.y;
			tr->closest_obj = obj[i];
		}
	}
}

t_vec3	global_normal(t_vec3 P, t_traceray *tr)
{
	t_vec3	N;
	t_vec3	os;
	t_vec3	proj;

	if (tr->closest_obj.name == SPHERE)
	{
		N = vec_sub(P, tr->closest_obj.center);
		N = normal(N);
		return (N);
	}
	if (tr->closest_obj.name == CYLINDER || tr->closest_obj.name == CONE)
	{
		os = normal(vec_sub(tr->closest_obj.direction, tr->closest_obj.center));
		N = vec_sub(P, tr->closest_obj.center);
		proj = vec_f_mult(dot(N, os), os);
		N = vec_sub(N, proj);
		N = normal(N);
		return (N);
	}
	N = tr->closest_obj.direction; // PLANE
	return (N);
}

int		RayTrace(t_geom geom, float t_min, float t_max)
{
	t_traceray	tr;
	t_object	obj[6];
	t_vec3		P;
	t_vec3		N;
	t_vec3		R;
	t_vec3		local_color[DEEP + 1];
	int			deep;
	double		intensity;

	obj[0].name = SPHERE;
	obj[0].center = (t_vec3){0, 0.99, 2};
	obj[0].color = (t_vec3){255, 0, 0};
	obj[0].specular = 50;
	obj[0].radius = 1; // red
	obj[0].reflection = 0.3;
	
	obj[1].name = SPHERE;
	obj[1].center = (t_vec3){-2, 0.99, 3};
	obj[1].color = (t_vec3){0, 0, 255};
	obj[1].specular = 50;
	obj[1].radius = 1; // blue
	obj[1].reflection = 0.3;
	
	obj[2].name = SPHERE;
	obj[2].center = (t_vec3){2, 0.99, 3};
	obj[2].color = (t_vec3){0, 255, 0};
	obj[2].specular = 50;
	obj[2].radius = 1; //  green
	obj[2].reflection = 0.3;
	
	obj[3].name = CONE;
	obj[3].center = (t_vec3){-2, 1, 0};
	obj[3].direction = (t_vec3){-2, 0, 0};
	obj[3].color = (t_vec3){0, 0, 255};
	obj[3].specular = 1000;
	obj[3].radius = 1; //  white
	obj[3].reflection = 0.3;
	obj[3].angle = 30;

	obj[4].name = CYLINDER;
	obj[4].center = (t_vec3){2, 0, 0};
	obj[4].direction = (t_vec3){2, 1, 0};
	obj[4].color = (t_vec3){0, 255, 0};
	obj[4].specular = 1000;
	obj[4].radius = 0.5; //  white
	obj[4].reflection = 0.3;

	obj[5].name = PLANE;
	obj[5].center = (t_vec3){0, 0, 0};
	obj[5].color = (t_vec3){255, 255, 255};
	obj[5].direction = (t_vec3){0, 1, 0};
	obj[5].specular = 1000;
	obj[5].reflection = 0.5; //  white

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
		ClosestIntersection(&tr, obj, geom.O, geom.D, t_min, t_max);
		if (tr.closest_t == INFINITY)
		{
			local_color[deep] = (t_vec3){0,0,0};
			break ;
		}
		P = vec_add(geom.O, vec_f_mult(tr.closest_t, geom.D)); //compute intersection
		N = global_normal(P, &tr);
		t_vec3 DD = (t_vec3){-geom.D.x, -geom.D.y, -geom.D.z, 0};
		intensity = ComputeLighting(obj, P, N, DD, tr.closest_obj.specular); // -D
		local_color[deep] = vec_f_mult(intensity, tr.closest_obj.color);


		local_color[deep].reflection = tr.closest_obj.reflection;
		if (tr.closest_obj.reflection > 0)
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

	// if (m->flag)
	// 	m->geom.O.y = sin(m->geom.camera_rot.y);
	x = -1;
	while (++x < m->screen->w && (y = -1))
	{
		while (++y < m->screen->h)
		{
			m->geom.D = rot_matrix(m->geom.camera_rot.x,
				m->geom.camera_rot.y,m->geom.camera_rot.z,
				CanvasToViewport(m, x - m->screen->w / 2, m->screen->h / 2 - y));
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
			map.geom.camera_rot.x += 0.1;
		draw(&map);
		display_fps(&map);
		lsync();
		SDL_UpdateWindowSurface(map.window);
	}
	return (quit(&map));
}
