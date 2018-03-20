/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.cl                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/07 18:50:22 by omiroshn          #+#    #+#             */
/*   Updated: 2018/02/07 18:50:24 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define TRUE 1
# define AMBIENT 1
# define POINT 2
# define DIRECTIONAL 3
# define SPHERE 1
# define PLANE 2
# define CYLINDER 3
# define CONE 4

# define DEEP 1

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
	int			objects;
	int			lights;
	long		kal;
	long		kal2;
	t_viewport	vp;
	int			screenw;
	int			screenh;
}				t_geom;



t_vec3	global_normal(t_vec3 P, t_traceray *tr);
t_vec3	rot_matrix(double alpha, double beta, double gamma, t_vec3 r);
t_vec3	vec_f_mult(double a, t_vec3 b);
t_vec3	vec_mult(t_vec3 a, t_vec3 b);
t_vec3	vec_add(t_vec3 a, t_vec3 b);
t_vec3	vec_div(t_vec3 a, float b);
t_vec3	vec_sub(t_vec3 a, t_vec3 b);
t_vec3	normal(t_vec3 a);
t_vec3	ReflectRay(t_vec3 R, t_vec3 N);
t_vec3	CanvasToViewport(t_geom *geom, float x, float y);
t_vec2	find_discriminant(float k[3]);
t_vec2	IntersectRayCylinder(t_vec3 P, t_vec3 V, __constant t_object *obj);
t_vec2	IntersectRayCone(t_vec3 P, t_vec3 V, __constant t_object *obj);
t_vec2	IntersectRaySphere(t_vec3 O, t_vec3 D, __constant t_object *obj);
t_vec2	IntersectRayPlane(t_vec3 O, t_vec3 D, __constant t_object *obj);
float	vec_lenght(t_vec3 struc);
float	deg_to_rad(float degree);
float	ft_dot(t_vec3 a, t_vec3 b);
int		rgb_to_int(int red, int green, int blue);
double	ComputeLighting(t_geom *geom, __constant t_object *obj, __constant t_light *light, t_vec3 P, t_vec3 N, t_vec3 V, float specular);
double	get_lim_solution(double t, t_vec3 P, t_vec3 V, t_vec3 VA, __constant t_object *obj);
int		RayTrace(t_geom geom, __constant t_object *obj, __constant t_light *light, float t_min, float t_max);
void	ClosestIntersection(t_traceray *tr, t_geom *geom, __constant t_object *obj,
							t_vec3 O, t_vec3 D, float t_min, float t_max);

t_vec3	vec_f_mult(double a, t_vec3 b)
{
	t_vec3 c;

	c.x = a * b.x;
	c.y = a * b.y;
	c.z = a * b.z;
	return (c);
}

int		rgb_to_int(int red, int green, int blue)
{
	int r;
	int g;
	int b;

	r = red & 0xFF;
	g = green & 0xFF;
	b = blue & 0xFF;
	return (r << 16 | g << 8 | b);
}

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

t_vec3	CanvasToViewport(t_geom *geom, float x, float y)
{
	t_vec3 D;

	D.x = x * geom->vp.w / geom->screenw;
	D.y = y * geom->vp.h / geom->screenh;
	D.z = geom->vp.dist;
	return (D);
}

float	deg_to_rad(float degree)
{
	float	radians;

	radians = (degree * M_PI) / 180;
	return (radians);
}

float	ft_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec3	ReflectRay(t_vec3 R, t_vec3 N)
{
	t_vec3 res;

	res = vec_sub(vec_f_mult(ft_dot(N, R), vec_f_mult(2.0, N)), R);
    return (res);
}

double		ComputeLighting(t_geom *geom, __constant t_object *obj, __constant t_light *light,
									t_vec3 P, t_vec3 N, t_vec3 V, float specular)
{
	double		i;
	int			j;
	t_vec3		L;
	t_vec3		R;
	float		nl;
	float		rv;
	float		max;
	t_traceray	shadow;

	i = 0.0;
	j = -1;
	while (++j < geom->lights)
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
			if (ft_dot(V, N) < 0)
				continue ;
			
			// Проверка тени
			ClosestIntersection(&shadow, geom, obj, P, L, 0.001, max);
			if (shadow.closest_t != INFINITY)
				continue ;

			// Диффузность
			nl = ft_dot(N, L);
			if (nl > 0)
				i += light[j].intensity * nl / (vec_lenght(N) * vec_lenght(L));

			// Зеркальность
			if (specular >= 0)
			{
				R = vec_sub(vec_f_mult(ft_dot(N, L), vec_f_mult(2.0, N)), L); //vec_sub(vec_f_mult((ft_dot(N, L) * 2), N), L);
				rv = ft_dot(R, V);
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

double		get_lim_solution(double t, t_vec3 P, t_vec3 V, t_vec3 VA, __constant t_object *obj)
{
	t_vec3	Q;
	float	k[2];

	if (t < 0)
		return (INFINITY);
	Q = vec_add(P, vec_f_mult(t, V));
	k[0] = ft_dot(VA, vec_sub(Q, obj->center));
	k[1] = ft_dot(VA, vec_sub(Q, obj->direction));
	if (k[0] < 0.0 && k[1] > 0.0)
		return (t);
	return (INFINITY);
}

t_vec2	IntersectRayCone(t_vec3 P, t_vec3 V, __constant t_object *obj)
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

	A = vec_sub(V, vec_f_mult(ft_dot(V, VA), VA));
	B = vec_sub(deltaP, vec_f_mult(ft_dot(deltaP, VA), VA));
	POWCOS = cos(angle) * cos(angle);
	POWSIN = sin(angle) * sin(angle);

	k[0] = POWCOS * ft_dot(A, A) - POWSIN * ft_dot(V, VA) * ft_dot(V, VA);
	k[1] = 2.0f * POWCOS * ft_dot(A, B) - 2.0f * POWSIN * ft_dot(V, VA) * ft_dot(deltaP, VA);
	k[2] = POWCOS * ft_dot(B, B) - POWSIN * ft_dot(deltaP, VA) * ft_dot(deltaP, VA);
	t = find_discriminant(k);
	t.x = get_lim_solution(t.x, P, V, VA, obj);
	t.y = get_lim_solution(t.y, P, V, VA, obj);
	return (t);
}

t_vec2	IntersectRayCylinder(t_vec3 P, t_vec3 V, __constant t_object *obj)
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

	A = vec_sub(V, vec_f_mult(ft_dot(V, VA), VA));
	B = vec_sub(deltaP, vec_f_mult(ft_dot(deltaP, VA), VA));

	k[0] = ft_dot(A, A);
	k[1] = 2.0f * ft_dot(A, B);
	k[2] = ft_dot(B, B) - obj->radius * obj->radius;
	t = find_discriminant(k);
	t.x = get_lim_solution(t.x, P, V, VA, obj);
	t.y = get_lim_solution(t.y, P, V, VA, obj);
	return (t);
}

t_vec2	IntersectRaySphere(t_vec3 O, t_vec3 D, __constant t_object *obj)
{
	t_vec3	C;
	t_vec3	OC;
	double	r;
	float	k[3];

	C = obj->center;
	r = obj->radius;
	OC = vec_sub(O, C);

	k[0] = ft_dot(D, D);
	k[1] = 2.0f * ft_dot(OC, D);
	k[2] = ft_dot(OC, OC) - r * r;
	return (find_discriminant(k));
}

t_vec2	IntersectRayPlane(t_vec3 O, t_vec3 D, __constant t_object *obj)
{
	t_vec3	X;
	t_vec3	C;
	t_vec3	N;
	t_vec2	t;
	float	k[2];

	C = obj->center;
	N = obj->direction;
	X = vec_sub(O, C);
	k[0] = ft_dot(D, N);
	k[1] = ft_dot(X, N);
	if (k[0])
	{
		t.x = -k[1] / k[0];
		t.y = INFINITY;
		return (t);		
	}
	return ((t_vec2){INFINITY, INFINITY});
}

void	ClosestIntersection(t_traceray *tr, t_geom *geom, __constant t_object *obj,
								t_vec3 O, t_vec3 D, float t_min, float t_max)
{
	int			i;
	t_vec2		t;

	tr->closest_t = INFINITY;
	i = -1;
	while (++i < geom->objects)
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
		proj = vec_f_mult(ft_dot(N, os), os);
		N = vec_sub(N, proj);
		N = normal(N);
		return (N);
	}
	N = tr->closest_obj.direction; // PLANE
	return (N);
}

int		RayTrace(t_geom geom, __constant t_object *obj, __constant t_light *light, float t_min, float t_max)
{
	t_traceray	tr;
	t_vec3		P;
	t_vec3		N;
	t_vec3		R;
	t_vec3		local_color[DEEP + 1];
	int			deep;
	double		intensity;

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
		ClosestIntersection(&tr, &geom, obj, geom.O, geom.D, t_min, t_max);
		if (tr.closest_t == INFINITY)
		{
			local_color[deep] = (t_vec3){0,0,0};
			break ;
		}
		P = vec_add(geom.O, vec_f_mult(tr.closest_t, geom.D)); //compute intersection
		N = global_normal(P, &tr);
		t_vec3 DD = (t_vec3){-geom.D.x, -geom.D.y, -geom.D.z, 0};
		intensity = ComputeLighting(&geom, obj, light, P, N, DD, tr.closest_obj.specular); // -D
		local_color[deep] = vec_f_mult(intensity, tr.closest_obj.color);


		local_color[deep].reflection = tr.closest_obj.reflection;
		if (tr.closest_obj.reflection > 0)
		{
			R = ReflectRay(DD, N);
			geom = (t_geom){P, R, geom.camera_rot, geom.color, geom.t_min,
				geom.t_max, geom.objects, geom.lights, geom.kal, geom.kal2, geom.vp, geom.screenw, geom.screenh};
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

__kernel
void	draw(__global int *image, t_geom geom, __constant t_object *obj, __constant t_light *light)
{
	int x = get_global_id(0);
	int y = get_global_id(1);

	geom.D = rot_matrix(geom.camera_rot.x, geom.camera_rot.y, geom.camera_rot.z,
		CanvasToViewport(&geom, x - geom.screenw / 2, geom.screenh / 2 - y));
	geom.color = RayTrace(geom, obj, light, 0.001f, INFINITY);
	image[x + y * geom.screenw] = geom.color;
}
