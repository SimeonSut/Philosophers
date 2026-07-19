/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 17:32:37 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/19 17:54:44 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*start(void	*arg);
static void	get_data(t_philo *node, int *phindex, int *states);
static void	closing_loop(t_list *lst);

void		thread_setup(t_philo *node)
{
	int		i;
	t_list	*lst;

	i = node->list->i;
	pthread_mutex_lock(&node->t_philo_mtx);
	while (node->list)
	{
		pthread_create(node->list->thread, NULL, &start, (void *)node);
		if (node->list->next)
			node->list = node->list->next;
		if (node->list->i == i)
			break;
	}
	lst = node->list;
	pthread_mutex_unlock(&node->t_philo_mtx);
	closing_loop(lst);
}

static void	*start(void	*arg)
{
	t_philo	*node;
	t_list	*lst;
	int		phindex;
	int		states[2];

	pthread_mutex_lock(&((t_philo *)arg)->t_philo_mtx);
	node = ((t_philo *)arg);
	lst = node->list;
	get_data(node, &phindex, states);
	pthread_mutex_unlock(&node->t_philo_mtx);
	open_close_gates(node, lst, phindex, LOCK);
	routine(node, lst, phindex, states);
	pthread_mutex_lock(&node->t_philo_mtx);
	if (node->death_check == DEAD)
		return (pthread_mutex_unlock(&node->t_philo_mtx), NULL);
	return (arg);
}

static void	closing_loop(t_list *lst)
{
	t_philo	*return_node;
	int		i;

	return_node = NULL;
	i = lst->i;
	pthread_join(*lst->thread, (void *)return_node);
	lst = lst->next;
	while (lst && i != lst->i && return_node)
	{
		pthread_join(*lst->thread, (void *)return_node);
		lst = lst->next;
	}
	while (lst && i != lst->i)
	{
		pthread_detach(*lst->thread);
		lst = lst->next;
	}
}

static void	get_data(t_philo *node, int *phindex, int *states)
{
	*phindex = node->list->i;
	states[EAT] = node->tt_eat;
	states[SLEEP] = node->tt_sleep;
	node->list = node->list->next;
}
