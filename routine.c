/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 17:32:37 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/02 19:44:35 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*start(void	*arg);
static void	uneven_group(t_philo *node, t_list *lst, int phindex);
static void even_group(t_philo *node, t_list *lst, int phindex);

void		thread_setup(t_philo *node)
{
	t_list	*lst;

	lst = node->list;
	while (lst)
	{
		pthread_create(lst->thread, NULL, &start, (void *)node);
		lst = lst->next;
	}
}

static void	*start(void	*arg)
{
	t_philo	*node;
	t_list	*lst;
	int		phindex;

	phindex = 0;
	pthread_mutex_lock(((t_philo *)arg)->i_mtx);
	node = ((t_philo *)arg);
	lst = ((t_philo *)arg)->list;
	phindex = ((t_philo *)arg)->list->i;
	((t_philo *)arg)->list = ((t_philo *)arg)->list->next;
	pthread_mutex_unlock(((t_philo *)arg)->i_mtx);
	if (phindex % 2 != 0)
		uneven_group(node, lst, phindex);
	else
		even_group(node, lst, phindex);
	return (arg);
}

static void	uneven_group(t_philo *node, t_list *lst, int phindex)
{
	if (node->n_of_philos % 2 != 0)
		/*uneven_uneven_cycle_*/;
	else
		/*even_uneven_cycle*/;
}

static void even_group(t_philo *node, t_list *lst, int phindex)
{
	if (node->n_of_philos % 2 != 0)
		/*uneven_even_cycle_*/;
	else
		/*even_even_cycle*/;
}
