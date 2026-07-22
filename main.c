/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 18:40:03 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/22 21:37:00 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	clean_t_list(t_list *lst);
static void	clean_t_philo(t_philo *node);

int	main(int argc, char **argv)
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
	while (node->list->i != 1)
		node->list = node->list->next;
	thread_setup(node);
	clean_t_list(node->list);
	clean_t_philo(node);
	return (SUCCESS);
}

static void	clean_t_list(t_list *lst)
{
	t_list	*target;
	int		i;

	i = lst->i;
	if (lst->next)
		lst = lst->next;
	while (lst->i != i)
	{
		pthread_mutex_destroy(&lst->fork_mtx);
		free(lst->thread);
		target = lst;
		lst = lst->next;
		free(target);
		target = NULL;
	}
	pthread_mutex_destroy(&lst->fork_mtx);
	free(lst->thread);
	free(lst);
	lst = NULL;
}

static void	clean_t_philo(t_philo *node)
{
	int	i;
	int	gates_nbr;

	i = 0;
	gates_nbr = node->n_of_philos / 2;
	if (node->n_of_philos % 2 != 0)
		gates_nbr++;
	while (i < gates_nbr)
		pthread_mutex_destroy(&node->gates_mtx[i++]);
	free(node->gates_mtx);
	pthread_mutex_destroy(&node->t_philo_mtx);
	pthread_mutex_destroy(&node->terminal_mtx);
	free(node);
	node = NULL;
}
