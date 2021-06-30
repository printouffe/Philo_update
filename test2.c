/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmontiel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 12:53:44 by pmontiel          #+#    #+#             */
/*   Updated: 2021/06/18 12:53:54 by pmontiel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

long    get_time(void)
{
    struct timeval  tp;
    long            milliseconds;

    gettimeofday(&tp, NULL);
    milliseconds = tp.tv_sec * 1000;
    milliseconds += tp.tv_usec / 1000;
    return (milliseconds);
}

int main(void)
{
    long start_time;
		
		// Remember when we started
    start_time = get_time();

    while (1)
    {
				// Print time from start, in ms
        printf("%ld\n", get_time() - start_time);

				// Sleep 200 times 1000 microseconds (1 millisecond)
        usleep(200 * 1000);
    }
}
