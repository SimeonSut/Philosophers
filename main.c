/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 18:40:03 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/06/28 22:29:25 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	set_time(t_philo *node);
static void	set_threads(t_philo *node);
static void	*routine_fork(void *arg);

int main(int argc, char **argv)
{
	t_philo	*node;
	
	if (argc < 5 || argc > 6)
	{
		write(2, "incorrect number of arguments\n", 31);
		return (ERROR);
	}
	node = check_n_initialize(argv);
	if (!node)
		return (ERROR);
	set_time(node);
	set_threads(node);
	return (SUCCESS);
}

static void	set_threads(t_philo *node)
{
	int	philos;

	philos = node->n_of_philos;
	while (philos >= 0)
	{
		pthread_create(&(node->thread[--philos]), NULL, &routine_fork, (void *)node);
	}
	return ;
}

static void	set_time(t_philo *node)
{
	node->start_time = malloc(sizeof(t_time));
	if (!node->start_time)
		return ;
	if (gettimeofday(node->start_time, NULL) == -1)
		return ;
}

static void	*routine_fork(void *arg)
{
	struct timeval	cur_time;
	char			*fork_msg;

	gettimeofday(&cur_time, NULL);
	fork_msg = ft_itoa(cur_time.tv_sec - ((t_philo *)arg)->start_time->tv_sec);
	if (!fork_msg)
		return (NULL);
	write(STDOUT_FILENO, &fork_msg, strlen(fork_msg));
	return (NULL);
}

/*
 * memset
 * printf
 * malloc
 * free
 * write
 * usleep
 * gettimeofday
 * pthread_create
 * pthread_detach
 * pthread_join
 * pthread_mutex_init
 * pthread_mutex_destroy
 * pthread_mutex_lock
 * pthread_mutex_unlock
 */
