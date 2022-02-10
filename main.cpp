/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcheiko <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 12:05:52 by rcheiko           #+#    #+#             */
/*   Updated: 2022/02/10 12:14:05 by rcheiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "error argument" << std::endl;
		exit(EXIT_FAILURE);
	}

	server serv;
	serv.setPassword(std::atoi(av[2]));
	serv.init_socket(std::atoi(av[1]));

	return (0);
}
