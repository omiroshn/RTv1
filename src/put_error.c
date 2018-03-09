/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 21:42:33 by omiroshn          #+#    #+#             */
/*   Updated: 2018/02/28 21:42:34 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	put_error(const char *msg)
{
	if (!msg)
		exit(-1);
	ft_putendl(ft_strjoin("Error: ", msg));
	exit(-1);
}

void	put_usage(void)
{
	ft_putendl("Usage: ./RTv1 resources/maps/map1.map");
	exit(-1);
}

int		quit(t_map *m)
{
	SDL_DestroyWindow(m->window);
	IMG_Quit();
	SDL_Quit();
	return (0);
}
