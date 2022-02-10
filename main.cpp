/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcheiko <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 12:05:52 by rcheiko           #+#    #+#             */
/*   Updated: 2022/02/10 14:25:12 by rcheiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "error argument : ./ircserv <port> <password>" << std::endl;
		exit(EXIT_FAILURE);
	}

	server serv;
	serv.setPassword(std::atoi(av[2]));
	serv.init_socket(std::atoi(av[1]));

	return (0);
}


	/*protoent *a; // FONCTION GET PROTO BY NAME
	const char str[] = "TCP";
	a = getprotobyname(str);
	std::cout << "protocol : " << a->p_name << std::endl;
	std::cout << "numero protocol : " << a->p_proto << std::endl;
	for(int i = 0; a->p_aliases[i]; i++)
	{
		std::cout << "protocol : " << a->p_aliases << std::endl;
	}*/
