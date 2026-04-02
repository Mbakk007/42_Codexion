/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 11:03:45 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/02 15:41:40 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_scheduler_ok(const char *s)
{
	if (ft_strcmp(s, "fifo") == 0)
		return (1);
	if (ft_strcmp(s, "edf") == 0)
		return (1);
	return (0);
}

int	parse_n_coders(int *out, const char *s)
{
	int	v;

	if (!is_digits(s))
		return (0);
	v = ft_atoi(s);
	if (v <= 0)
		return (0);
	*out = v;
	return (1);
}
int	parse_pos_long(long *out, const char *s)
{
	long	v;

	if (!is_digits(s))
		return (0);
	v = ft_atol(s);
	if (v <= 0)
		return (0);
	*out = v;
	return (1);
}

int	parse_args(t_params *p, int argc, char **argv)
{
	if (argc != 9)
		return (0);
	if (!parse_n_coders(&p->n_coders, argv[1]))
		return (0);
	if (!parse_pos_long(&p->t_burnout, argv[2]))
		return (0);
	if (!parse_pos_long(&p->t_compile, argv[3]))
		return (0);
	if (!parse_pos_long(&p->t_debug, argv[4]))
		return (0);
	if (!parse_pos_long(&p->t_refactor, argv[5]))
		return (0);
	if (!parse_pos_int(&p->must_compile_count, argv[6]))
		return (0);
	if (!parse_pos_long(&p->dongle_cooldown, argv[7]))
		return (0);
	return (1);
}
int	parse_pos_int(int *out, const char *s)
{
	int	v;

	if (!is_digits(s))
		return (0);
	v = ft_atoi(s);
	if (v <= 0)
		return (0);
	*out = v;
	return (1);
}


