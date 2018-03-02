/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/02 18:10:14 by omiroshn          #+#    #+#             */
/*   Updated: 2018/03/02 18:10:16 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include <time.h>
# include <fcntl.h>
# include <SDL2/SDL.h>
# include "libft.h"
# include "SDL_ttf.h"
# include "SDL_image.h"
# include "SDL_mixer.h"

# define WIDTH 1280
# define HEIGHT 1024
# define THREADS 8
# define TRUE 1

typedef unsigned int	t_uint;

typedef	struct	s_vec
{
	double		x;
	double		y;
}				t_vec;

typedef	struct	s_ivec
{
	int			x;
	int			y;
}				t_ivec;


typedef	struct	s_map
{
	SDL_Surface	*screen;
	SDL_Window	*window;
	t_uint		*image;
	t_uint		*bufp;
}				t_map;

#endif
