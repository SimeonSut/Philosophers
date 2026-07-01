/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 17:32:37 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/01 21:56:06 by ssutarmi         ###   ########.fr       */
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
	t_list	*lst;

	phindex = 0;
	lst = ((t_philo *)arg)->list;
	pthread_mutex_lock(lst->mutex);
	return (arg);
}

static void	find_index(t_list *lst, int *phindex)
{
	if (*phindex == 0 && lst->i_taken == NOT_TAKEN)
	{
		*phindex = lst->i;
		lst->i_taken = TAKEN;
		return ;
	}
}
