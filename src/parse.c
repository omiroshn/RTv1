#include "rt.h"

void		check_filename(char *filename)
{
	if (!(ft_strcmp(filename, "scenes/scene.1") || ft_strcmp(filename, "scenes/scene.2")
		|| ft_strcmp(filename, "scenes/scene.3") || ft_strcmp(filename, "scenes/scene.4")
		|| ft_strcmp(filename, "scenes/scene.5") || ft_strcmp(filename, "scenes/scene.6")
		|| ft_strcmp(filename, "scenes/scene.7") || ft_strcmp(filename, "scenes/scene.8")
		|| ft_strcmp(filename, "scenes/scene.9") || ft_strcmp(filename, "scenes/scene.10")))
		put_error("wrong input scene. Avaliable scenes : 1-10.");
}

inline static void	number_action(char **numbers, char *line)
{
	int		i;

	i = -1;
	while (numbers[++i])
		free(numbers[i]);
	free(numbers);
	free(line);
}

void		read_map(t_map *m, char *filename)
{
	int		fd;
	char	*lol;
	char	**numbers;
	char	**parameters;
	char	**coordinates;

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
	number_action(numbers, lol);
	get_next_line(fd, &lol) != 1 ? put_error("map invalid.") : 0;
	numbers = ft_strsplit(lol, ' ');
	printf("%s\n", numbers[0]);
	printf("%s\n", numbers[1]);
	if (ft_strcmp(numbers[0], "camera"))
	{
		 put_error("map invalid.");
	}
	// ft_strcmp(numbers[0], "camera") ? put_error("map invalid.") : 0;
	parameters = ft_strsplit(lol, ',');
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