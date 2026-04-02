/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_alloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 16:14:15 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/02 16:20:40 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	sim_alloc(t_sim *sim)
{
	int	i;

	sim->coders = malloc(sizeof(t_coder) * sim->params.n_coders);
	if (!sim->coders)
		return (0);
	i = 0;
	while (i < sim->params.n_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].sim = sim;
		i++;
	}
	return (1);
}

void	sim_free(t_sim *sim)
{
	free(sim->coders);
	sim->coders = NULL;
}