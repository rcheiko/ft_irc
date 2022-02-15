/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmontiel <pmontiel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 12:05:52 by rcheiko           #+#    #+#             */
/*   Updated: 2022/02/15 15:16:29 by pmontiel         ###   ########.fr       */
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
	serv.setPassword(av[2]);
	serv.init_socket(std::atoi(av[1]));
	serv.k_init();
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
