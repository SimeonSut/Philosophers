/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: simeon <simeon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 17:32:37 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/02 07:55:44 by simeon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*routine(void	*arg);

void		thread_setup(t_philo *node)
{
	t_list	*lst;

	lst = node->list;
	while (lst)
	{
		pthread_create(lst->thread, NULL, &routine, (void *)node);
		lst = lst->next;
	}
}

static void	*routine(void	*arg)
{
	int		phindex;

	phindex = 0;
	pthread_mutex_lock(((t_philo *)arg)->i_mtx);
	phindex = ((t_philo *)arg)->list->i;
	((t_philo *)arg)->list = ((t_philo *)arg)->list->next;
	pthread_mutex_unlock(((t_philo *)arg)->i_mtx);
	return (arg)
}
