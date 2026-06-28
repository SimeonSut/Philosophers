/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 22:24:03 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/06/28 22:25:25 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static char	ft_addnb(long n, int len);
static long	ft_strcount(long n);

char	*ft_freejoin(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*s3;

	i = 0;
	j = 0;
	while (s1[i])
		i++;
	while (s2[j])
		j++;
	if (i == 0 && j == 0)
		return (free(s1), free(s2), NULL);
	else if (i == 0 && j != 0)
		return (free(s1), s2);
	else if (j == 0 && i != 0)
		return (free(s2), s1);
	s3 = malloc((i + j + 1) * sizeof(char));
	if (!s3)
		return (free(s1), free(s2), NULL);
	s3[i + j] = '\0';
	while (j--)
		s3[i + j] = s2[j];
	while (i--)
		s3[i] = s1[i];
	return (free(s1), free(s2), s3);
}

char	*ft_itoa(int n)
{
	long	nb;
	int		i;
	int		len;
	char	*result;

	nb = n;
	i = 0;
	len = ft_strcount(nb);
	result = malloc((len + 1) * sizeof(char));
	if (!result)
		return (NULL);
    result[len] = '\0';
	if (nb < 0)
	{
		result[i] = '-';
		i++;
	}
	while (i < len)
	{
		result[i] = ft_addnb(nb, len - i);
		i++;
	}
	return (result);
}

static char	ft_addnb(long n, int len)
{
	if (n < 0)
	{
		n = -n;
	}
	while (len > 1)
	{
		n /= 10;
		len--;
	}
	if (n > 9)
	{
		n %= 10;
		return (n + 48);
	}
	return (n + 48);
}

static long	ft_strcount(long n)
{
	int	count;

	count = 0;
	if (n < 0)
	{
		n = -n;
		count++;
	}
	while (n > 9)
	{
		n /= 10;
		count++;
	}
	count++;
	return (count);
}
