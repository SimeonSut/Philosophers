/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 20:21:49 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/06/28 17:03:28 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int				int_filter(char *input, int len);
static int  			is_all_digit(char *input);
static t_philo			*t_philo_init(char **argv);
static pthread_mutex_t	*mutex_initializer(int n_of_philos);

t_philo	*check_n_initialize(char **argv)
{
	int		i;
	int 	arg_len;
	t_philo	*node;

	i = 1;
	arg_len = 0;
	while (argv[i])
	{
		arg_len = (int)ft_strlen(argv[i]);
		if (int_filter(argv[i++], arg_len) == ERROR)
			return (NULL);
	}
	node = t_philo_init(argv);
	if (argv[5])
	{
		node->t_must_eat = ft_atoi(argv[5]);
		if (node->t_must_eat <= 0)
			return (free(node), write(2, "wrong must eat input\n", 22), NULL);
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
	if (len > 21 ||  is_all_digit(input) == ERROR)
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
static int  is_all_digit(char *input)
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
	t_philo *node;

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
	node->thread = malloc(node->n_of_philos * sizeof(pthread_t));
	if (!node->thread)
		return (free(node), NULL);
	node->mutex = mutex_initializer(node->n_of_philos);
	if (!node->mutex)
		return (free(node->thread), free(node), NULL);
	node->time = NULL;
	return (node);
}

/*MUTEX_INITIALIZER
 * 
 * 	Input :
 * 		The n_of_philos number of philosophers
 * 
 * 	Behaviour :
 * 		Allocated an array of the correct amount of mutexes an initializes it
 * 
 * 	Returns :
 * 		The allocated and initilialized array on success
 * 		NULL on failure
 * 
 * 	Description :
 * 		Once the array is allocated iterates while calling the mutex init
 * 		given function
 */
static pthread_mutex_t	*mutex_initializer(int n_of_philos)
{
	int				i;
	pthread_mutex_t	*mutex;

	i = 0;
	mutex = malloc(n_of_philos * sizeof(pthread_mutex_t));
	if (!mutex)
		return (NULL);
	while (i < n_of_philos)
	{
		if (pthread_mutex_init(&(mutex[i++]), NULL) != 0)
		{
			write(2, "pthread_mutex_init error\n", 26);
			return (free(mutex), NULL);
		}
	}
	return (mutex);
}
