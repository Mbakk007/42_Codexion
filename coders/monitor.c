/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 17:11:40 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/02 17:35:57 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	all_done(t_sim *sim)
{
	int	i;
	int	done;

	if (sim->params.must_compile_count <= 0)
		return (0);
	i = 0;
	while (i < sim->params.n_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_mtx);
		done = (sim->coders[i].compile_count >= sim->params.must_compile_count);
		pthread_mutex_unlock(&sim->coders[i].state_mtx);
		if (!done)
			return (0);
		i++;
	}
	return (1);
}

static int	check_burnout(t_sim *sim, int *out_id)
{
	int		i;
	long	now;
	long	last;

	now = now_ms();
	i = 0;
	while (i < sim->params.n_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_mtx);
		last = sim->coders[i].last_compile_start_ms;
		pthread_mutex_unlock(&sim->coders[i].state_mtx);
		if (now - last > sim->params.t_burnout)
		{
			*out_id = sim->coders[i].id;
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		burned_id;

	sim = (t_sim *)arg;
	while (!sim_should_stop(sim))
	{
		if (all_done(sim))
		{
			sim_request_stop(sim);
			return (NULL);
		}
		if (check_burnout(sim, &burned_id))
		{
			/* IMPORTANT: this message must print even if we stop right after */
			pthread_mutex_lock(&sim->print_mtx);
			printf("%ld %d %s\n", sim_time_ms(sim), burned_id, "burned out");
			pthread_mutex_unlock(&sim->print_mtx);

			sim_request_stop(sim);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
