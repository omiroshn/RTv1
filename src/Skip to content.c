Skip to content
This repository
Search
Pull requests
Issues
Marketplace
Explore
 @omiroshn
 Sign out
0
0 0 kushnirs/rtv1
 Code  Issues 0  Pull requests 0  Projects 0  Wiki  Insights
rtv1/kernel/rtv1.cl
57a67ba  8 days ago
 Sergii KUSHNIR done
     
465 lines (428 sloc)  10.9 KB
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skushnir <skushnir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/06 09:09:57 by skushnir          #+#    #+#             */
/*   Updated: 2018/02/24 13:18:49 by skushnir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1_cl.h"
/*
**
***********************************COLOR****************************************
**
*/
static int	parse_color(int c1, int c2, float t)
{
	int dr;
	int dg;
	int db;

	dr = (1 - t) * (float)(c1 / 0x10000 % 256) + t * (float)(c2 / 0x10000 % 256);
	dg = (1 - t) * (float)(c1 / 0x100 % 256) + t * (float)(c2 / 0x100 % 256);
	db = (1 - t) * (float)(c1 % 256) + t * (float)(c2 % 256);
	return (dr * 0x10000 + dg * 0x100 + db);
}

static int	refl_color(t_color *color, int deep)
{
	int	i;

	i = 0;
	while(i < deep)
	{
		color[i + 1].color = parse_color(0, color[i + 1].color, 1 - color[i + 1].reflection) +
			parse_color(0, color[i].color, color[i + 1].reflection);
		i++;
	}
	return (color[i].color);
}

static int	average_color(int *color, int smooth)
{
	int	i;
	int d[3] = {0, 0, 0};

	i = 0;
	while( i < smooth * smooth)
	{
		d[0] += (float)(color[i] / 0x10000 % 256);
		d[1] += (float)(color[i] / 0x100 % 256);
		d[2] += (float)(color[i] % 256);
		i++;
	}
	d[0] /= smooth * smooth;
	d[1] /= smooth * smooth;
	d[2] /= smooth * smooth;
	return (d[0] * 0x10000 + d[1] * 0x100 + d[2]);
}
/*
**
***********************************CONVERT**************************************
**
*/
static	void	convert_obj(__constant t_o *o, t_obj *obj, int num)
{
	while (num--)
	{
		obj[num].name = o[num].name;
		obj[num].c = (float3){o[num].c.x, o[num].c.y, o[num].c.z};
		obj[num].d = (float3){o[num].d.x, o[num].d.y, o[num].d.z};
		if (obj[num].name == PLANE)
			obj[num].c = cam_rot(obj[num].d, obj[num].c);
		if (obj[num].name == SPHERE)
			obj[num].c = cam_rot(obj[num].d, obj[num].c);
		obj[num].radius = o[num].radius;
		obj[num].color = o[num].color;
		obj[num].specular = o[num].specular;
		obj[num].reflection = o[num].reflection;
	}
}

static	void	convert_light(__constant t_l *l, t_light *light, int num)
{
	while (num--)
	{
		light[num].type = l[num].type;
		light[num].intensity = l[num].intensity;
		light[num].direction = (float3){l[num].direction.x,
			l[num].direction.y, l[num].direction.z};
	}
}

static	t_scene	convert_scene(t_s s)
{
	t_scene scene;

	scene.o = (float3){s.o.x, s.o.y, s.o.z};
	scene.d = (float3){s.d.x, s.d.y, s.d.z};
	scene.cam_rot = (float3){s.cam_rot.x, s.cam_rot.y, s.cam_rot.z};
	scene.canvas = (float3){s.canvas.x, s.canvas.y, s.canvas.z};
	scene.viewport = (float3){s.viewport.x, s.viewport.y, s.viewport.z};
	scene.t_min = s.t_min;
	scene.t_max = s.t_max;
	scene.n_o = s.n_o;
	scene.n_l = s.n_l;
	return (scene);
}
/*
**
***********************************UTILITY**************************************
**
*/
float3	cam_rot(float3 rot, float3 coord)
{
	float3	angle;
	float3	p[3];
	float	func[6];

	angle.x = PI * rot.x / 180.0f;
	func[0] = cos(angle.x);
	func[1] = sin(angle.x);
	angle.y = PI * rot.y / 180.0f;
	func[2] = cos(angle.y);
	func[3] = sin(angle.y);
	angle.z = PI * rot.z / 180.0f;
	func[4] = cos(angle.z);
	func[5] = sin(angle.z);

	p[0].x = coord.x;
	p[0].y = coord.y * func[0] + coord.z * func[1];
	p[0].z = coord.z * func[0] - coord.y * func[1];

	p[1].x = p[0].x * func[2] + p[0].z * func[3];
	p[1].y = p[0].y;
	p[1].z = p[0].z * func[2] - p[0].x * func[3];

	p[2].x = p[1].x * func[4] + p[1].y * func[5];
	p[2].y = p[1].y * func[4] - p[1].x * func[5];
	p[2].z = p[1].z;
	return (p[2]);
}

