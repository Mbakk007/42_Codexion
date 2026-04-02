/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 15:17:50 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/02 15:38:09 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_params	p;

	if (argc != 9)
	{
		fprintf(stderr, "Usage: ./codexion n_coders t_burnout t_compile t_debug "
			"t_refactor must_compile dongle_cooldown fifo|edf\n");
		return (1);
	}
	if (!is_scheduler_ok(argv[8]))
	{
		fprintf(stderr, "Error: scheduler must be fifo or edf\n");
		return (1);
	}
	if (!parse_args(&p, argc, argv))
	{
		fprintf(stderr, "Error: invalid numeric arguments\n");
		return (1);
	}
	printf("OK n=%d burnout=%ld compile=%ld debug=%ld ref=%ld must=%d cd=%ld sched=%s\n",
	p.n_coders, p.t_burnout, p.t_compile, p.t_debug, p.t_refactor,
	p.must_compile_count, p.dongle_cooldown, argv[8]);
	return (0);
}