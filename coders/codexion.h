/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 17:40:36 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/02 17:53:08 by ael-bakk         ###   ########.fr       */
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

typedef struct s_sim t_sim;
typedef struct s_queue	t_queue;
typedef struct s_dongle	t_dongle;

typedef enum e_sched
{
	FIFO = 0,
	EDF = 1
}	t_sched;

typedef struct s_params
{
	int				n_coders;
	long			t_burnout;
	long			t_compile;
	long			t_debug;
	long			t_refactor;
	int				must_compile_count;
	long			dongle_cooldown;
	t_sched			scheduler;
}					t_params;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_sim			*sim;
	long 			last_compile_start_ms;
	int				compile_count;
	pthread_mutex_t state_mtx;
}					t_coder;

typedef struct s_sim
{
	t_params		params;
	pthread_mutex_t	print_mtx;
	int				stop;
	pthread_mutex_t	stop_mtx;
	long			start_ms;
	pthread_t		monitor_thread;
	t_coder			*coders;
	t_queue 		*queue;
	t_dongle		*dongle;
}					t_sim;

// args.c
int					parse_n_coders(int *out, const char *s);
int					parse_pos_long(long *out, const char *s);
int					parse_args(t_params *p, int argc, char **argv);
int					parse_pos_int(int *out, const char *s);
int					is_scheduler_ok(const char *s);

// utils.c
int					ft_strcmp(const char *s1, const char *s2);
int					is_digits(const char *s);
int					ft_atoi(const char *s);
long				ft_atol(const char *s);

// sim.c
int					sim_init(t_sim *sim, const t_params *param);
void				sim_destroy(t_sim *sim);
void				sim_request_stop(t_sim *sim);
int					sim_should_stop(t_sim *sim);
int					sim_run(t_sim *sim);

// time.c
long				now_ms(void);
long				sim_time_ms(t_sim *sim);
void				ms_sleep(long ms);

// log.c
void				log_event(t_sim *sim, int coder_id, const char *msg);

// coder.c
void				*coder_routine(void *arg);

// monitor.c
void				*monitor_routine(void *arg);

// sim_alloc.c
int					sim_alloc(t_sim *sim);
void				sim_free(t_sim *sim);

// init.c
int					sim_resources_init(t_sim *sim);
void				sim_resources_destroy(t_sim *sim);

// queue.c
int		queue_init(t_sim *sim);
void	queue_destroy(t_sim *sim);

// dongle.c
int		dongle_init(t_sim *sim);
void	dongle_destroy(t_sim *sim);

// scheduler.c
int		scheduler_init(t_sim *sim);
void	scheduler_destroy(t_sim *sim);
#endif