float2	q_equation(float k[3])
{
	float2	t;
	float	disc;

	disc = k[1] * k[1] - 4.0f * k[0] * k[2];
	if (disc < 0)
		return ((float2){INFINITY, INFINITY});
	t.x = (-k[1] + sqrt(disc)) / (2.0f * k[0]);
	t.y = (-k[1] - sqrt(disc)) / (2.0f * k[0]);
	return (t);
}
float3	v_normal(float3 p, t_closest closest)
{
	float3		proj;
	float3		t;
	float3		n;
	
	if (closest.closest_obj.name == CYLINDER || closest.closest_obj.name == CONE)
	{
		t = (closest.closest_obj.d - closest.closest_obj.c) /
			length(closest.closest_obj.d - closest.closest_obj.c);
		n = p - closest.closest_obj.c;
		proj = t * dot(n, t);
		n = n - proj;
		n = n / length(n);
		return (n);
	}
	else if (closest.closest_obj.name == PLANE)
	{
		n = -closest.closest_obj.c / length(-closest.closest_obj.c);
		return (n);
	}
	n = p - closest.closest_obj.c;
	n = n / length(n);
	return (n);
}
/*
**
***********************************RAY_OBJ**************************************
**
*/
float2	raysphere(float3 o, float3 d, t_obj obj)
{
	float3	oc;
	float2	t;
	float	k[3];

	oc = o - obj.c;
	k[0] = dot(d, d);
	k[1] = 2.0f * dot(oc, d);
	k[2] = dot(oc, oc) - obj.radius * obj.radius;
	t = q_equation(k);
	return (t);
}

float intersect_cyl_con(float3 d, float3 o, float3 v, t_obj obj, float t)
{
	float3	p;
	float3	a;
	float k[2];

	if (t < 0)
		return (INFINITY);
	p = d * t + o;
	a = p - obj.c;
	k[0] = dot(v, a);
	a = p - obj.d;
	k[1] = dot(v, a);
	if (k[0] > 0 && k[1] < 0 && t > 0)
		return (t);
	return (INFINITY);
}

float2	raycylinder(float3 o, float3 d, t_obj obj)
{
	float3	p;
	float3	v;
	float2	t;
	float3	a[2];
	float	k[3];

	v = (obj.d - obj.c) / length(obj.d - obj.c);
	p = o - obj.c;
	a[0] = d - v * dot(d, v);
	k[0] = dot(a[0], a[0]);
	a[1] = p - v * dot(p, v);
	k[1] = 2.0f * dot(a[0], a[1]);
	k[2] = dot(a[1], a[1]) - obj.radius * obj.radius;
	t = q_equation(k);
	t.x = intersect_cyl_con(d, o, v, obj, t.x);
	t.y = intersect_cyl_con(d, o, v, obj, t.y);
	return (t);
}

float2	raycone(float3 o, float3 d, t_obj obj)
{
	float3	p;
	float3	v;
	float2	t;
	float3	a[2];
	float	angle;
	float	k[3];

	angle = PI * obj.radius / 180;
	v = (obj.d - obj.c) / length(obj.d - obj.c);
	p = o - obj.d;
	a[0] = d - v * dot(d, v);
	k[0] = cos(angle) * cos(angle) * dot(a[0], a[0]);
	k[0] -= sin(angle) * sin(angle) * dot(d, v) * dot(d, v);
	a[1] = p - v * dot(p, v);
	k[1] = 2.0f * cos(angle) * cos(angle) * dot(a[0], a[1]);
	k[1] -= 2.0f * sin(angle) * sin(angle) * dot(d, v) * dot(p, v);
	k[2] = cos(angle) * cos(angle) * dot(a[1], a[1]);
	k[2] -= sin(angle) * sin(angle) * dot(p, v) * dot(p, v);
	t = q_equation(k);
	t.x = intersect_cyl_con(d, o, v, obj, t.x);
	t.y = intersect_cyl_con(d, o, v, obj, t.y);
	return (t);
}

float2	rayplane(float3 o, float3 d, t_obj obj)
{
	float3	oc;
	float3	n;
	float2	t;
	float	k[2];

	n = -obj.c / length(-obj.c);
	oc = o - obj.c;
	k[0] = dot(d, n);
	k[1] = dot(oc, n);
	if (k[0])
	{
		t.x = -k[1] / k[0];
		t.y = INFINITY;
		return (t);		
	}
	return ((float2){INFINITY, INFINITY});
}
/*
**
***********************************LIGHT****************************************
**
*/
float	ft_p_d(float3 l, float3 n, float3 v, int s, float intens)
{
	float		rv;
	float		nl;
	float		i;
	float3		r;

	i = 0.0f;
	nl = dot(n, l);
	nl > 0.0f ? i += intens * nl / (length(n) * length(l)) : 0;
	if (s >= 0)
	{
		r = 2.0f * n * dot(n, l) - l;
		rv = dot(r, v);
		rv > 0.0f ? i += intens * pow(rv / (length(r) * length(v)), s) : 0;
	}
	return (i);
}

