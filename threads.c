/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 17:32:37 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/14 17:12:39 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*start(void	*arg);

void		thread_setup(t_philo *node)
{
	int		i;
	t_list	*lst;

	i = node->list->i;
	pthread_mutex_lock(&node->t_philo_mtx);
	while (1)
	{
		pthread_create(node->list->thread, NULL, &start, (void *)node);
		node->list = node->list->next;
		if (node->list->i == i)	
			break;
	}
	pthread_mutex_unlock(&node->t_philo_mtx);
	i = node->list->i;
	lst = node->list;
	while (1)
	{
		pthread_join(*lst->thread, NULL);
		lst = lst->next;
		if (lst->i == i)
			break;
	}
}

static void	*start(void	*arg)
{
	t_philo	*node;
	t_list	*lst;
	int		phindex;
	int		states[3];

	pthread_mutex_lock(&((t_philo *)arg)->t_philo_mtx);
	node = ((t_philo *)arg);
	lst = node->list;
	phindex = node->list->i;
	states[EAT] = node->tt_eat;
	states[SLEEP] = node->tt_sleep;
	states[THINK] = -1;
	node->list = node->list->next;
	if (phindex % 2 == 0)
		even_philos(node, lst, phindex, states);
	else
		odd_philos(node, lst, phindex, states);
	return (arg);
}
