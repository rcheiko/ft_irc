/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmontiel <pmontiel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 11:50:46 by rcheiko           #+#    #+#             */
/*   Updated: 2022/02/10 19:29:08 by pmontiel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <cstring>
# include <cstdlib>
# include <vector>
# include <netdb.h>
# include <poll.h>
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
				std::cout << "\t--Socket error" << std::endl;
				exit(EXIT_FAILURE);
			}
			else
				std::cout << "\t--Socket created" << std::endl;
			sin.sin_family = AF_INET;
			sin.sin_port = htons(port);
			sin.sin_addr.s_addr = INADDR_ANY;
			
			int optval = 1;
			if ((setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int))) == -1)
			{
				std::cout << "\t--Set Socket Error \n";
				exit(EXIT_FAILURE);
			}
			else
				std::cout << "\t--Set Socket Success" << std::endl;
	
			if (bind(socketfd, (sockaddr*)&sin, sizeof(sin)) == -1)
			{
				std::cout << "\t--Bind error" << std::endl;
				exit(EXIT_FAILURE);
			}
			else
				std::cout << "\t--Bind was linked" << std::endl;
	
			if (listen(socketfd, SOMAXCONN) < 0)
			{
				std::cout << "\t--Listen error" << std::endl;
				exit(EXIT_FAILURE);
			}
			else
				std::cout << "\t--Listen success" << std::endl;
			fds[0].fd = socketfd;
  			fds[0].events = POLLIN;			
		}
		int	poll_init(int fd)
		{
			int res;
			memset(fds, 0 , sizeof(fds));
			res = poll(fds, fd, -1);
			if (res == -1)
			{
				std::cout << "\t--Poll error" << std::endl;
				exit(EXIT_FAILURE);
			}
			else
				std::cout << "\t--Poll success" << std::endl;
			return (res);
		}
	
		int	init_accept()
		{
			int d;
		    int l;
			sockaddr_in	c;
		    if ((d = accept(socketfd, (sockaddr *)&c, (socklen_t *)&l)) == -1)
		    {
		        std::cout << "\t--Accept error\n";
		        exit(EXIT_FAILURE);  
		    }
		    else
		        std::cout << "\t--New client connect from port no. " << ntohs(sin.sin_port) << "\n";
			return (d);
		}
		void	recv_send()
		{
			int nfds = 1;	
			std::string buffer[256];
			memset(buffer ,0 , 256);
			int d = 0;
			int i = 0;
			int res = 0;
			std::cout << "\t-- ICI --\n";
			while (1)
			{
				std::cout << "\t-- ICI 1--\n";
				res = poll_init(nfds);
				std::cout << "\t-- ICI 2--\n";
				if (res == -1)
				{
					std::cout << "\t--poll error\n";
		  		    exit(EXIT_FAILURE); 	
				}
				if (res == 0)
				{
					std::cout << "\t--poll time out\n";
		  		    exit(EXIT_FAILURE);
				}
				
				while (d != -1)
				{
					d = init_accept();
					fds[nfds].fd = d;
	 		        fds[nfds].events = POLLIN;
   		       		nfds++;
					while (1)
					{
						int rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
						if (rc == -1)
						{
							std::cout << "\t--Recv error" << std::endl;
							exit(EXIT_FAILURE);
						}
						else
							std::cout << "\t--Recv success" << std::endl;
						if (send(fds[i].fd, buffer, rc, 0) == -1)
						{
							std::cout << "\t--Send error" << std::endl;
							exit(EXIT_FAILURE);
						}
						else
							std::cout << "\t--Send success" << std::endl;
					}
				}		
			}
		}
		void	setPassword(int pass)
		{
			password = pass;
		}

	private:
		int					socketfd;
		int					password;
		//int					f_poll;
		std::vector<int>	clientfd;
		sockaddr_in			sin;
		pollfd				fds[200];

};


#endif
