/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_and_states.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 23:50:35 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/05 20:00:25 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	fill_states_times(t_philo *node, int *states)
{
	states[DIE] = node->tt_die;
	states[EAT] = node->tt_eat;
	states[SLEEP] = node->tt_sleep;
}

void	state(t_philo *node, int time_to_state, int phindex)
{
	struct timeval		tm;
	long long			microstart;
	long long			check_tm;
	int					timestamp;

	pthread_mutex_lock(&node->t_philo_mtx);
	microstart = node->microstart;
	pthread_mutex_unlock(&node->t_philo_mtx);
	if (gettimeofday(&tm, NULL) == -1)
		return ;//gettimeofday error
	timestamp = (((tm.tv_sec * 1000000LL + tm.tv_usec) - microstart) /1000);
	printf("%d %d is eating\n", timestamp, phindex);
	//send_msg(timestamp, phindex, state);
	check_tm = (long long)time_to_state * 1000LL;
	while ((tm.tv_sec * 1000000LL + tm.tv_usec) - microstart < check_tm)
	{
		if (gettimeofday(&tm, NULL) == -1)
			return ;//gettimeofday error
	}
	return ;
}
/*
int	send_msg(int timestamp, int phindex, char *state)
{
	char	*msg;
	char	*timestmp;
	char	*philo_index;
	char	*tmp;

	timestmp = ft_itoa(timestamp);
	philo_index = ft_itoa(phindex);
	tmp = ft_strdup(" ");
	if (!timestmp || !philo_index || !tmp)
		return (ERROR);
	msg = ft_freejoin(timestmp, tmp);
	if (!msg)
		return (free(timestmp), free(philo_index), free(tmp), ERROR);
	msg = ft_freejoin(msg, philo_index);
	if (!msg)
		return (free(timestmp), free(tmp), ERROR);
	tmp = ft_strdup(state);
	if (!tmp)
		return (free(timestmp), free(msg), ERROR);
	msg = ft_freejoin(msg, tmp);
	if (!msg)
		return (free(timestmp), ERROR);
	write(STDOUT_FILENO, msg, (int)ft_strlen(msg));
	return (free(msg), free(timestmp), SUCCESS);
}*/
