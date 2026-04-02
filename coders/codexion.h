/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 17:40:36 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/02 15:40:01 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_params
{
	int n_coders;
	long t_burnout;
	long t_compile;
	long t_debug;
	long t_refactor;
	int must_compile_count;
	long dongle_cooldown;
} t_params;

// args.c
int	parse_n_coders(int *out, const char *s);
int	parse_pos_long(long *out, const char *s);
int	parse_args(t_params *p, int argc, char **argv);
int	parse_pos_int(int *out, const char *s);
int is_scheduler_ok(const char *s);

// utils.c
int	ft_strcmp(const char *s1, const char *s2);
int is_digits(const char *s);
int		ft_atoi(const char *s);
long	ft_atol(const char *s);

#endif