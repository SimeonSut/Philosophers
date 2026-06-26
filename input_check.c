/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: simeon <simeon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 20:21:49 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/06/26 07:51:24 by simeon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int		int_filter(char *input, int len);
static int  	is_all_digit(char *input);
static t_philo	*t_philo_init(char **argv);

t_philoe	is_valid_input(char **argv)
{
	int	i;
	int arg_len;

	i = 1;
	arg_len = 0;
	while (argv[i])
	{
		arg_len = (int)ft_strlen(argv[i]);
		if (int_filter(argv[i++], arg_len) == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
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
		return (write(2, "wrong input: numerical value required", 38), ERROR);
	if (ft_strcmp("-2147483648", input) == 0)
		return (SUCCESS);
	if (input[0] == '-' || input[0] == '+')
		input++;
	if ((len > (int)ft_strlen(int_max)))
	{
		write(2, "wrong input: number in scope of range required\n", 48);
		return (ERROR);
	}
	while (input[i] && input[i] <= int_max[i])
		i++;
	if (len == (int)ft_strlen(int_max) && input[i] > int_max[i])
	{
		write(2, "wrong input: number in scope of range required\n", 48);
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
	while (input)
	{
		if (*input < 48 || *input > 57)
		{
			write(2, "wrong input %s, (fully) numerical value required\n", 50);
			return (ERROR);
		}
		input++;
	}
	return (SUCCESS);
}

static t_philo	*t_philo_init(char **argv)
{
	t_philo *node;

	node = malloc(sizeof(t_philo));
	if (!node)
		return (NULL);
}
