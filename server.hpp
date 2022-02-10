/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcheiko <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 11:50:46 by rcheiko           #+#    #+#             */
/*   Updated: 2022/02/10 12:14:59 by rcheiko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <cstring>
# include <cstdlib>
# include <vector>
# include <sys/socket.h> // For socket functions
# include <netinet/in.h> // For sockaddr_in

class server{

	public:

		server(void){}
		~server(){}
	
		void init_socket(int port)
		{
			socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (socketfd == -1)
			{
				std::cout << "socket error" << std::endl;
				exit(EXIT_FAILURE);
			}
			else
				std::cout << "socket created" << std::endl;
			sin.sin_family = AF_INET;
			sin.sin_port = htons(port);
			sin.sin_addr.s_addr = INADDR_ANY;
			
			int optval = 1;
			if ((setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int))) == -1)
			{
				std::cout << "Set Socket Error \n";
				exit(EXIT_FAILURE);
			}
			else
				std::cout << "Set Socket Success" << std::endl;
	
			if (bind(socketfd, (sockaddr*)&sin, sizeof(sin)) == -1)
			{
				std::cout << "bind error" << std::endl;
				exit(EXIT_FAILURE);
			}
			else
				std::cout << "bind was linked" << std::endl;
	
			if (listen(socketfd, SOMAXCONN) < 0)
			{
				std::cout << "listen error" << std::endl;
				exit(EXIT_FAILURE);
			}
			else
				std::cout << "listen success" << std::endl;
		}
	
		void	setPassword(int pass)
		{
			password = pass;
		}

	private:
		int					socketfd;
		int					password;
		std::vector<int>	clientfd;
		sockaddr_in			sin;
		std::string			buffer;

};


#endif
