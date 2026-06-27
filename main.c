/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 18:40:03 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/06/27 23:01:29 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
	return (SUCCESS);
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
