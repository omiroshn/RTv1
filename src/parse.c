/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/16 18:25:32 by omiroshn          #+#    #+#             */
/*   Updated: 2018/03/16 18:25:34 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		check_filename(char *filename)
{
	if (!(ft_strcmp(filename, "scenes/scene.1")
		|| ft_strcmp(filename, "scenes/scene.2")
		|| ft_strcmp(filename, "scenes/scene.3")
		|| ft_strcmp(filename, "scenes/scene.4")
		|| ft_strcmp(filename, "scenes/scene.5")
		|| ft_strcmp(filename, "scenes/scene.6")
		|| ft_strcmp(filename, "scenes/scene.7")
		|| ft_strcmp(filename, "scenes/scene.8")
		|| ft_strcmp(filename, "scenes/scene.9")
		|| ft_strcmp(filename, "scenes/scene.10")))
		put_error("wrong input scene. Avaliable scenes : 1-10.");
}

inline static void	number_action(char **numbers, char *line)
{
	int i;

	i = -1;
	while (numbers[++i])
		free(numbers[i]);
	free(numbers);
	free(line);
}

char		*get_from_brackets(char *str, char c1, char c2)
{
	char	*tmp;
	int		i;
	int		n1;
	int		n2;

	n1 = 0;
	n2 = 0;
	if (str[0] != c1 || str[ft_strlen(str) - 1] != c2)
		put_error("wrong brackets.");
	i = -1;
	while (++i < ft_strlen(str))
	{
		str[i] == c1 ? n1 = i : 0;
		str[i] == c2 ? n2 = i : 0;
	}
	tmp = ft_strsub(str, n1 + 1, n2 - n1 - 1);
	return (tmp);
}

float		ft_ftoi(char *str)
{
	int		i;
	int		len;
	float	tmp;
	float	tmp2;

	i = -1;
	len = 0;
	tmp = ft_atoi(str);
	if (!(ft_strchr(str, '.')))
		return (tmp);
	while (str[++i] != '.')
		len++;
	tmp2 = ft_atoi(ft_strsub(str, len + 1, ft_strlen(str)));
	tmp2 /= pow(10, ft_strlen(str) - len - 1);
	tmp += tmp2;
	return (tmp);
}

/* ************************************************************************** */

void		read_camera(t_map *m, int fd, char *lol, char **numbers)
{
	char	**parameters;
	char	**coordinates;

	get_next_line(fd, &lol) != 1 ? put_error("map invalid.") : 0;
	!ft_strlen(lol) ? put_error("invalid map.") : 0;
	numbers = ft_strsplit(lol, ' ');
	if (ft_strcmp(numbers[0], "camera"))
		put_error("camera name.");
	numbers[1] = get_from_brackets(numbers[1], '[', ']');
	if (ft_countwords(numbers[1], ',') != 2)
		put_error("empty numbers.");
	parameters = ft_strsplit(numbers[1], ',');
	parameters[0] = get_from_brackets(parameters[0], '{', '}');
	parameters[1] = get_from_brackets(parameters[1], '{', '}');
	if (ft_countwords(parameters[0], ';') != 3
				|| ft_countwords(parameters[1], ';') != 3)
		put_error("empty parameter.");
	coordinates = ft_strsplit(parameters[0], ';');
	m->geom.O.x = ft_ftoi(coordinates[0]);
	m->geom.O.y = ft_ftoi(coordinates[1]);
	m->geom.O.z = ft_ftoi(coordinates[2]);
	coordinates = ft_strsplit(parameters[1], ';');
	m->geom.camera_rot.x = deg_to_rad(ft_ftoi(coordinates[0]));
	m->geom.camera_rot.y = deg_to_rad(ft_ftoi(coordinates[1]));
	m->geom.camera_rot.z = deg_to_rad(ft_ftoi(coordinates[2]));
}

/* ************************************************************************** */

