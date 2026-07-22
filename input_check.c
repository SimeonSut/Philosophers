/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 20:21:49 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/22 21:39:52 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int		int_filter(char *input, int len);
static int		is_all_digit(char *input);
static t_philo	*t_philo_init(char **argv);
static t_list	*t_list_init(t_philo *node, t_list *first, int i);

/*CHECK_N_INITIALIZE
 * 
 * 	Input :
 * 		the argv argument of input
 * 
 * 	Behaviour :
 * 		Verifies the validity of input and intiates struct
 * 
 * 	Return :
 * 		The initialize node on succes
 * 		NULL on error at any point
 * 
 * 	Description :
 * 		Uses all static functions of this file
 * 		Checks for positive-integers-only input
 * 		Initiates the node, then the chainlist t_list within it
 */
t_philo	*check_n_initialize(char **argv)
{
	int		i;
	t_philo	*node;
	t_list	*first;

	i = 0;
	while (argv[++i])
		if (int_filter(argv[i], (int)ft_strlen(argv[i])) == ERROR)
			return (NULL);
	node = t_philo_init(argv);
	if (!node)
		return (NULL);
	i = 1;
	node->list = t_list_init(node, NULL, i);
	first = node->list;
	while (++i <= node->n_of_philos)
	{
		node->list->next = t_list_init(node, first, i);
		if (!node || !node->list || !node->list->next)
			return (NULL);
		node->list = node->list->next;
	}
	return (node);
}

/*INT_FILTER
 * 
 * 	Input :
 * 		The input string to check and its len lenght
 * 
 * 	Behaviour :
 * 		Checks it the input is a valid integer
 * 
 * 	Returns :
 * 		SUCCES(0) if valid interger
 * 		ERROR(1) if not
 * 
 * 	Description :
 * 		If lenght is bigger than long long max or a non digit present
 * 		return ERROR
 * 		If at any point, the string is bigger by ascii value to its
 * 		position in the int_max char string, return ERROR
 * 		Upon reaching the end of all checks, assumes its a valid integer
 * 		and return SUCCESS
 */
static int	int_filter(char *input, int len)
{
	int		i;
	char	*int_max;

	i = 0;
	int_max = "2147483647";
	if (len > 21 || is_all_digit(input) == ERROR)
		return (write(2, "wrong input: numerical value required\n", 39), ERROR);
	if (ft_strcmp("-2147483648", input) == 0)
		return (SUCCESS);
	if (input[0] == '-' || input[0] == '+')
		input++;
	if ((len > (int)ft_strlen(int_max)))
	{
		write(2, "wrong input: number in range of int required\n", 46);
		return (ERROR);
	}
	while (input[i] && input[i] <= int_max[i])
		i++;
	if (len == (int)ft_strlen(int_max) && input[i] > int_max[i])
	{
		write(2, "wrong input: number in range of int required\n", 46);
		return (ERROR);
	}
	return (SUCCESS);
}

/*IS_ALL_DIGIT
 * 
 * 	Input :
 * 		the input string to check
 * 
 * 	Behaviour :
 * 		Checks if every char is an integer by ascii
 * 
 * 	Return :
 * 		SUCCESS if all is ascii
 * 		Failure if anything is not ascii
 */
static int	is_all_digit(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] < 48 || input[i] > 57)
			return (ERROR);
		i++;
	}
	if (!input[i] && i == 0)
		return (ERROR);
	return (SUCCESS);
}

/*T_PHILO_INIT
 * 
 * 	Input :
 * 		The argv arguments provided with main
 * 
 * 	Behaviour :
 * 		Initiates the t_philo struct and fill each position with a check
 * 
 * 	Return :
 * 		The initiated struct on success
 * 		NULL on failure
 * 
 * 	Description :
 * 		After initialization, each member of the structed is checked
 * 		If any is less or equal than zero, frees, outputs a message and returns
 */
static t_philo	*t_philo_init(char **argv)
{
	t_philo	*node;

	node = malloc(sizeof(t_philo));
	if (!node)
		return (NULL);
	node->n_of_philos = ft_atoi(argv[1]);
	if (node->n_of_philos <= 0)
		return (free(node), write(2, "wrong philosophers number\n", 27), NULL);
	node->tt_die = ft_atoi(argv[2]);
	if (node->tt_die <= 0)
		return (free(node), write(2, "wrong time to die\n", 19), NULL);
	node->tt_eat = ft_atoi(argv[3]);
	if (node->tt_eat <= 0)
		return (free(node), write(2, "wrong time to eat\n", 19), NULL);
	node->tt_sleep = ft_atoi(argv[4]);
	if (node->tt_sleep <= 0)
		return (free(node), write(2, "wrong time to sleep\n", 21), NULL);
	if (t_philo_additional_setup(node, argv) == ERROR)
		return (free(node), NULL);
	if (pthread_mutex_init(&node->t_philo_mtx, NULL) == -1)
		return (NULL);
	if (pthread_mutex_init(&node->terminal_mtx, NULL) == -1)
		return (NULL);
	return (node);
}

/*T_LIST_INIT
 *
 * 	Input :
 * 		The first t_list struct pre, the i index
 * 
 * 	Behaviour :
 * 		Allocates a chainlist
 * 
 * 	Returns :
 * 		The newly allocated struct on success
 * 		NULL on error
 * 
 * 	Description :
 * 		Allocates the struct and the thread + mutex within it
 * 		On error on any of these steps, frees what it allocated so far
 * 		and return NULL.
 */
static t_list	*t_list_init(t_philo *node, t_list *first, int i)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->i = i;
	new->gate_count = 0;
	new->fork_state = UNLOCKED;
	new->eat_count = 0;
	new->last_eat_tm = 0;
	new->thread = malloc(sizeof(pthread_t));
	if (!new->thread)
		return (free(node), NULL);
	memset((void *)new->thread, 0, 8);
	if (pthread_mutex_init(&new->fork_mtx, NULL) != 0)
		return (free(new), NULL);
	if (new->i != node->n_of_philos)
		new->next = NULL;
	else
		new->next = first;
	return (new);
}
