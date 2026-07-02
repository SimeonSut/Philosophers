/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:34:46 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/02 20:53:11 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void uneven_uneven_cycle(t_list *lst)
{
	pthread_mutex_lock(lst->mutex);
	/*time to eat to input here*/;
	pthread_mutex_lock(lst->next->mutex);
	/*time to eat to input here*/;
}

void uneven_even_cycle(t_list *lst)
{
	pthread_mutex_lock(lst->mutex);
	/*time to eat to input here*/;
	pthread_mutex_lock(lst->next->mutex);
	/*time to eat to input here*/;
}

void even_uneven_cycle(t_list *lst)
{
	pthread_mutex_lock(lst->mutex);
	/*time to eat to input here*/;
	pthread_mutex_lock(lst->next->mutex);
	/*time to eat to input here*/;
}

void even_even_cycle(t_list *lst)
{
	pthread_mutex_lock(lst->mutex);
	/*time to eat to input here*/;
	pthread_mutex_lock(lst->next->mutex);
	/*time to eat to input here*/;
}
