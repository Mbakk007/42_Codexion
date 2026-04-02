/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 20:33:33 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/02 17:07:28 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_event(t_sim *sim, int coder_id, const char *msg)
{
	long	ms;

	if (sim_should_stop(sim))
		return ;
	pthread_mutex_lock(&sim->print_mtx);
	if (!sim_should_stop(sim))
	{
		ms = sim_time_ms(sim);
		printf("%ld %d %s\n", ms, coder_id, msg);
	}
	pthread_mutex_unlock(&sim->print_mtx);
}