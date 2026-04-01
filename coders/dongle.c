/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:10:15 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/01 16:47:19 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Helper functions for priority queue management
static void	enqueue_coder(t_dongle *dongle, int coder_id, long priority)
{
	t_priority_queue	*q;
	int					i;
	t_priority_coder	tmp;

	q = &dongle->wait_queue;
	if (q->size >= q->capacity)
		return ;
	q->coders[q->size].coder_id = coder_id;
	q->coders[q->size].priority = priority;
	q->size++;
	i = q->size - 1;
	while (i > 0 && q->coders[i].priority < q->coders[(i - 1) / 2].priority)
	{
		tmp = q->coders[i];
		q->coders[i] = q->coders[(i - 1) / 2];
		q->coders[(i - 1) / 2] = tmp;
		i = (i - 1) / 2;
	}
}

static int	dequeue_coder(t_dongle *dongle)
{
	t_priority_queue	*q;
	int					result;
	int					i;
	int					smallest;
	t_priority_coder	tmp;

	q = &dongle->wait_queue;
	if (q->size == 0)
		return (-1);
	result = q->coders[0].coder_id;
	q->coders[0] = q->coders[q->size - 1];
	q->size--;
	i = 0;
	while (2 * i + 1 < q->size)
	{
		smallest = i;
		if (q->coders[2 * i + 1].priority < q->coders[smallest].priority)
			smallest = 2 * i + 1;
		if (2 * i + 2 < q->size && q->coders[2 * i
				+ 2].priority < q->coders[smallest].priority)
			smallest = 2 * i + 2;
		if (smallest == i)
			break ;
		tmp = q->coders[i];
		q->coders[i] = q->coders[smallest];
		q->coders[smallest] = tmp;
		i = smallest;
	}
	return (result);
}

static int	is_coder_in_queue(t_dongle *dongle, int coder_id)
{
	int	i;

	i = 0;
	while (i < dongle->wait_queue.size)
	{
		if (dongle->wait_queue.coders[i].coder_id == coder_id)
			return (1);
		i++;
	}
	return (0);
}

// Releases dongles when called, signals waiting coders
void	release_dongles(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;
	long		current_time;

	left = coder->left_dongle;
	right = coder->right_dongle;
	current_time = get_elapsed_ms(coder->sim);
	pthread_mutex_lock(&left->mutex);
	left->is_held = 0;
	left->release_time = current_time;
	pthread_cond_broadcast(&left->cond);
	pthread_mutex_unlock(&left->mutex);
	pthread_mutex_lock(&right->mutex);
	right->is_held = 0;
	right->release_time = current_time;
	pthread_cond_broadcast(&right->cond);
	pthread_mutex_unlock(&right->mutex);
}

// Waits until both dongles are available, then locks them
void	wait_for_dongles(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;
	long		priority;
	long		current_time;

	if (coder->left_dongle->id < coder->right_dongle->id)
	{
		left = coder->left_dongle;
		right = coder->right_dongle;
	}
	else
	{
		left = coder->right_dongle;
		right = coder->left_dongle;
	}
	if (coder->sim->params.sched == EDF_SCHED)
		priority = coder->deadline;
	else
		priority = get_timestamp_ms();
	pthread_mutex_lock(&left->mutex);
	current_time = get_elapsed_ms(coder->sim);
	while (left->is_held || (current_time
			- left->release_time < coder->sim->params.dongle_cooldown_ms))
	{
		if (!is_coder_in_queue(left, coder->id))
			enqueue_coder(left, coder->id, priority);
		pthread_cond_wait(&left->cond, &left->mutex);
		current_time = get_elapsed_ms(coder->sim);
	}
	if (dequeue_coder(left) != coder->id && left->wait_queue.size > 0)
	{
		enqueue_coder(left, coder->id, priority);
		pthread_cond_wait(&left->cond, &left->mutex);
		current_time = get_elapsed_ms(coder->sim);
	}
	left->is_held = 1;
	pthread_mutex_unlock(&left->mutex);
	log_taken_dongle(coder->sim, coder);
	pthread_mutex_lock(&right->mutex);
	current_time = get_elapsed_ms(coder->sim);
	while (right->is_held || (current_time
			- right->release_time < coder->sim->params.dongle_cooldown_ms))
	{
		if (!is_coder_in_queue(right, coder->id))
			enqueue_coder(right, coder->id, priority);
		pthread_cond_wait(&right->cond, &right->mutex);
		current_time = get_elapsed_ms(coder->sim);
	}
	if (dequeue_coder(right) != coder->id && right->wait_queue.size > 0)
	{
		enqueue_coder(right, coder->id, priority);
		pthread_cond_wait(&right->cond, &right->mutex);
		current_time = get_elapsed_ms(coder->sim);
	}
	right->is_held = 1;
	pthread_mutex_unlock(&right->mutex);
	log_taken_dongle(coder->sim, coder);
	coder->last_compile_start = get_elapsed_ms(coder->sim);
}