void		read_objects(t_map *m, char	*objects, int i)
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
	parameters[1] = get_from_brackets(parameters[1], '{', '}');
	coordinates = ft_strsplit(parameters[1], ';');
	m->geom.obj[i].center = (t_vec3){ft_ftoi(coordinates[0]),
		ft_ftoi(coordinates[1]), ft_ftoi(coordinates[2])};
	parameters[2] = get_from_brackets(parameters[2], '{', '}');
	coordinates = ft_strsplit(parameters[2], ';');
	m->geom.obj[i].direction = (t_vec3){ft_ftoi(coordinates[0]),
		ft_ftoi(coordinates[1]), ft_ftoi(coordinates[2])};
	parameters[3] = get_from_brackets(parameters[3], '{', '}');
	coordinates = ft_strsplit(parameters[3], ';');
	m->geom.obj[i].color = (t_vec3){ft_ftoi(coordinates[0]),
		ft_ftoi(coordinates[1]), ft_ftoi(coordinates[2])};
	m->geom.obj[i].specular = ft_ftoi(parameters[4]);
	m->geom.obj[i].radius = ft_ftoi(parameters[5]);
	m->geom.obj[i].reflection = ft_ftoi(parameters[6]);
	m->geom.obj[i].angle = ft_ftoi(parameters[7]);
}

/* ************************************************************************** */

void		read_lights(t_map *m, char *objects, int i)
{
	char	**parameters;
	char	**coordinates;

	if (ft_countwords(objects, ',') != 4)
		put_error("empty numbers.");
	parameters = ft_strsplit(objects, ',');
	m->geom.light[i].type = ft_atoi(parameters[0]);;
	m->geom.light[i].intensity = ft_ftoi(parameters[1]);
	if (ft_countwords(parameters[2], ';') != 3
			|| ft_countwords(parameters[3], ';') != 3)
		put_error("empty parameter.");
	parameters[2] = get_from_brackets(parameters[2], '{', '}');
	coordinates = ft_strsplit(parameters[2], ';');
	m->geom.light[i].position = (t_vec3){ft_ftoi(coordinates[0]),
		ft_ftoi(coordinates[1]), ft_ftoi(coordinates[2])};
	parameters[3] = get_from_brackets(parameters[3], '{', '}');
	coordinates = ft_strsplit(parameters[3], ';');
	m->geom.light[i].direction = (t_vec3){ft_ftoi(coordinates[0]),
		ft_ftoi(coordinates[1]), ft_ftoi(coordinates[2])};
}

/* ************************************************************************** */

void		read_map(t_map *m, char *filename)
{
	int		fd;
	char	*lol;
	char	**numbers;

	check_filename(filename);
	if ((fd = open(filename, O_RDONLY)) < 0 || read(fd, NULL, 0) < 0)
		put_error("opening file.");
	if (get_next_line(fd, &lol) != 1 || ft_countwords(lol, ' ') != 2)
		put_error("map invalid.");
	!ft_strlen(lol) ? put_error("invalid map.") : 0;
	numbers = ft_strsplit(lol, ' ');
	m->geom.objects = ft_atoi(numbers[0]);
	m->geom.lights = ft_atoi(numbers[1]);
	if (m->geom.objects <= 0 || m->geom.lights <= 0)
		put_error("you can't have less that 1 object or light.");
	m->geom.obj = malloc(sizeof(t_object) * m->geom.objects);
	m->geom.light = malloc(sizeof(t_object) * m->geom.lights);
	number_action(numbers, lol);
	read_camera(m, fd, lol, numbers);

	int o = 0;
	int l = 0;
	while (get_next_line(fd, &lol))
	{
		!ft_strlen(lol) ? put_error("invalid map.") : 0;
		numbers = ft_strsplit(lol, ' ');
		if (!(ft_strcmp(numbers[0], "object")))
			read_objects(m, get_from_brackets(numbers[1], '[', ']'), o++);
		else if (!(ft_strcmp(numbers[0], "light")))
			read_lights(m, get_from_brackets(numbers[1], '[', ']'), l++);
		else
			put_error("wrong object or light name.");
	}
	if (o != m->geom.objects || l != m->geom.lights )
		put_error("wrong number of objects or lights.");
	close(fd);
}
