/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 17:32:37 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/05 00:07:57 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*start(void	*arg);
static void	odd_philos(t_philo *node, t_list *lst, int phindex, int *tms);
static void even_philos(t_philo *node, t_list *lst, int phindex, int *tms);

void		thread_setup(t_philo *node)
{
	int		i;
	t_list	*lst;

	i = 0;
	lst = node->list;
	while (node->gates_mtx[i])
		pthread_mutex_init(node->gates_mtx[i++], NULL);
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
	int		states[3];

	phindex = 0;
	pthread_mutex_lock(((t_philo *)arg)->i_mtx);
	node = ((t_philo *)arg);
	lst = node->list;
	phindex = node->list->i;
	fill_states_times(node, states);
	if (phindex % 2 == 0)
		pthread_mutex_lock(node->gates_mtx[((phindex - 2) / 2)]);
	node->list = node->list->next;
	pthread_mutex_unlock(node->i_mtx);
	if (phindex % 2 == 0)
	{
		even_philos(node, lst, phindex, states);
	}
	else
	{
		pthread_mutex_lock(node->gates_mtx[((phindex - 1) / 2)]);
		odd_philos(node, lst, phindex, states);
	}
	return (arg);
}

static void even_philos(t_philo *node, t_list *lst, int phindex, int *tms)
{
	while (1)
	{
		pthread_mutex_lock(lst->fork_mtx);
		pthread_mutex_lock(lst->next->fork_mtx);
		state(node, "eating", tms[EAT], tms[DIE]);
		pthread_mutex_unlock(lst->fork_mtx);
		pthread_mutex_unlock(lst->next->fork_mtx);
		/*sleeping*/;
		/*thinking*/;
	}
}

static void	odd_philos(t_philo *node, t_list *lst, int phindex, int *tms)
{
	while (1)
	{
		pthread_mutex_lock(lst->fork_mtx);
		pthread_mutex_lock(lst->next->fork_mtx);
		/*eating*/;
		pthread_mutex_unlock(lst->fork_mtx);
		pthread_mutex_unlock(lst->next->fork_mtx);
		/*sleeping*/;
		/*thinking*/;
	}
}
