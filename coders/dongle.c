/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bakk <ael-bakk@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:10:15 by ael-bakk          #+#    #+#             */
/*   Updated: 2026/04/02 10:50:52 by ael-bakk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Helper functions for priority queue management
static void	enqueue_coder(t_dongle *dongle, int coder_id, long priority)
{
	t_priority_queue	*q;
	t_priority_coder	tmp;
	int					i;
	int					parent;

	q = &dongle->wait_queue;
	if (q->size >= q->capacity)
		return ;
	q->coders[q->size++] = (t_priority_coder){coder_id, priority};
	i = q->size - 1;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (q->coders[i].priority >= q->coders[parent].priority)
			break ;
		tmp = q->coders[i];
		q->coders[i] = q->coders[parent];
		q->coders[parent] = tmp;
		i = parent;
	}
}

static int	dequeue_coder(t_dongle *dongle)
{
	t_priority_queue	*q;
	t_priority_coder	tmp;
	int					result;
	int					i;
	int					smallest;

	q = &dongle->wait_queue;
	if (q->size == 0)
		return (-1);
	result = q->coders[0].coder_id;
	q->coders[0] = q->coders[--q->size];
	i = 0;
	while (2 * i + 1 < q->size)
	{
		smallest = 2 * i + 1;
		if (2 * i + 2 < q->size
			&& q->coders[2 * i + 2].priority < q->coders[smallest].priority)
			smallest = 2 * i + 2;
		if (q->coders[i].priority <= q->coders[smallest].priority)
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

void	release_dongles(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;
	long		now;

	left = coder->left_dongle;
	right = coder->right_dongle;
	now = get_elapsed_ms(coder->sim);
	pthread_mutex_lock(&left->mutex);
	left->is_held = 0;
	left->release_time = now;
	pthread_cond_broadcast(&left->cond);
	pthread_mutex_unlock(&left->mutex);
	pthread_mutex_lock(&right->mutex);
	right->is_held = 0;
	right->release_time = now;
	pthread_cond_broadcast(&right->cond);
	pthread_mutex_unlock(&right->mutex);
}

void	wait_for_dongles(t_coder *c)
{
	t_dongle	*l;
	t_dongle	*r;
	long		p;
	long		now;

	if (c->left_dongle->id < c->right_dongle->id)
	{
		l = c->left_dongle;
		r = c->right_dongle;
	}
	else
	{
		l = c->right_dongle;
		r = c->left_dongle;
	}
	if (c->sim->params.sched == EDF_SCHED)
		p = c->deadline;
	else
		p = get_timestamp_ms();
	pthread_mutex_lock(&l->mutex);
	now = get_elapsed_ms(c->sim);
	while (l->is_held || now - l->release_time
		< c->sim->params.dongle_cooldown_ms)
	{
		if (!is_coder_in_queue(l, c->id))
			enqueue_coder(l, c->id, p);
		pthread_cond_wait(&l->cond, &l->mutex);
		now = get_elapsed_ms(c->sim);
	}
	if (dequeue_coder(l) != c->id && l->wait_queue.size > 0)
	{
		enqueue_coder(l, c->id, p);
		pthread_cond_wait(&l->cond, &l->mutex);
	}
	l->is_held = 1;
	pthread_mutex_unlock(&l->mutex);
	log_taken_dongle(c->sim, c);
	pthread_mutex_lock(&r->mutex);
	now = get_elapsed_ms(c->sim);
	while (r->is_held || now - r->release_time
		< c->sim->params.dongle_cooldown_ms)
	{
		if (!is_coder_in_queue(r, c->id))
			enqueue_coder(r, c->id, p);
		pthread_cond_wait(&r->cond, &r->mutex);
		now = get_elapsed_ms(c->sim);
	}
	if (dequeue_coder(r) != c->id && r->wait_queue.size > 0)
	{
		enqueue_coder(r, c->id, p);
		pthread_cond_wait(&r->cond, &r->mutex);
	}
	r->is_held = 1;
	pthread_mutex_unlock(&r->mutex);
	log_taken_dongle(c->sim, c);
	c->last_compile_start = get_elapsed_ms(c->sim);
}