float3	ft_light_p_d(float3 p, t_light light, t_obj *obj, int n_o, int n_l)
{
	t_closest	closest;
	float		max;
	float3		l;

	if (light.type == POINT)
	{
		l = light.direction - p;
		max = 1.0f;
	}
	else
	{
		l = light.direction;
		max = INFINITY;
	}
	closest = intersections((t_scene){p, l, (float3){0,0,0}, (float3){0,0,0},
		(float3){0,0,0}, 0.001f, max, n_o, n_l}, obj);
	if (closest.closest_obj.color)
		return ((float3){INFINITY, INFINITY, 0});
	return (l);
}

float	ft_light(float3 *pnv, int s, t_light *light, t_obj *obj, int n_o, int n_l)
{
	int			a;
	float		i;
	float3		l;

	i = 0.0f;
	a = -1;
	while (++a < n_l)
	{
		if (light[a].type == AMBIENT)
			i += light[a].intensity;
		else
		{
			l = ft_light_p_d(pnv[0], light[a], obj, n_o, n_l);
			if (l.x != INFINITY && l.y != INFINITY)
				i += ft_p_d(l, pnv[1], pnv[2], s, light[a].intensity);
		}
	}
	i > 1.0f ? i = 1.0f : 0;
	return (i);
}
/*
**
***********************************MAIN*****************************************
**
*/
t_closest	intersections(t_scene scene, t_obj *obj)
{
	t_closest	closest;
	float2		t;
	int			i;

	closest.closest_obj.color = 0;
	closest.c_t = scene.t_max;
	i = -1;
	while (++i < scene.n_o)
	{
		if (obj[i].name == CYLINDER)
			t = raycylinder(scene.o, scene.d, obj[i]);
		else if (obj[i].name == SPHERE)
			t = raysphere(scene.o, scene.d, obj[i]);
		else if (obj[i].name == PLANE)
			t = rayplane(scene.o, scene.d, obj[i]);
		else if (obj[i].name == CONE)
			t = raycone(scene.o, scene.d, obj[i]);
		if (t.x > scene.t_min && t.x < scene.t_max && t.x < closest.c_t)
		{
			closest.c_t = t.x;
			closest.closest_obj = obj[i];
		}
		if (t.y > scene.t_min && t.y < scene.t_max && t.y < closest.c_t)
		{
			closest.c_t = t.y;
			closest.closest_obj = obj[i];
		}
	}
	return (closest);
}

int	raytrace(t_scene scene, t_obj *obj, t_light *light)
{
	t_color		color[DEEP + 1];
	t_closest	closest;
	float3		p;
	float3		n;
	float3		r;
	int			deep = DEEP;

	while (deep >= 0)
	{
		closest = intersections(scene, obj);
		if (!closest.closest_obj.color)
			return (0);
		p = scene.o + scene.d * closest.c_t;
		n = v_normal(p, closest);
		color[deep].color = parse_color(0, closest.closest_obj.color,
			ft_light((float3[3]){p, n, -scene.d}, closest.closest_obj.specular,
				light, obj, scene.n_o, scene.n_l));
		color[deep].reflection = closest.closest_obj.reflection;
		if (deep > 0 && closest.closest_obj.reflection > 0)
		{
			r = 2.0f * n * dot(n, -scene.d) - -scene.d;
			scene = (t_scene){p, r, scene.cam_rot, scene.canvas, scene.viewport,
				0.001f, INFINITY, scene.n_o, scene.n_l};
			deep--;
		}
		else
			break;
	}
	DEEP ? color[DEEP].color = refl_color(color, DEEP) : 0;
	return (color[DEEP].color);
}

__kernel
void	draw_scene(__global int *buff, t_s s, __constant t_o *o, __constant t_l *l)
{
	int			x = get_global_id(0);
	int			y = get_global_id(1);
	int			i;
	int			row;
	int			col;
	int			color[4];
	int			smooth;
	t_scene		scene;
	t_light		light[20];
	t_obj		obj[20];

	scene = convert_scene(s);
	convert_obj(o, obj, scene.n_o);
	convert_light(l, light, scene.n_l);

	smooth = 2;
	i = 0;
	row = -1;
	while (++row < smooth)
	{
		col = -1;
		while(++col < smooth)
		{
			scene.d = (float3){(x - scene.canvas.x / 2.0f + (row + 0.5f) / smooth)
				* scene.viewport.x / scene.canvas.x,
			(scene.canvas.y / 2.0f - y + (col + 0.5f) / smooth)
				* scene.viewport.y / scene.canvas.y, 100};
			scene.d = cam_rot(scene.cam_rot, scene.d);
			color[i++] = raytrace(scene, obj, light);
		}
	}
	buff[x + y * (int)scene.canvas.x] = average_color(color, smooth);
}
© 2018 GitHub, Inc.
Terms
Privacy
Security
Status
Help
Contact GitHub
API
Training
Shop
Blog
About