/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jescully <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 13:24:30 by jescully          #+#    #+#             */
/*   Updated: 2022/01/14 14:14:49 by jescully         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	main(int argc, char **argv)
{
	int	*parsed_args;

	parsed_args = parse_args(argc, argv);
	if (parsed_args)
	{
		birth_philosophers(parsed_args[0], &parsed_args[1]);
		free(parsed_args);
	}
	return (0);
}
