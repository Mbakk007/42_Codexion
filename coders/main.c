/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 13:34:00 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/01 16:30:08 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	monitor_simulation(t_simulation *sim)
{
	int		i;
	long	current_time;

	while (1)
	{
		pthread_mutex_lock(&sim->state_mutex);
		if (sim->someone_burned_out || sim->all_done)
		{
			sim->simulation_running = 0;
			pthread_mutex_unlock(&sim->state_mutex);
			break ;
		}
		pthread_mutex_unlock(&sim->state_mutex);
		if (!sim->has_started)
		{
			usleep(100);
			continue ;
		}
		i = 0;
		while (i < sim->params.number_of_coders)
		{
			current_time = get_elapsed_ms(sim);
			if (current_time >= sim->coders[i].deadline
				&& sim->coders[i].state != BURNED_OUT)
			{
				pthread_mutex_lock(&sim->state_mutex);
				sim->coders[i].state = BURNED_OUT;
				log_message(sim, &sim->coders[i], "burned out");
				sim->someone_burned_out = 1;
				sim->simulation_running = 0;
				pthread_mutex_unlock(&sim->state_mutex);
				break ;
			}
			i++;
		}
		usleep(100);
	}
}

int	main(int argc, char **argv)
{
	t_simulation	sim;
	char			*usage;

	usage = "Usage: ./codexion [number_of_coders] [time_to_burnout_ms] "
		"[time_to_compile_ms] [time_to_debug_ms] [time_to_refactor_ms] "
		"[number_of_compiles_required] [dongle_cooldown_ms] "
		"[sched(fifo or edf)]\n";
	if (init_simulation(&sim, argc, argv) != 0)
	{
		fprintf(stderr, "Failed to initialize simulation\n");
		fprintf(stderr, "%s", usage);
		return (1);
	}
	monitor_simulation(&sim);
	destroy_simulation(&sim);
	return (0);
}
