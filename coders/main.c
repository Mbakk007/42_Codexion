/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 15:17:50 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/04 23:49:37 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	print_usage(void)
{
	write(2, "Usage: ./codexion n_coders t_burnout t_compile t_debug", 54);
	write(2, " t_refactor must_compile dongle_cooldown fifo|edf\n", 50);
}

static void	print_error(void)
{
	write(2, "Error: invalid arguments (numbers must be > 0", 45);
	write(2, "scheduler must be fifo|edf)\n", 29);
}

int	main(int argc, char **argv)
{
	t_params	p;
	t_sim		sim;

	if (argc != 9)
		return (print_error(), 1);
	if (!parse_args(&p, argc, argv))
		return (print_error(), 1);
	if (!sim_init(&sim, &p))
		return (1);
	if (!sim_run(&sim))
	{
		write(2, "Error: simulation failed\n", 25);
		sim_destroy(&sim);
		return (1);
	}
	sim_destroy(&sim);
	return (0);
}
