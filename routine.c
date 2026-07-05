/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 17:32:37 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/05 20:01:08 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*start(void	*arg);
static void	odd_philos(t_philo *node, t_list *lst, int phindex, int *states);
static void even_philos(t_philo *node, t_list *lst, int phindex, int *states);

void		thread_setup(t_philo *node)
{
	int		i;
	t_list	*lst;

	i = 0;
	lst = node->list;
	while (1)
	{
		pthread_create(&lst->thread, NULL, &start, (void *)node);
		lst = lst->next;
		if (lst->i == node->n_of_philos)
			break;
	}
	while (1)
	{
		pthread_join(lst->thread, NULL);
		lst = lst->next;
		if (lst->i == node->n_of_philos)
			break;;
	}
}

static void	*start(void	*arg)
{
	t_philo	*node;
	t_list	*lst;
	int		phindex;
	int		states[3];

	write(1, "1\n", 2);
	phindex = 0;
	pthread_mutex_lock(&((t_philo *)arg)->t_philo_mtx);
	node = ((t_philo *)arg);
	lst = node->list;
	phindex = node->list->i;
	fill_states_times(node, states);
	write(1, "2\n", 2);
	if (phindex % 2 == 0)
		pthread_mutex_lock(&node->gates_mtx[((phindex - 2) / 2)]);
	node->list = node->list->next;
	pthread_mutex_unlock(&node->t_philo_mtx);
	write(1, "3\n", 2);
	if (phindex % 2 == 0)
	{
		even_philos(node, lst, phindex, states);
	}
	else
	{
		pthread_mutex_lock(&node->gates_mtx[((phindex - 1) / 2)]);
		odd_philos(node, lst, phindex, states);
	}
	write(1, "4\n", 2);
	return (arg);
}

static void even_philos(t_philo *node, t_list *lst, int phindex, int *states)
{
	struct timeval	tm;
	long long		microstart;
	int				timestmp;

	pthread_mutex_lock(&node->t_philo_mtx);
	microstart = node->microstart;
	pthread_mutex_unlock(&node->t_philo_mtx);
	while (1)
	{
		pthread_mutex_lock(&lst->fork_mtx);
		if (gettimeofday(&tm, NULL) == -1)
			return ;//gettimeofday error
		timestmp = (((tm.tv_sec * 1000000LL + tm.tv_usec) - microstart) / 1000);
		//if (send_msg(timestmp, phindex, " has taken a fork\n") == ERROR)
		//	return ;
		printf("%d %d has taken a fork\n", timestmp, phindex);
		pthread_mutex_lock(&lst->next->fork_mtx);
		if (gettimeofday(&tm, NULL) == -1)
			return ;//gettimeofday error
		timestmp = (((tm.tv_sec * 1000000LL + tm.tv_usec) - microstart) / 1000);
		//if (send_msg(timestmp, phindex, " has taken a fork\n") == ERROR)
		//	return;
		printf("%d %d has taken a fork\n", timestmp, phindex);
		state(node, states[EAT], phindex);
		pthread_mutex_unlock(&lst->fork_mtx);
		pthread_mutex_unlock(&lst->next->fork_mtx);
		state(node, states[SLEEP], phindex);
		state(node, states[SLEEP], phindex);
	}
}

static void	odd_philos(t_philo *node, t_list *lst, int phindex, int *states)
{
	struct timeval	tm;
	long long		microstart;
	int				timestmp;

	pthread_mutex_lock(&node->t_philo_mtx);
	microstart = node->microstart;
	pthread_mutex_unlock(&node->t_philo_mtx);
	while (1)
	{
		pthread_mutex_lock(&lst->fork_mtx);
		if (gettimeofday(&tm, NULL) == -1)
			return ;//gettimeofday error
		timestmp = (((tm.tv_sec * 1000000LL + tm.tv_usec) - microstart) / 1000);
		//if (send_msg(timestmp, phindex, " has taken a fork\n") == ERROR)
		//	return ;
		printf("%d %d has taken a fork\n", timestmp, phindex);
		pthread_mutex_lock(&lst->next->fork_mtx);
		if (gettimeofday(&tm, NULL) == -1)
			return ;//gettimeofday error
		timestmp = (((tm.tv_sec * 1000000LL + tm.tv_usec) - microstart) / 1000);
		//if (send_msg(timestmp, phindex, " has taken a fork\n") == ERROR)
		//	return;
		printf("%d %d has taken a fork\n", timestmp, phindex);
		state(node, states[EAT], phindex);
		pthread_mutex_unlock(&lst->fork_mtx);
		pthread_mutex_unlock(&lst->next->fork_mtx);
		state(node, states[SLEEP], phindex);
		state(node, states[SLEEP], phindex);
	}
}
