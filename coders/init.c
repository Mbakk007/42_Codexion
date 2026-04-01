/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:49:49 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/01 16:29:00 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	number_of_dongles(t_simulation *sim);
static int	init_dongles(t_simulation *sim);
static int	init_coders(t_simulation *sim);
static int	create_threads(t_simulation *sim);

void	destroy_simulation(t_simulation *sim)
{
	int	i;

	i = 0;
	if (sim->coders)
	{
		if (sim->simulation_running)
		{
			while (i < sim->params.number_of_coders)
			{
				pthread_join(sim->coders[i].thread, NULL);
				i++;
			}
			pthread_join(sim->monitor_thread, NULL);
		}
		pthread_mutex_destroy(&sim->log_mutex);
		pthread_mutex_destroy(&sim->state_mutex);
		free(sim->coders);
		sim->coders = NULL;
	}
	if (sim->dongles)
	{
		i = 0;
		while (i < number_of_dongles(sim))
		{
			pthread_mutex_destroy(&sim->dongles[i].mutex);
			pthread_cond_destroy(&sim->dongles[i].cond);
			free(sim->dongles[i].wait_queue.coders);
			i++;
		}
		free(sim->dongles);
		sim->dongles = NULL;
	}
}

int	init_simulation(t_simulation *sim, int argc, char **argv)
{
	int	i;

	sim->coders = NULL;
	sim->dongles = NULL;
	sim->simulation_running = 0;
	sim->someone_burned_out = 0;
	sim->all_done = 0;
	sim->has_started = 0;
	if (!parse_args(&sim->params, argc, argv))
		return (-1);
	pthread_mutex_init(&sim->log_mutex, NULL);
	pthread_mutex_init(&sim->state_mutex, NULL);
	if (init_dongles(sim) < 0 || init_coders(sim) < 0)
	{
		destroy_simulation(sim);
		return (-1);
	}
	sim->start_time = get_timestamp_ms();
	if (create_threads(sim) < 0)
	{
		destroy_simulation(sim);
		return (-1);
	}
	return (0);
}

static int	number_of_dongles(t_simulation *sim)
{
	if (sim->params.number_of_coders > 1)
		return (sim->params.number_of_coders);
	else
		return (2);
}

static int	init_dongles(t_simulation *sim)
{
	int	i;
	int	num_dongles;

	num_dongles = number_of_dongles(sim);
	sim->dongles = malloc(sizeof(t_dongle) * num_dongles);
	if (!sim->dongles)
		return (-1);
	i = 0;
	while (i < num_dongles)
	{
		sim->dongles[i].id = i + 1;
		sim->dongles[i].is_held = 0;
		sim->dongles[i].release_time = 0;
		sim->dongles[i].sim = sim;
		pthread_mutex_init(&sim->dongles[i].mutex, NULL);
		pthread_cond_init(&sim->dongles[i].cond, NULL);
		sim->dongles[i].wait_queue.coders = malloc(sizeof(t_priority_coder)
				* num_dongles);
		if (!sim->dongles[i].wait_queue.coders)
			return (-1);
		sim->dongles[i].wait_queue.size = 0;
		sim->dongles[i].wait_queue.capacity = num_dongles;
		i++;
	}
	return (0);
}

static int	init_coders(t_simulation *sim)
{
	int		i;
	t_coder	*coder;

	i = 0;
	sim->coders = malloc(sizeof(t_coder) * sim->params.number_of_coders);
	if (!sim->coders)
		return (-1);
	while (i < sim->params.number_of_coders)
	{
		coder = &sim->coders[i];
		coder->id = i + 1;
		coder->state = WAITING;
		coder->compiles_completed = 0;
		coder->last_compile_start = 0;
		coder->deadline = 0;
		coder->left_dongle = &sim->dongles[i % number_of_dongles(sim)];
		coder->right_dongle = &sim->dongles[(i + 1) % number_of_dongles(sim)];
		coder->sim = sim;
		i++;
	}
	return (0);
}

static int	create_threads(t_simulation *sim)
{
	int	i;

	sim->simulation_running = 1;
	i = 0;
	while (i < sim->params.number_of_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL, coder_thread,
				&sim->coders[i]) != 0)
			return (-1);
		i++;
	}
	if (pthread_create(&sim->monitor_thread, NULL,
			(void *(*)(void *))monitor_simulation, sim) != 0)
		return (-1);
	return (0);
}
