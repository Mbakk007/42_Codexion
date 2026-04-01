/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 11:03:45 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/01 14:09:00 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_digit(char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	parse_long(char *s, long *result)
{
	int		i;
	int		d;
	long	l;

	if (!result || !is_digit(s))
		return (0);
	i = 0;
	l = 0;
	while (s[i])
	{
		d = s[i] - '0';
		if (l > (LONG_MAX - d) / 10)
			return (0);
		l = l * 10 + d;
		i++;
	}
	*result = l;
	return (1);
}

static int	parse_sched(char *s, t_sched *result)
{
	if (!result || !s)
		return (0);
	if (strcmp(s, "fifo") == 0)
	{
		*result = FIFO_SCHED;
		return (1);
	}
	if (strcmp(s, "edf") == 0)
	{
		*result = EDF_SCHED;
		return (1);
	}
	return (0);
}

int	parse_args(t_args *output, int argc, char **argv)
{
	t_args	tmp;

	if (!output || argc != 9)
		return (0);
	if (!parse_long(argv[1], (long *)&tmp.number_of_coders)
		|| tmp.number_of_coders < 1 || !parse_long(argv[2],
			&tmp.time_to_burnout_ms) || tmp.time_to_burnout_ms <= 0
		|| !parse_long(argv[3], &tmp.time_to_compile_ms)
		|| tmp.time_to_compile_ms <= 0 || !parse_long(argv[4],
			&tmp.time_to_debug_ms) || tmp.time_to_debug_ms <= 0
		|| !parse_long(argv[5], &tmp.time_to_refactor_ms)
		|| tmp.time_to_refactor_ms <= 0 || !parse_long(argv[6],
			(long *)&tmp.number_of_compiles_required)
		|| tmp.number_of_compiles_required < 1 || !parse_long(argv[7],
			&tmp.dongle_cooldown_ms) || tmp.dongle_cooldown_ms < 0
		|| !parse_sched(argv[8], &tmp.sched))
		return (0);
	*output = tmp;
	return (1);
}
