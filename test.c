/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmontiel <pmontiel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 12:37:34 by pmontiel          #+#    #+#             */
/*   Updated: 2021/06/22 12:01:04 by pmontiel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_philo
{
	pthread_t		philo;
	pthread_mutex_t	*right;
	pthread_mutex_t	*left;
	int				time_die;
	int				time_sleep;
	int				time_eat;
	int				nb_philo;
	int				id;
	long			time_start;
}				t_philo;

typedef struct s_infos
{
	int				time_die;
	int				time_sleep;
	int				time_eat;
	int				nb_philo;
	int				*li;
	pthread_t		*thread;
	pthread_mutex_t	*forks;
	t_philo			*p;

}				t_infos;

void	init_struct(t_infos *i)
{
	i->time_die = 0;
	i->time_sleep = 0;
	i->time_eat = 0;
	i->nb_philo = 0;
	i->li = NULL;
}

void	fill_struct_info(t_infos *i)
{
	i->time_die = i->li[1];
	i->time_eat = i->li[2];
	i->time_sleep = i->li[3];
	i->nb_philo = i->li[0];
	i->p = malloc(sizeof(t_philo) * i->li[0]);
}

long	ft_time(void)
{
	struct timeval	s;

	gettimeofday(&s, NULL);
	return ((s.tv_sec * 1000) + (s.tv_usec / 1000));
}

unsigned long long	gettime(void)
{
	struct timeval		time;
	unsigned long long	ms;

	gettimeofday(&time, NULL);
	ms = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (ms);
}

long time_diff(struct timeval *start, struct timeval *end)
{
	return (end->tv_sec - start->tv_sec) + 1e-6 * (end->tv_usec - start->tv_usec);
}

int	ft_isdigit(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			return (1);
		i++;
	}
	return (0);
}

int	ft_atoi(const char *str)
{
	unsigned int res;
	unsigned int sign;

	res = 0;
	sign = 1;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = sign * -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
	}
	return (sign * res);
}

void	malloc_tab(t_infos *i)
{
	i->thread = malloc(sizeof(pthread_t) * i->li[0]);
	i->forks = malloc(sizeof(pthread_mutex_t) * i->li[0]);
}

void	fill_tab_struct(t_infos *i)
{
	int len;
	int count;

	len = 0;
	while (len < i->li[0])
	{
		i->p[len].right = &i->forks[(len + 1) % i->li[0]];
		i->p[len].left = &i->forks[len];
		len++;
	}
	len = 0;
	count = 1;
	while (len < i->li[0])
	{
		i->p[len].time_eat = i->li[2];
		i->p[len].time_die = i->li[1];
		i->p[len].time_sleep = i->li[3];
		i->p[len].nb_philo = i->li[0];
		i->p[len].id = count;
		len++;
		count++;
	}
}

void *philosophers(void *arg)
{
	t_philo *p;
	p = (t_philo *)arg;
	//struct timeval start;
	//struct timeval end;
	//gettimeofday(&start, NULL);
	while (1)
	{
		pthread_mutex_lock(p->right);
		//gettimeofday(&end, NULL);
		//printf("time:%ld ", time_diff(&start, &end));
		printf("%d -> TAKE A FORK\n", p->id);
		//gettimeofday(&p->end, NULL);
		//printf("%f\n", time_diff(&p->start, &p->end));
		pthread_mutex_lock(p->left);
		//gettimeofday(&end, NULL);
		//printf("time:%ld ", time_diff(&start, &end));
		printf("%d -> TAKE A FORK\n", p->id);
		usleep(p->time_eat * 1000);
		//gettimeofday(&p->end, NULL);
		printf("%d -> EATING\n", p->id);
		pthread_mutex_unlock(p->right);
		printf("%d -> DROP A FORK\n", p->id);
		pthread_mutex_unlock(p->left);
		printf("%d -> DROP A FORK\n", p->id);
		usleep(p->time_sleep * 1000);
		printf("%d -> SLEEPING\n", p->id);
		printf("%d -> THINKING\n", p->id);
	}
	return (0);
}

void	init_mutex(t_infos *i)
{
	int	len;

	len = 0;
	while (len < i->li[0])
	{
		pthread_mutex_init(&i->forks[len], NULL);
		len++;
	}
}

void	init_threads(t_infos *i)
{
	int	len;

	len = 0;
	while (len < i->li[0])
	{
		if (len % 2)
			pthread_create(&i->thread[len], NULL, philosophers, &i->p[len]);
		len++;
	}
	usleep(100);
	len = 0;	
	while (len < i->li[0])
	{
		if (!len % 2)
			pthread_create(&i->thread[len], NULL, philosophers, &i->p[len]);
		len++;
	}

	len = 0;
	while (len < i->li[0])
	{
		pthread_join(i->thread[len], NULL);
		len++;
	}
}

int main(int ac, char **av)
{
	int		i;
	int		j;
	//struct timeval start;
	//struct timeval end;
	//float time;
	t_infos d;
	init_struct(&d);
	if (ac > 6)
		return (write(1,"Error\n", 6));
	else if (ac < 5)
		return (write(1,"Error\n", 6));
	d.li = malloc(sizeof(int) * ac - 1);
	i = 1;
	j = 0;
	while (i < ac)
	{
		if (ft_isdigit(av[i]) == 1)
			d.li[j] = ft_atoi(av[i]);
		else
			return (write(1,"Error\n", 6));
		j++;
		i++;
	}
	i = 0;
	if (!d.li[0])
		return (write(1,"Error\n", 6));
	fill_struct_info(&d);
	malloc_tab(&d);
	fill_tab_struct(&d);
	init_mutex(&d);
	init_threads(&d);
}
