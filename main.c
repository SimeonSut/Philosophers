/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 18:40:03 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/06/29 21:01:36 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	set_time(t_philo *node);
static void	set_threads(t_philo *node);
static void	*routine_fork(void *arg);
static void join_all_threads(t_philo *node);

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
	join_all_threads(node);
	return (SUCCESS);
}

static void	set_threads(t_philo *node)
{
	int		philos;

	philos = node->n_of_philos;
	node->thrd_i = &philos;
	while (philos >= 0)
	{
		pthread_create(&(node->thread[philos]), NULL, &routine_fork, (void *)node);
		philos--;
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

static void join_all_threads(t_philo *node)
{
	int	i;

	i = 0;
	while (i < node->n_of_philos)
	{
		pthread_join(node->thread[i], NULL);
		i++;
	}
}

static void	*routine_fork(void *arg)
{
	char	*msg;

	msg = ft_itoa(*((t_philo *)arg)->thrd_i);
	write(STDOUT_FILENO, msg, strlen(msg));
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
