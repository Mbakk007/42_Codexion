/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 15:17:50 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/02 16:28:37 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_params	p;
	t_sim		sim;

	if (argc != 9)
	{
		write(2, "Usage: ./codexion n_coders t_burnout t_compile t_debug t_refactor must_compile dongle_cooldown fifo|edf\n", 102);
		return (1);
	}
	if (!parse_args(&p, argc, argv))
	{
		write(2, "Error: invalid arguments (numbers must be >0, scheduler must be fifo|edf)\n", 74);
		return (1);
	}
	if (!sim_init(&sim, &p))
	{
		write(2, "Error: sim_init failed\n", 23);
		return (1);
	}
	if (!sim_run(&sim))
	{
		write(2, "Error: simulation failed\n", 25);
		sim_destroy(&sim);
		return (1);
	}
	sim_destroy(&sim);
	return (0);
}
