/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmontiel <pmontiel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 11:50:46 by rcheiko           #+#    #+#             */
/*   Updated: 2022/02/16 11:53:37 by pmontiel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <unistd.h>
# include <cstring>
# include <cstdlib>
# include <map>
# include <netdb.h>
# include <poll.h>
# include <sys/socket.h> // For socket functions
# include <netinet/in.h> // For sockaddr_in
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <stdlib.h>
#include <arpa/inet.h>

class server{

	public:

		server(void){
			fillBool();
			fillPassword();		
		}
		~server(){}

		int		count_word(char *str, char c)
		{
			int		i;
			int		word;

			word = 0;
			i = 0;
			while (str[i])
			{
				while (str[i] && str[i] == c)
					i++;
				if (str[i] && str[i] != c)
				{
					word++;
					while (str[i] && str[i] != c)
						i++;
				}
			}
			return (word);
		}

		char		*tab_malloc(char *str, char c)
		{
			int		i;
			char	*tab;

			i = 0;
			while (str[i] && str[i] != c)
				i++;
			if (!(tab = new char [i + 1]))
				return (NULL);
			i = 0;
			while (str[i] && str[i] != c)
			{
				tab[i] = str[i];
				i++;
			}
			tab[i] = '\0';
			return (tab);
		}

		char			**ft_split(char *s, char c)
		{
			int		i;
			int		i_tab;
			int		total_word;
			char	**tab;

			i = 0;
			i_tab = 0;
			total_word = count_word(s, c);
			if (!(tab = new char * [total_word + 1]))
				return (NULL);
			while (s[i])
			{
				while (s[i] == c)
					i++;
				if (s[i] && s[i] != c)
				{
					tab[i_tab] = tab_malloc(s + i, c);
					i_tab++;
					while (s[i] && s[i] != c)
						i++;
				}
			}
			tab[i_tab] = 0;
			return (tab);
		}

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
			sin.sin_addr.s_addr = inet_addr("127.0.0.1");

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
		int ft_strlen(char *str)
		{
			int i = 0;
			while (str[i])
				i++;
			return (i);
		}
		void	k_init()
		{
			int new_events;
			int kq;
			int con;
			char	*com;
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
					std::map<int, Node*>::iterator it = users.begin();
					std::map<int, Node*>::iterator ite = users.end();
					for (; it != ite; it++)
					{
						if (it->first == event_fd)
							break;
					}
					if (it == ite)
					{
						Node *node = new Node;
						node->ope = false;			
						users[event_fd] = node;
					}
					if (checkPassword[event_fd - 5] == -1)
						send(event_fd, "ENTER PASSWORD : ", 17, 0);

					if (event[i].flags & EV_EOF)
					{
						std::cout << "Client has disconnected" << std::endl;;
						checkPassword[event_fd - 5] = -1;
						users.erase(event_fd);
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
						buf[bytes_read - 1] = '\0';
						if (checkAll(event_fd) == 1 && strncmp(buf, "/send ", 6) == 0)
						{
							char **str = ft_split(buf, ' ');
							std::map<int, Node*>::iterator it = users.begin();
							std::map<int, Node*>::iterator ite = users.end();
							for (; it != ite; it++)
							{
								char *user = &it->second->username[0];
								if (strcmp(user, str[1]) == 0)
								{
									send(it->first, "[ Private ] [", 13, 0);
									send(it->first, user, ft_strlen(user), 0);
									send(it->first, "] : ", 4, 0);
									for (int i = 2; str[i]; i++)
									{
										send(it->first, str[i], ft_strlen(str[i]), 0);
										send(it->first, " ", 1, 0);
									}
									send(it->first, "\n", 1, 0);
								}
							}
							for (int i = 0; str[i] ; i++)
							{
								free(str[i]);
								str[i] = NULL;
							}
							free(str);
						}
						if (checkAll(event_fd) == 1 && strncmp(buf, "/kick ", 6) == 0 && users[event_fd]->ope == true)
						{
							char **str = ft_split(buf, ' ');
							std::map<int, Node*>::iterator it = users.begin();
							std::map<int, Node*>::iterator ite = users.end();
							for (; it != ite; it++)
							{
								char *user = &it->second->username[0];
								for (int i = 1; str[i]; i++)
								{
									if (strcmp(user, str[i]) == 0)
										it->second->channel = "";
								}
							}
						}
						if (checkAll(event_fd) == 1 && strncmp(buf, "/send ", 6) && users[event_fd]->channel.length())
						{
							std::map<int, Node*>::iterator it = users.begin();
							std::map<int, Node*>::iterator ite = users.end();
							for (; it != ite; it++)
							{
								if (it->first != event_fd && it->second->channel == users[event_fd]->channel)
								{
									char *test = &users[event_fd]->username[0];
									send(it->first, "[", 1, 0);
									send(it->first, test, ft_strlen(test), 0);
									send(it->first, "] : ", 4, 0);
									send(it->first, buf, ft_strlen(buf), 0);
									send(it->first, "\n", 1, 0);
								}
							}
						}
						if (users[event_fd]->username.length() && users[event_fd]->nickname.length() == 0)
						{
							if (users[event_fd]->nickname.length() == 0)
							{									
								users[event_fd]->nickname = buf;
								std::cout << users[event_fd]->nickname << std::endl;
							}	
						}	
						if (checkPassword[event_fd - 5] == 1 && users[event_fd])
						{
							if (users[event_fd]->username.length() == 0)
							{									
								users[event_fd]->username = buf;
								std::cout << users[event_fd]->username << std::endl;
								send(event_fd, "Enter Nickname : ", 17, 0);
							}	
						}
						if (strcmp(buf, password) == 0 && checkPassword[event_fd - 5] == -1)
						{
							checkPassword[event_fd - 5] = 1;
							send(event_fd, "Good Password\n", 14, 0);
							send(event_fd, "Enter Username : ", 17, 0);
						}
						if (checkAll(event_fd) == 1 && strcmp(buf, "/help") == 0)
						{
							com = (char *)"############# COMMAND LIST ##############\n#\t\"/join\" <name of the channel>   #\n#\t\"/send\" <username>            \t#\n#########################################\n";
							send(event_fd, com, ft_strlen(com), 0);
						}
						if (checkAll(event_fd) == 1 && strncmp(buf, "/join ", 6) == 0)
						{
							char *tab = ft_substr(buf, 6, ft_strlen(buf) - 6);
							std::map<int, Node*>::iterator it = users.begin();
							std::map<int, Node*>::iterator ite = users.end();
							int	i = 0;
							for (; it != ite; it++)
							{
								char *operato = &it->second->channel[0];
								if (strcmp(operato, tab) == 0)
									i++;
							}
							if (i == 0)
								users[event_fd]->ope = true;
							users[event_fd]->channel = tab;
							free(tab);
							tab = NULL;
						}
						std::cout << "read " << bytes_read << " bytes" << "\n";
					}
				}				
			}
			close(socketfd);
			closeAllFd();
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
			std::cout << "\t--New client connect from port no. " << ntohs(sin.sin_port) << "\n";
			send(event_fd, "Please fill the following informations : \n", 41, 0);
			return (d);
		}
		void	closeAllFd()
		{
			std::map<int, Node*>::iterator it = users.begin();
			std::map<int, Node*>::iterator ite = users.end();
			for (; it != ite; it++)
			{
				close(it->first);
			}
		}
		int		checkAll(int fd)
		{
			if (users[fd]->nickname.length() && users[fd]->username.length())
				return 1;
			return 0;
		}
		void	setPassword(char* pass)
		{
			password = pass;
		}
		void	fillPassword()
		{
			for (int i = 0; i < 4 ; i++)
			{
				checkPassword[i] = -1;
			}
		}
		void	fillBool()
		{
			for (int i = 0; i < 4 ; i++)
			{
				check[i] = 0;
			}
		}
		char	*ft_substr(char *s, unsigned int start, size_t len)
		{
			char			*strf;
			int				i;
			unsigned int	lens;

			lens = ft_strlen(s);
			i = 0;
			if (start > lens)
			{
				if (!(strf = new char [1]))
					return (NULL);
				return (strf);
			}
			if (!(strf = new char [len + 1]))
				return (NULL);
			while (s[start] && len != 0)
			{
				if (len == 0)
					return (strf);
				strf[i++] = s[start++];
				len--;
			}
			strf[i] = '\0';
			return (strf);
		}

	private:
		struct Node
		{
			std::string nickname;
			std::string username;
			std::string channel;
			bool		ope;
		};
		int					socketfd;
		char*				password;
		std::map<int, Node*>	users;
		sockaddr_in			sin;
		int 				event_fd;
		struct kevent 		change_event[4], event[4];
		int					checkPassword[4];
		bool				check[4];


};


#endif
