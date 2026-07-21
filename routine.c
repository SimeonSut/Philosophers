/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:34:46 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/21 18:00:01 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	take_a_fork(t_philo *node, t_list *lst, int phindex, int side);
static int	state(t_philo *node, char *action, int time_to_state, int phindex);
static int	try_fork(t_list *lst, int side);
static int	drop_fork(t_list *lst);

void routine(t_philo *node, t_list *lst, int phindex, int *states)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	while (death_check(node, t, phindex) == ALIVE)
	{
		if (take_a_fork(node, lst, phindex, LEFT) == DEAD)
			break ;
		if (!lst->next || take_a_fork(node, lst, phindex, RIGHT) == DEAD)
			break ;
		open_close_gates(node, lst, phindex, UNLOCK);
		if (state(node, "eating", states[EAT], phindex) == DEAD)
			break ;
		drop_fork(lst);
		if (state(node, "sleeping", states[SLEEP], phindex) == DEAD)
			break ;
		announce_thinking(node, phindex);
		gettimeofday(&t, NULL);
	}
	if (!lst->next)
		wait_death_time(node);
}

static int	state(t_philo *node, char *action, int time_to_state, int phindex)
{
	struct timeval		t;
	struct timeval		death_count_t;
	long long			tm;

	gettimeofday(&t, NULL);
	tm = (((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart) / 1000LL);
	pthread_mutex_lock(&node->terminal_mtx);
	printf("%lld %d is %s\n", tm, phindex, action);
	pthread_mutex_unlock(&node->terminal_mtx);
	tm = (t.tv_sec * 1000000LL + t.tv_usec);
	death_count_t = t;
	while (((t.tv_sec * 1000000LL + t.tv_usec) - tm) < time_to_state * 1000LL)
	{
		usleep(500);
		if (death_check(node, death_count_t, phindex) == DEAD)
		{
			printf("bla\n");
			return (DEAD);
		}
		gettimeofday(&t, NULL);
	}
	return (ALIVE);
}

static int	take_a_fork(t_philo *node, t_list *lst, int phindex, int side)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	while (death_check(node, t, phindex) == ALIVE)
	{
		if (try_fork(lst, side) == YES)
		{
			lst->forks_held++;
			announce_fork_taken(node, phindex);
			break ;
		}
		usleep(10);
	}
	if (death_check(node, t, phindex) == DEAD)
		return (DEAD);
	return (ALIVE);
}

static int	try_fork(t_list *lst, int side)
{
	if (side == LEFT)
	{
		pthread_mutex_lock(&lst->fork_mtx);
		if (lst->fork_state == UNLOCKED)
		{
			lst->fork_state = LOCKED;
			pthread_mutex_unlock(&lst->fork_mtx);
			return (YES);
		}
		pthread_mutex_unlock(&lst->fork_mtx);
	}
	else if (side == RIGHT)
	{
		pthread_mutex_lock(&lst->next->fork_mtx);
		if (lst->next->fork_state == UNLOCKED)
		{
			lst->next->fork_state = LOCKED;
			pthread_mutex_unlock(&lst->next->fork_mtx);
			return (YES);
		}
		pthread_mutex_unlock(&lst->next->fork_mtx);
	}
	return (NO);
}

static int	drop_fork(t_list *lst)
{
	pthread_mutex_lock(&lst->fork_mtx);
	lst->fork_state = UNLOCKED;
	pthread_mutex_unlock(&lst->fork_mtx);
	lst->forks_held--;
	pthread_mutex_lock(&lst->next->fork_mtx);
	lst->next->fork_state = UNLOCKED;
	pthread_mutex_unlock(&lst->next->fork_mtx);
	lst->forks_held--;
	return (ALIVE);
}
