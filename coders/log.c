/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 20:33:33 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/01 15:30:05 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_message(t_simulation *sim, t_coder *coder, char *message)
{
	long	timestamp;

	pthread_mutex_lock(&sim->log_mutex);
	timestamp = get_elapsed_ms(sim);
	if (sim->simulation_running || strcmp(message, "burned out") == 0)
		printf("%ld %d %s\n", timestamp, coder->id, message);
	pthread_mutex_unlock(&sim->log_mutex);
}

void	log_taken_dongle(t_simulation *sim, t_coder *coder)
{
	log_message(sim, coder, "has taken a dongle");
}

void	log_compiling(t_simulation *sim, t_coder *coder)
{
	log_message(sim, coder, "is compiling");
}

void	log_debugging(t_simulation *sim, t_coder *coder)
{
	log_message(sim, coder, "is debugging");
}

void	log_refactoring(t_simulation *sim, t_coder *coder)
{
	log_message(sim, coder, "is refactoring");
}
