/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 20:47:02 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/01 15:30:31 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_timestamp_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	get_elapsed_ms(t_simulation *sim)
{
	if (!sim)
		return (0);
	return (get_timestamp_ms() - sim->start_time);
}

void	sleep_ms(long ms)
{
	long	start;
	long	target;
	long	remaining;

	start = get_timestamp_ms();
	target = start + ms;
	while (get_timestamp_ms() < target)
	{
		remaining = target - get_timestamp_ms();
		if (remaining > 1)
			usleep(remaining * 1000);
		else
			usleep(100);
	}
}
