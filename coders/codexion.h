/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 17:40:36 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/05 00:25:43 by ael-bakk         ###   ########.fr       */
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

typedef struct s_sim	t_sim;
typedef struct s_dongle	t_dongle;

typedef struct s_request
{
	int					coder_id;
	long				arrival_key;
}						t_request;

typedef struct s_queue
{
	t_request			*requests;
	int					len;
	int					cap;
}						t_queue;

typedef enum e_sched
{
	FIFO = 0,
	EDF = 1
}						t_sched;

typedef struct s_params
{
	int					n_coders;
	long				t_burnout;
	long				t_compile;
	long				t_debug;
	long				t_refactor;
	int					must_compile_count;
	long				dongle_cooldown;
	t_sched				scheduler;
}						t_params;

typedef struct s_coder
{
	int					id;
	pthread_t			thread;
	t_sim				*sim;
	long				last_compile_start_ms;
	int					compile_count;
	pthread_mutex_t		state_mtx;
}						t_coder;

typedef struct s_sim
{
	t_params			params;
	pthread_mutex_t		print_mtx;
	int					stop;
	pthread_mutex_t		stop_mtx;
	long				start_ms;
	pthread_t			monitor_thread;
	t_coder				*coders;
	t_queue				*queue;
	t_dongle			*dongles;
	pthread_mutex_t		sched_mtx;
	pthread_cond_t		sched_cond;
	long				next_arrival_key;
}						t_sim;

typedef struct s_dongle
{
	pthread_mutex_t		mtx;
	long				available_at_ms;
}						t_dongle;

// args.c
int						parse_args(t_params *p, int argc, char **argv);

// utils.c
int						is_digits(const char *s);
long					ft_atol(const char *s);

// sim.c
int						sim_init(t_sim *sim, const t_params *param);
void					sim_destroy(t_sim *sim);
void					sim_request_stop(t_sim *sim);
int						sim_should_stop(t_sim *sim);
int						sim_run(t_sim *sim);

// time.c
long					now_ms(void);
long					sim_time_ms(t_sim *sim);
void					ms_sleep(t_sim *sim, long ms);

// log.c
void					log_event(t_sim *sim, int coder_id, const char *msg);

// coder.c
void					*coder_routine(void *arg);

// monitor.c
void					*monitor_routine(void *arg);

// sim_alloc.c
int						sim_alloc(t_sim *sim);
void					sim_free(t_sim *sim);

// init.c
int						sim_resources_init(t_sim *sim);
void					sim_resources_destroy(t_sim *sim);
int						dongles_init(t_sim *sim);
int						scheduler_init(t_sim *sim);
int						queue_init(t_sim *sim);

// queue.c
void					queue_destroy(t_sim *sim);
int						queue_push(t_sim *sim, int coder_id);
void					queue_remove(t_sim *sim, int coder_id);
int						queue_pick_winner(t_sim *sim);

// dongle.c
void					dongles_destroy(t_sim *sim);
int						dongles_take_two(t_coder *c);
void					dongles_release_two(t_coder *c);

// scheduler.c
void					scheduler_destroy(t_sim *sim);
long					scheduler_key_ms(t_sim *sim, int coder_id,
							long arrival_ms);
#endif