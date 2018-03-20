/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 19:54:00 by omiroshn          #+#    #+#             */
/*   Updated: 2018/03/20 19:54:02 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		read_camera2(t_map *m, char **coordinates)
{
	if (ft_countwords(m->tmp2, ';') != 3
		|| ft_countwords(m->tmp3, ';') != 3)
		put_error("empty parameter.");
	coordinates = ft_strsplit(m->tmp2, ';');
	m->geom.o.x = ft_ftoi(coordinates[0]);
	m->geom.o.y = ft_ftoi(coordinates[1]);
	m->geom.o.z = ft_ftoi(coordinates[2]);
	number_action(coordinates, m->tmp2);
	coordinates = ft_strsplit(m->tmp3, ';');
	m->geom.camera_rot.x = deg_to_rad(ft_ftoi(coordinates[0]));
	m->geom.camera_rot.y = deg_to_rad(ft_ftoi(coordinates[1]));
	m->geom.camera_rot.z = deg_to_rad(ft_ftoi(coordinates[2]));
	number_action(coordinates, m->tmp3);
}

void		read_camera(t_map *m, int fd, char *lol, char **numbers)
{
	char	**parameters;
	char	**coordinates;

	get_next_line(fd, &lol) != 1 ? put_error("map invalid.") : 0;
	!ft_strlen(lol) ? put_error("invalid map.") : 0;
	numbers = ft_strsplit(lol, ' ');
	if (ft_strcmp(numbers[0], "camera"))
		put_error("camera name.");
	m->tmp = get_from_brackets(numbers[1], '[', ']');
	if (ft_countwords(m->tmp, ',') != 2)
		put_error("empty numbers.");
	parameters = ft_strsplit(m->tmp, ',');
	number_action(numbers, m->tmp);
	m->tmp2 = get_from_brackets(parameters[0], '{', '}');
	m->tmp3 = get_from_brackets(parameters[1], '{', '}');
	number_action(parameters, lol);
	read_camera2(m, coordinates);
}

void		read_objects2(t_map *m, char **coordinates,
								char **parameters, int i)
{
	coordinates = ft_strsplit(m->tmp3, ';');
	m->geom.obj[i].color = (t_vec3){ft_ftoi(coordinates[0]),
		ft_ftoi(coordinates[1]), ft_ftoi(coordinates[2])};
	number_action(coordinates, m->tmp2);
	m->geom.obj[i].specular = ft_ftoi(parameters[4]);
	m->geom.obj[i].radius = ft_ftoi(parameters[5]);
	m->geom.obj[i].reflection = ft_ftoi(parameters[6]);
	m->geom.obj[i].angle = ft_ftoi(parameters[7]);
	number_action(parameters, m->tmp3);
}

void		read_objects(t_map *m, char *objects, int i)
{
	char	**parameters;
	char	**coordinates;

	if (ft_countwords(objects, ',') != 8)
		put_error("empty numbers.");
	parameters = ft_strsplit(objects, ',');
	m->geom.obj[i].name = ft_atoi(parameters[0]);
	if (ft_countwords(parameters[1], ';') != 3
		|| ft_countwords(parameters[2], ';') != 3
		|| ft_countwords(parameters[3], ';') != 3)
		put_error("empty parameter.");
	m->tmp = get_from_brackets(parameters[1], '{', '}');
	coordinates = ft_strsplit(m->tmp, ';');
	m->geom.obj[i].center = (t_vec3){ft_ftoi(coordinates[0]),
		ft_ftoi(coordinates[1]), ft_ftoi(coordinates[2])};
	number_action(coordinates, objects);
	m->tmp2 = get_from_brackets(parameters[2], '{', '}');
	coordinates = ft_strsplit(m->tmp2, ';');
	m->geom.obj[i].direction = (t_vec3){ft_ftoi(coordinates[0]),
		ft_ftoi(coordinates[1]), ft_ftoi(coordinates[2])};
	number_action(coordinates, m->tmp);
	m->tmp3 = get_from_brackets(parameters[3], '{', '}');
	read_objects2(m, coordinates, parameters, i);
}

void		read_lights(t_map *m, char *objects, int i)
{
	char	**parameters;
	char	**coordinates;

	if (ft_countwords(objects, ',') != 4)
		put_error("empty numbers.");
	parameters = ft_strsplit(objects, ',');
	m->geom.light[i].type = ft_atoi(parameters[0]);
	m->geom.light[i].intensity = ft_ftoi(parameters[1]);
	if (ft_countwords(parameters[2], ';') != 3
		|| ft_countwords(parameters[3], ';') != 3)
		put_error("empty parameter.");
	m->tmp = get_from_brackets(parameters[2], '{', '}');
	coordinates = ft_strsplit(m->tmp, ';');
	m->geom.light[i].position = (t_vec3){ft_ftoi(coordinates[0]),
		ft_ftoi(coordinates[1]), ft_ftoi(coordinates[2])};
	m->tmp2 = get_from_brackets(parameters[3], '{', '}');
	number_action(coordinates, objects);
	coordinates = ft_strsplit(m->tmp2, ';');
	m->geom.light[i].direction = (t_vec3){ft_ftoi(coordinates[0]),
		ft_ftoi(coordinates[1]), ft_ftoi(coordinates[2])};
	number_action(coordinates, m->tmp);
	number_action(parameters, m->tmp2);
}
