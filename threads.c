/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 17:32:37 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/22 21:44:05 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*start(void	*arg);
static void	closing_loop(t_list *lst);

void	thread_setup(t_philo *node)
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
			break ;
	}
	lst = node->list;
	pthread_mutex_unlock(&node->t_philo_mtx);
	closing_loop(lst);
}

static void	*start(void	*arg)
{
	t_philo	*node;
	t_list	*lst;
	int		states[2];

	pthread_mutex_lock(&((t_philo *)arg)->t_philo_mtx);
	node = ((t_philo *)arg);
	lst = node->list;
	states[EAT] = node->tt_eat;
	states[SLEEP] = node->tt_sleep;
	if (node->list->next)
		node->list = node->list->next;
	pthread_mutex_unlock(&node->t_philo_mtx);
	open_close_gates(node, lst, LOCK);
	routine(node, lst, states);
	if (lst->gate_count == 0)
		open_close_gates(node, lst, UNLOCK);
	pthread_mutex_lock(&node->t_philo_mtx);
	if (node->death_check == DEAD)
	{
		pthread_mutex_unlock(&node->t_philo_mtx);
		return (NULL);
	}
	pthread_mutex_unlock(&node->t_philo_mtx);
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
	while (lst && lst->i != i)
	{
		pthread_join(*lst->thread, (void *)return_node);
		lst = lst->next;
	}
}

/**
Odd philos enter :

1 locks 1 and 5 -> 1 eats
3 locks 3 and 4 -> 3 eats
5 locks 5 and blocks on 1 -> dont eat


Even philos enter :

2 locks 2 and blocks on 3 (held by 3) -> dont eat
4 locks blocks on 4 -> dont eat

when 1 goes to sleep, it unlocks 1 and 5, so 5 takes 1 -> 5 eats
when 3 goes to sleep, 2 locks 3 -> 2 eats
BUT 4 locks 4 and blocks on 5 (held by 5 now) -> dont eat

when 5 goes to sleep, 4 locks 5 -> 4 eats
when 2 goes to sleep, 1 locks 1 and 2 (after getting up) -> 2 eats

 */
