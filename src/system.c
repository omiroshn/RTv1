/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 17:35:52 by omiroshn          #+#    #+#             */
/*   Updated: 2018/03/20 17:35:53 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

float	deg_to_rad(float degree)
{
	float	radians;

	radians = (degree * M_PI) / 180;
	return (radians);
}

void	number_action(char **numbers, char *line)
{
	int i;

	i = -1;
	while (numbers[++i])
		free(numbers[i]);
	free(numbers);
	free(line);
}

char	*get_from_brackets(char *str, char c1, char c2)
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

float	ft_ftoi(char *str)
{
	int		i;
	int		len;
	float	tmp;
	float	tmp2;
	char	*huimorja;

	i = -1;
	len = 0;
	tmp = ft_atoi(str);
	if (!(huimorja = ft_strchr(str, '.')))
		return (tmp);
	while (str[++i] != '.')
		len++;
	tmp2 = ft_atoi(huimorja + 1);
	tmp2 /= pow(10, ft_strlen(str) - len - 1);
	tmp += tmp2;
	return (tmp);
}
