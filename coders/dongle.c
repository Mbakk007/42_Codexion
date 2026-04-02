/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:10:15 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/02 17:53:39 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

struct s_dongle { int unused; };

int	dongle_init(t_sim *sim)
{
	(void)sim;
	return (1);
}

void	dongle_destroy(t_sim *sim)
{
	(void)sim;
}
