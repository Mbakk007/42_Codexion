/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 10:53:32 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/05 00:08:34 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	reached_compile_count(t_coder *c)
{
	int	done;

	if (c->sim->params.must_compile_count <= 0)
		return (0);
	pthread_mutex_lock(&c->state_mtx);
	done = (c->compile_count >= c->sim->params.must_compile_count);
	pthread_mutex_unlock(&c->state_mtx);
	return (done);
}

static int	coder_cycle(t_coder *c)
{
	if (!dongles_take_two(c))
		return (0);
	if (sim_should_stop(c->sim))
		return (dongles_release_two(c), 0);
	pthread_mutex_lock(&c->state_mtx);
	c->last_compile_start_ms = sim_time_ms(c->sim);
	pthread_mutex_unlock(&c->state_mtx);
	log_event(c->sim, c->id, "is compiling");
	ms_sleep(c->sim, c->sim->params.t_compile);
	dongles_release_two(c);
	if (sim_should_stop(c->sim))
		return (0);
	log_event(c->sim, c->id, "is debugging");
	ms_sleep(c->sim, c->sim->params.t_debug);
	if (sim_should_stop(c->sim))
		return (0);
	log_event(c->sim, c->id, "is refactoring");
	pthread_mutex_lock(&c->state_mtx);
	c->compile_count++;
	pthread_mutex_unlock(&c->state_mtx);
	ms_sleep(c->sim, c->sim->params.t_refactor);
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder	*c;

	c = (t_coder *)arg;
	while (!sim_should_stop(c->sim))
	{
		if (!coder_cycle(c))
			break ;
		if (reached_compile_count(c))
			break ;
	}
	return (NULL);
}
