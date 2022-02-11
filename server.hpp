/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmontiel <pmontiel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 11:50:46 by rcheiko           #+#    #+#             */
/*   Updated: 2022/02/11 16:35:05 by pmontiel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <unistd.h>
# include <cstring>
# include <cstdlib>
# include <vector>
# include <netdb.h>
# include <poll.h>
# include <sys/socket.h> // For socket functions
# include <netinet/in.h> // For sockaddr_in
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>

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
		}
		void	k_init()
		{
			int new_events;
			int kq;
			int con;
			kq = kqueue();
			EV_SET(change_event, socketfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
			if (kevent(kq, change_event, 1, NULL, 0, NULL) == -1)
		    {
		        perror("kevent");
		        exit(1);
		    }
			while (1)
			{
				new_events = kevent(kq, NULL, 0, event, 1, NULL); 
				if (new_events == -1)
		        {
		            perror("kevent");
		            exit(1);
		        }
				for (int i = 0; new_events > i; i++)
        		{
        		    event_fd = event[i].ident;
					if (event[i].flags & EV_EOF)
    	        	{
    	        	    std::cout << "Client has disconnected\n";
    	        	    close(event_fd);
    	        	}
					else if (event_fd == socketfd)
					{
						con = init_accept();
						EV_SET(change_event, con, EVFILT_READ, EV_ADD, 0, 0, NULL);
						if (kevent(kq, change_event, 1, NULL, 0, NULL) < 0)
		                {
		                    perror("kevent error");
		                }
					}
					else if (event[i].filter & EVFILT_READ)
		            {
		                char buf[1024];
		                size_t bytes_read = recv(event_fd, buf, sizeof(buf), 0);
		                std::cout << "read " << bytes_read << " bytes" << "\n";
						send(event_fd, buf, bytes_read, 0);
		            }
				}				
			}
		}
		int	init_accept()
		{
			char buf[256];
			int d;
		    int l;
			sockaddr_in	c;
			memset(buf ,0 , 256);
		    if ((d = accept(event_fd, (sockaddr *)&c, (socklen_t *)&l)) == -1)
		    {
		        std::cout << "\t--Accept error\n";
		        exit(EXIT_FAILURE);  
		    }
		    else
		        std::cout << "\t--New client connect from port no. " << ntohs(sin.sin_port) << "\n";
			while (std::strcmp(buf, password) != 0)
			{
				send(d, "ENTER A PASSWORD : ", 19, 0);
		    	int a = recv(d, buf, sizeof(buf), 0);
				buf[a - 1] = '\0';
			}
			return (d);
		}
		void	setPassword(char* pass)
		{
			password = pass;
		}

	private:
		int					socketfd;
		char*				password;
		std::vector<int>	clientfd;
		sockaddr_in			sin;
		int 				event_fd;
		struct kevent 		change_event[4], event[4];


};


#endif
