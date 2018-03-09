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

__kernel void hello(__global char *string)
{
	string[0] = 'H';
	string[1] = 'e';
	string[2] = 'l';
	string[3] = 'l';
	string[4] = 'o';
	string[5] = ',';
	string[6] = ' ';
	string[7] = 'W';
	string[8] = 'o';
	string[9] = 'r';
	string[10] = 'l';
	string[11] = 'd';
	string[12] = '!';
	string[13] = '\0';
}
