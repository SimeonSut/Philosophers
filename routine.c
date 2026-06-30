/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 17:32:37 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/06/30 21:44:13 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*routine(void	*arg);
static void	mutex_lock(int	*forks, t_philo	*node, t_list *lst);

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
	int		forks[2];
	t_list	*lst;

	forks[LEFT] = NOT_TAKEN;
	forks[RIGHT] = NOT_TAKEN;
	lst = ((t_philo *)arg)->list;
	return (arg);
}

static void	find_forks(int	*forks, t_philo	*node, t_list *lst)
{
	while (1/*the time doesnt exceed time to die (or its before the first pick of forks)*/)
	{
		if (lst->mtx_state == UNLOCKED && (forks[LEFT] == 0 || forks[RIGHT] == 0))
			mutex_lock(forks, node, lst);
		if (forks[LEFT] == TAKEN && forks[RIGHT] == TAKEN)
			break ;
		lst = lst->next;
	}
}

static void	mutex_lock(int	*forks, t_philo	*node, t_list *lst)
{
	if (lst->mtx_state == UNLOCKED && pthread_mutex_lock(lst->mutex) == 0)
	{
		lst->mtx_state = LOCKED;
		if (forks[LEFT] == NOT_TAKEN)
			forks[LEFT] = TAKEN;/*has taken fork*/
		else if (forks[LEFT] == TAKEN && forks[RIGHT] == NOT_TAKEN)
			forks[RIGHT] = TAKEN;/*has taken fork*/
		else if (forks[LEFT] == TAKEN && forks[RIGHT] == TAKEN)
			;/*philosophers is eating*/
	}
}
