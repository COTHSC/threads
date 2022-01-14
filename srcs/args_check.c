/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jescully <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 13:54:26 by jescully          #+#    #+#             */
/*   Updated: 2022/01/14 16:13:32 by jescully         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static int	check_arg_count(int argc)
{
	if (argc < 5)
	{
		printf("Error: not enough arguments\n");
		return (1);
	}
	if (argc > 6)
	{
		printf("Error: too many arguments\n");
		return (1);
	}
	return (0);
}

static int	check_args_value(char **argv)
{
	int	i;
	int	d;

	i = 0;
	while (argv[i])
	{
		d = 0;
		while (argv[i][d])
		{
			if (!ft_isdigit(argv[i][d]))
			{
				printf("Error, your arguments are bad.\n");
				return (1);
			}
			d++;
		}
		i++;
	}
	return (0);
}

int	*parse_args(int argc, char **argv)
{
	int	i;
	int	*parsed_args;

	if (check_arg_count(argc))
		return (NULL);
	if (check_args_value(&argv[1]))
		return (NULL);
	parsed_args = ft_calloc(sizeof(int), 7);
	parsed_args[4] = -1;
	i = 0;
	while (i < argc - 1)
	{
		parsed_args[i] = ft_atoi(argv[i + 1]);
		i++;
	}
	return (parsed_args);
}
