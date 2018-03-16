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

void		read_map(t_map *m, char *filename)
{
	int		fd;
	char	*lol;
	char	**numbers;
	char	**parameters;
	char	**coordinates1;
	char	**coordinates2;

	check_filename(filename);
	if ((fd = open(filename, O_RDONLY)) < 0 || read(fd, NULL, 0) < 0)
		put_error("opening file.");
	if (get_next_line(fd, &lol) != 1 || ft_countwords(lol, ' ') != 2)
		put_error("map invalid.");
	numbers = ft_strsplit(lol, ' ');
	m->geom.objects = ft_atoi(numbers[0]);
	m->geom.lights = ft_atoi(numbers[1]);
	if (m->geom.objects <= 0 || m->geom.lights <= 0)
		put_error("wrong input scene. U can't have less that 1 object or light.");
	m->geom.obj = malloc(sizeof(t_object) * m->geom.objects);
	m->geom.light = malloc(sizeof(t_object) * m->geom.lights);
	number_action(numbers, lol);
	get_next_line(fd, &lol) != 1 ? put_error("map invalid.") : 0;
	printf("%s\n", lol);
	numbers = ft_strsplit(lol, ' ');
	if (ft_strcmp(numbers[0], "camera"))
		put_error("map invalid: camera name.");
	numbers[1] = get_from_brackets(numbers[1], '[', ']');
	printf("%s\n", numbers[0]);
	printf("%s\n", numbers[1]);
	if (ft_countwords(numbers[1], ',') != 2)
		put_error("empty numbers.");
	parameters = ft_strsplit(numbers[1], ',');
	parameters[0] = get_from_brackets(parameters[0], '{', '}');
	parameters[1] = get_from_brackets(parameters[1], '{', '}');
	printf("%s\n", parameters[0]);
	printf("%s\n", parameters[1]);
	if (ft_countwords(parameters[0], ';') != 3 || ft_countwords(parameters[1], ';') != 3)
		put_error("empty parameter.");
	coordinates1 = ft_strsplit(parameters[0], ';');
	printf("%s\n",coordinates1[0]);
	printf("%s\n",coordinates1[1]);
	printf("%s\n",coordinates1[2]);
	m->geom.O.x = ft_atoi(coordinates1[0]);
	m->geom.O.y = ft_atoi(coordinates1[1]);
	m->geom.O.z = ft_atoi(coordinates1[2]);
	printf("%f\n", m->geom.O.x);
	printf("%f\n", m->geom.O.y);
	printf("%f\n", m->geom.O.z);
	coordinates2 = ft_strsplit(parameters[1], ';');
	printf("%s\n", coordinates2[0]);
	printf("%s\n", coordinates2[1]);
	printf("%s\n", coordinates2[2]);
	m->geom.camera_rot.x = deg_to_rad(ft_atoi(coordinates2[0]));
	m->geom.camera_rot.y = deg_to_rad(ft_atoi(coordinates2[1]));
	m->geom.camera_rot.z = deg_to_rad(ft_atoi(coordinates2[2]));
	printf("%f\n", m->geom.camera_rot.x);
	printf("%f\n", m->geom.camera_rot.y);
	printf("%f\n", m->geom.camera_rot.z);
	// ft_strcmp(numbers[0], "camera") ? put_error("map invalid.") : 0;
	// printf("%s\n", parameters[0]);
	// printf("%s\n", parameters[1]);
	// !(ft_strcmp(numbers[0], "camera")) ? put_error("map invalid.") : 0;



	// if (get_next_line(fd, &lol) != 1 || ft_countwords(lol, ' ') != 2)
	// 	put_error("map invalid.");
	// numbers = ft_strsplit(lol, ' ');
	// m->pos.x = ft_atoi(numbers[0]) - 0.5;
	// m->pos.y = ft_atoi(numbers[1]) - 0.5;
	// number_action(numbers, lol);
	// if (m->pos.x <= 1 || m->pos.x > m->karta.rows - 1
	// 	|| m->pos.y <= 1 || m->pos.y > m->karta.cols - 1)
	// 	put_error("invalid player position.");
	// alloc_map(m, fd, lol, numbers);
	// check_textures(m, m->karta.data);
	// check_sprites(m, m->karta.data);
}
