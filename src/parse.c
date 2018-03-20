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

void		readasd(t_map *m, int fd, char *lol)
{
	int		o;
	int		l;
	char	**numbers;

	o = 0;
	l = 0;
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
		number_action(numbers, lol);
	}
	free(lol);
	if (o != m->geom.objects || l != m->geom.lights)
		put_error("wrong number of objects or lights.");
}

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
	readasd(m, fd, lol);
	close(fd);
}
