/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 15:53:54 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/02 17:24:40 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	sim_init(t_sim *sim, const t_params *param)
{
	sim->params = *param;
	sim->start_ms = now_ms();
	sim->stop = 0;
	sim->coders = NULL;
	if (pthread_mutex_init(&sim->print_mtx, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->stop_mtx, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->print_mtx);
		return (0);
	}
	if (!sim_alloc(sim))
	{
		pthread_mutex_destroy(&sim->stop_mtx);
		pthread_mutex_destroy(&sim->print_mtx);
		return (0);
	}
	if (!sim_resources_init(sim))
	{
		sim_free(sim);
		pthread_mutex_destroy(&sim->stop_mtx);
		pthread_mutex_destroy(&sim->print_mtx);
		return (0);
	}
	return (1);
}

void	sim_destroy(t_sim *sim)
{
	sim_resources_destroy(sim);
	sim_free(sim);
	pthread_mutex_destroy(&sim->stop_mtx);
	pthread_mutex_destroy(&sim->print_mtx);
}

void	sim_request_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_mtx);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mtx);
}

int	sim_should_stop(t_sim *sim)
{
	int	v;

	pthread_mutex_lock(&sim->stop_mtx);
	v = sim->stop;
	pthread_mutex_unlock(&sim->stop_mtx);
	return (v);
}

int	sim_run(t_sim *sim)
{
	int	i;

	if (pthread_create(&sim->monitor_thread, NULL,
			&monitor_routine, sim) != 0)
		return (0);

	i = 0;
	while (i < sim->params.n_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				&coder_routine, &sim->coders[i]) != 0)
		{
			sim_request_stop(sim);
			break ;
		}
		i++;
	}
	while (i > 0)
	{
		i--;
		pthread_join(sim->coders[i].thread, NULL);
	}
	pthread_join(sim->monitor_thread, NULL);
	return (1);
}
