/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabae <dabae@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 12:26:23 by dabae             #+#    #+#             */
/*   Updated: 2024/05/06 17:30:46 by dabae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	time_to_stop(t_philo *philo)
{
	mutex_handler(philo->data, &philo->data->exit_lock, LOCK);
	mutex_handler(philo->data, &philo->data->stop_lock, LOCK);
	if (philo->data->stop == 1)
	{
		mutex_handler(philo->data, &philo->data->exit_lock, UNLOCK);
		mutex_handler(philo->data, &philo->data->stop_lock, UNLOCK);		
		return (1);
	}
	mutex_handler(philo->data, &philo->data->exit_lock, UNLOCK);
	mutex_handler(philo->data, &philo->data->stop_lock, UNLOCK);	
	if (philo->data->num_must_eat != -1)
	{
		check_full(philo);
		mutex_handler(philo->data, &philo->data->monitor_lock, LOCK);
		mutex_handler(philo->data, &philo->data->full_lock, LOCK);
		if (philo->data->num_full == philo->data->num_philo)
		{
			mutex_handler(philo->data, &philo->data->monitor_lock, UNLOCK);
			mutex_handler(philo->data, &philo->data->full_lock, UNLOCK);
			return (1);
		}
		mutex_handler(philo->data, &philo->data->monitor_lock, UNLOCK);
		mutex_handler(philo->data, &philo->data->full_lock, UNLOCK);
	}
	return (0);
}

void	*routine(void *philo)
{
	t_philo	*phi;

	phi = (t_philo *)philo;
	while (!time_to_stop(phi))
	{
		if (time_to_stop(phi))
			return (NULL);
		take_forks(phi);
		if (unlock_forks(phi))
			return (NULL);
		eat(phi);
		if (time_to_stop(phi))
			return (NULL);
		sleep_phase(phi);
		if (time_to_stop(phi))
			return (NULL);
		think(phi);
	}
	return (NULL);
}

void	life_cycle(t_data *data)
{
	int	i;

	i = -1;
	data->simul_start = get_time();
	while (++i < data->num_philo)
		pthread_create(&data->tids[i], NULL, &routine, &data->philo[i]);
}
