/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 20:47:02 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/02 17:06:18 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	now_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (0);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

long	sim_time_ms(t_sim *sim)
{
	return (now_ms() - sim->start_ms);
}

void	ms_sleep(long ms)
{
	long	start;
	long	now;

	if (ms <= 0)
		return ;
	start = now_ms();
	while (1)
	{
		now = now_ms();
		if (now - start >= ms)
			break ;
		if (ms - (now - start) > 1)
			usleep(500);
		else
			usleep(100);
	}
}
