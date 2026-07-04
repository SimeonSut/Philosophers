/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_and_states.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 23:50:35 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/05 00:07:42 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	fill_states_times(t_philo *node, int *states)
{
	states[DIE] = node->tt_die;
	states[EAT] = node->tt_eat;
	states[SLEEP] = node->tt_sleep;
}

void	state(t_philo *node, char *state, int time_to_state, int time_to_die)
{
	t_time	*tm;
	long	micro_start;

	tm = malloc(sizeof(tm));
	if (!tm)
		return ;//malloc error
	if (gettimeofday(tm, NULL) == -1)
		return ;//gettimeofday error
	micro_start = tm->tv_sec * 1000000 + tm->tv_usec;
	return ;
}
