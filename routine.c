/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 17:32:37 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/06 21:17:29 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*start(void	*arg);
static void	odd_philos(t_philo *node, t_list *lst, int phindex, int *states);
static void even_philos(t_philo *node, t_list *lst, int phindex, int *states);
static void	take_a_fork(t_philo *node, t_list *lst, int phindex);

void		thread_setup(t_philo *node)
{
	int		i;
	t_list	*lst;

	lst = node->list;
	i = lst->i;
	while (1)
	{
		pthread_create(lst->thread, NULL, &start, (void *)node);
		lst = lst->next;
		if (lst->i == i)
			break;
	}
	i = lst->i;
	while (1)
	{
		pthread_join(*lst->thread, NULL);
		lst = lst->next;
		if (lst->i == i)
			break;;
	}
}

static void	*start(void	*arg)
{
	t_philo	*node;
	t_list	*lst;
	int		phindex;
	int		states[3];

	phindex = 0;
	pthread_mutex_lock(&((t_philo *)arg)->t_philo_mtx);
	node = ((t_philo *)arg);
	lst = node->list;
	phindex = node->list->i;
	states[EAT] = node->tt_eat;
	states[SLEEP] = node->tt_sleep;
	states[THINK] = -1;
	if (phindex % 2 == 0)
		open_close_gates(node, phindex, CLOSE);
	node->list = node->list->next;
	pthread_mutex_unlock(&node->t_philo_mtx);
	if (phindex % 2 == 0)
		even_philos(node, lst, phindex, states);
	else
	{
		open_close_gates(node, phindex, CLOSE);
		odd_philos(node, lst, phindex, states);
	}
	return (arg);
}

static void even_philos(t_philo *node, t_list *lst, int phindex, int *states)
{
	struct timeval	tm;
	long long		microstart;
	long long		timestamp;

	pthread_mutex_lock(&node->t_philo_mtx);
	microstart = node->microstart;
	pthread_mutex_unlock(&node->t_philo_mtx);
	while (1)
	{
		take_a_fork(node, lst, phindex);
		take_a_fork(node, lst->next, phindex);
		open_close_gates(node, phindex, OPEN);
		state(node, "eating", states[EAT], phindex);
		pthread_mutex_unlock(&lst->fork_mtx);
		pthread_mutex_unlock(&lst->next->fork_mtx);
		state(node, "sleeping", states[SLEEP], phindex);
		if (gettimeofday(&tm, NULL) == -1)
			break ;//gettimeofday error
		timestamp = (tm.tv_sec * 1000000LL + tm.tv_usec) - microstart;
		pthread_mutex_lock(&node->terminal_mtx);
		printf("%lld %d is thinking\n", timestamp, phindex);
		pthread_mutex_unlock(&node->terminal_mtx);
	}
}

static void	odd_philos(t_philo *node, t_list *lst, int phindex, int *states)
{
	struct timeval	tm;
	long long		microstart;
	int				timestamp;

	pthread_mutex_lock(&node->t_philo_mtx);
	microstart = node->microstart;
	pthread_mutex_unlock(&node->t_philo_mtx);
	while (1)
	{
		take_a_fork(node, lst, phindex);
		take_a_fork(node, lst->next, phindex);
		state(node, "eating", states[EAT], phindex);
		pthread_mutex_unlock(&lst->fork_mtx);
		pthread_mutex_unlock(&lst->next->fork_mtx);
		state(node, "sleeping", states[SLEEP], phindex);
		if (gettimeofday(&tm, NULL) == -1)
			break ;//gettimeofday error
		timestamp = (tm.tv_sec * 1000000LL + tm.tv_usec) - microstart;
		pthread_mutex_lock(&node->terminal_mtx);
		printf("%d %d is thinking\n", timestamp, phindex);
		pthread_mutex_unlock(&node->terminal_mtx);
	}
}

static void	take_a_fork(t_philo *node, t_list *lst, int phindex)
{
	struct timeval	tm;
	long long		timestmp;
	long long		microstart;

	pthread_mutex_lock(&lst->fork_mtx);
	pthread_mutex_lock(&node->t_philo_mtx);
	microstart = node->microstart;
	pthread_mutex_unlock(&node->t_philo_mtx);
	if (gettimeofday(&tm, NULL) == -1)
		return ;//gettimeofday error
	timestmp = (((tm.tv_sec * 1000000LL + tm.tv_usec) - microstart) / 1000LL);
	pthread_mutex_lock(&node->terminal_mtx);
	printf("%lld %d has taken a fork\n", timestmp, phindex);
	pthread_mutex_unlock(&node->terminal_mtx);
}
