/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 17:40:36 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/03/31 15:30:00 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <unistd.h>

typedef struct s_simulation	t_simulation;
// FIFO: first arrived, first served
// EDF: serve the coder closest to burnout_ms
typedef enum e_sched
{
	FIFO_SCHED = 0,
	EDF_SCHED = 1
}							t_sched;

// args struct
typedef struct args
{
	int						number_of_coders;
	long					time_to_burnout_ms;
	long					time_to_compile_ms;
	long					time_to_debug_ms;
	long					time_to_refactor_ms;
	int						number_of_compiles_required;
	long					dongle_cooldown_ms;
	t_sched					sched;
}							t_args;

// Represents one coder waiting in the queue
typedef struct s_priority_coder
{
	int						coder_id;
	long					priority;
}							t_priority_coder;

// A queue of coders waiting for the dongle
typedef struct s_priority_queue
{
	t_priority_coder		*coders;
	int						size;
	int						capacity;
}							t_priority_queue;

// Represents a single USB dongle
typedef struct s_dongle
{
	int						id;
	int						is_held;
	long					release_time;
	t_simulation			*sim;
	pthread_mutex_t			mutex;
	pthread_cond_t			cond;
	t_priority_coder		wait_queue;
}							t_dongle;

// Enum for coder state
typedef enum e_coder_state
{
	WAITING,
	COMPILING,
	DEBUGGING,
	REFACTORING,
	BURNED_OUT
}							t_coder_state;

// Represents a single coder
typedef struct s_coder
{
	int						id;
	t_coder_state			state;
	int						compiles_completed;
	long					last_compile_start;
	long					deadline;
	t_simulation			*sim;
	pthread_t				thread;
	t_dongle				*left_dongle;
	t_dongle				*right_dongle;
}							t_coder;

// Represents the entire simulation
typedef struct s_simulation
{
	t_args					params;
	t_coder					*coders;
	t_dongle				*dongles;
	pthread_t				monitor_thread;
	pthread_mutex_t			log_mutex;
	pthread_mutex_t			state_mutex;
	int						simulation_running;
	int						someone_burned_out;
	int						all_done;
	long					start_time;
}							t_simulation;

// args.c
int							parse_args(t_args *out, int argc, char **argv);

// time.c
long						get_timestamp_ms(void);
long						get_elapsed_ms(t_simulation *sim);
void						sleep_ms(long ms);

// log.c
void						log_message(t_simulation *sim, t_coder *coder,
								const char *message);
void						log_taken_dongle(t_simulation *sim, t_coder *coder);
void						log_compiling(t_simulation *sim, t_coder *coder);
void						log_debugging(t_simulation *sim, t_coder *coder);
void						log_refactoring(t_simulation *sim, t_coder *coder);

// init.c
void						destroy_simulation(t_simulation *sim);
int							init_simulation(t_simulation *sim, int argc,
								char **argv);

// coder.c
void						*coder_thread(void *arg);

#endif