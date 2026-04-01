/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 10:53:32 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/01 16:27:55 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	start_compiling(t_coder *coder);
static void	start_debugging(t_coder *coder);
static void	start_refactoring(t_coder *coder);

void	*coder_thread(void *arg)
{
	t_coder			*coder;
	t_simulation	*sim;

	coder = (t_coder *)arg;
	sim = coder->left_dongle->sim;
	while (sim->simulation_running)
	{
		pthread_mutex_lock(&sim->state_mutex);
		if (sim->someone_burned_out || sim->all_done)
		{
			pthread_mutex_unlock(&sim->state_mutex);
			break ;
		}
		pthread_mutex_unlock(&sim->state_mutex);
		wait_for_dongles(coder);
		start_compiling(coder);
		pthread_mutex_lock(&sim->state_mutex);
		if (sim->someone_burned_out || sim->all_done)
		{
			pthread_mutex_unlock(&sim->state_mutex);
			break ;
		}
		pthread_mutex_unlock(&sim->state_mutex);
		start_debugging(coder);
		pthread_mutex_lock(&sim->state_mutex);
		if (sim->someone_burned_out || sim->all_done)
		{
			pthread_mutex_unlock(&sim->state_mutex);
			break ;
		}
		pthread_mutex_unlock(&sim->state_mutex);
		start_refactoring(coder);
	}
	return (NULL);
}

// Simulate compiling
static void	start_compiling(t_coder *coder)
{
	t_simulation	*sim;
	int				i;

	sim = coder->left_dongle->sim;
	pthread_mutex_lock(&sim->state_mutex);
	if (!sim->has_started)
	{
		sim->has_started = 1;
		i = 0;
		while (i < sim->params.number_of_coders)
		{
			sim->coders[i].deadline = get_elapsed_ms(sim)
				+ sim->params.time_to_burnout_ms;
			i++;
		}
	}
	pthread_mutex_unlock(&sim->state_mutex);
	coder->state = COMPILING;
	log_message(sim, coder, "is compiling");
	sleep_ms(sim->params.time_to_compile_ms);
	coder->compiles_completed++;
	release_dongles(coder);
	coder->last_compile_start = get_elapsed_ms(sim);
}

// Simulate debugging
static void	start_debugging(t_coder *coder)
{
	t_simulation	*sim;

	sim = coder->left_dongle->sim;
	pthread_mutex_lock(&sim->state_mutex);
	if (sim->someone_burned_out)
	{
		pthread_mutex_unlock(&sim->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&sim->state_mutex);
	coder->state = DEBUGGING;
	log_message(sim, coder, "is debugging");
	sleep_ms(sim->params.time_to_debug_ms);
}

// Simulate refactoring
static void	start_refactoring(t_coder *coder)
{
	t_simulation	*sim;

	sim = coder->left_dongle->sim;
	pthread_mutex_lock(&sim->state_mutex);
	if (sim->someone_burned_out)
	{
		pthread_mutex_unlock(&sim->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&sim->state_mutex);
	coder->state = REFACTORING;
	log_message(sim, coder, "is refactoring");
	sleep_ms(sim->params.time_to_refactor_ms);
	pthread_mutex_lock(&sim->state_mutex);
	if (coder->compiles_completed >= sim->params.number_of_compiles_required)
		sim->all_done = 1;
	pthread_mutex_unlock(&sim->state_mutex);
}
