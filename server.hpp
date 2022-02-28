/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmontiel <pmontiel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 11:50:46 by rcheiko           #+#    #+#             */
/*   Updated: 2022/02/28 12:57:21 by rcheiko          ###   ########.fr       */
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
# include <netinet/in.h> // for sockaddr_in
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <stdlib.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <vector>
# include <stdio.h>
# include <ctime>

class server
{

	public:
		server(void): ope_password("987"){
			fillBool();
			fillPassword();
			bzero(res, 1024);

		}
		~server(){}


		int	is_in_chan(char *str)
		{
			std::map<t_channels *, std::vector<int> >::iterator it = canals.begin();
			std::map<t_channels *, std::vector<int> >::iterator ite = canals.end();	
			for (; it != ite; it++)
			{	
				if (strcmp(it->first->name_channels.c_str(), str) == 0)
					return (1);
			}
			return (0);
		}
		int	size_int(int a)
		{
			int res;

			res = 0;
			if (a < 0)
				res++;
			if (a == -0)
				return (1);
			while (a)
			{
				a /= 10;
				res++;
			}
			return (res);
		}

		char	*ft_rev(char *str)
		{
			size_t	i;
			size_t	n;
			char	temp;

			n = ft_strlen(str);
			i = 0;
			while (n > i)
			{
				n--;
				temp = str[n];
				str[n] = str[i];
				str[i] = temp;
				i++;
			}
			return (str);
		}

		char		*ft_itoa(int n)
		{
			char		*re;
			int			i;
			int			signe;
			long int	nb;

			i = 0;
			nb = n;
			re = new char;
			if (!(re))
				return (NULL);
			if ((signe = 0) != nb == 0)
				re[i++] = '0';
			if (nb < 0)
			{
				nb = -nb;
				signe = -1;
			}
			while (nb > 0)
			{
				re[i++] = (nb % 10) + '0';
				nb = nb / 10;
			}
			if (signe == -1)
				re[i++] = '-';
			re[i] = '\0';
			return (ft_rev(re));
		}

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
			{	}
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
					{	}
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
			//envoie de l'erreur ERR_NEEDMOREPARAMS 461 "<commande> :Not enough parameters";
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
		int ft_strlen(const char *str)
		{
			int i = 0;
			if (!str)
				return 0;
			while (str[i])
				i++;
			return (i);
		}
		int ft_strlen_tab(char **str)
		{
			int i = 0;
			while (str[i])
				i++;
			return (i);
		}
		void	free_tab(char **str)
		{
			for(int i = 0; str[i]; i++)
			{
				delete (str[i]);
				str[i] = NULL;
			}
			delete (str);
		}
		int is_in(char *str, char c)
		{
			int i = 0;
			if (str)
			{
				while (str[i])
				{
					if (str[i] == c)
						return (1);
					i++;
				}
			}
			return (0);
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
						node->nbr_channel = 0;
						node->username = "";
						node->nickname = "";
						node->ok = 0;
						users[event_fd] = node;
					}
					if (event[i].flags & EV_EOF)
					{
						std::cout << "Client has disconnected" << std::endl;;
						checkPassword[event_fd - 5] = -3;
						users.erase(event_fd);
						close(event_fd);
					}
					else if (event_fd == socketfd)
					{
						con = init_accept();
						EV_SET(change_event, con, EVFILT_READ, EV_ADD, 0, 0, NULL);
						if (kevent(kq, change_event, 1, NULL, 0, NULL) < 0)
							perror("kevent error");
					}
					else if (event[i].filter & EVFILT_READ)
					{

						char buf[1024];
						char **buf_info = NULL;
						bzero(buf, 1024);
						if (recv(event_fd, buf, 1024, 0) < 0)
							perror("recv error");
						if (strcmp(buf, "") != 0)
						{
							if (buf[ft_strlen(buf) - 1] != '\n')
							{
								if (buf[ft_strlen(buf) - 1] != '\n')
								{
									strcat(res, buf);
									continue;
								}
								else
									ft_strcat(res, buf);
							}
							else
							{	
								ft_strcat(res, buf);
							}
						}
						if (checkPassword[event_fd - 5] == -3)
						{
							buf_info = ft_split(res, '\n');
							for (int i = 0;buf_info[i]; i++)
							{
								std::cout << "TABB-->" << buf_info[i] << std::endl;
								users[event_fd]->init.push_back(buf_info[i]);
							}
							free_tab(buf_info);
							if (users[event_fd]->ok != 4)
							{
								users[event_fd]->ok = 0;
								
								std::vector<std::string>::iterator it = users[event_fd]->init.begin();
								std::vector<std::string>::iterator ite = users[event_fd]->init.end();
								for (; it != ite; it++)
								{
									if (strncmp(it->c_str(), "CAP LS", 6) == 0)
										users[event_fd]->ok++;
									if (strncmp(it->c_str(), "PASS ", 5) == 0)
										users[event_fd]->ok++;
									if (strncmp(it->c_str(), "NICK ", 5) == 0)
										users[event_fd]->ok++;
									if (strncmp(it->c_str(), "USER ", 5) == 0)
										users[event_fd]->ok++;
								}
								if (users[event_fd]->ok != 4)
								{
									it = users[event_fd]->init.begin();
									ite = users[event_fd]->init.end();
									users[event_fd]->init.erase(it, ite);
									std::cout << res << std::endl;
								}						
								if (users[event_fd]->ok == 4)
								{
									bzero(res, 1024);
									checkPassword[event_fd - 5] = -1;
									char *user = NULL;
									char *pass = NULL;
									char *nick = NULL;
									if (checkPassword[event_fd - 5] == -1)
									{
										std::vector<std::string>::iterator save = users[event_fd]->init.begin();
										std::vector<std::string>::iterator save1 = users[event_fd]->init.end();
										for (; save != save1; save++)
										{
											if (strncmp(save->c_str(), "PASS ", 5) == 0)
											{
												pass = ft_substr(save->c_str(), 5, save->length() - 5);
												pass = checkRN(pass);
											}
											if (strncmp(save->c_str(), "NICK ", 5) == 0)
											{
												nick = ft_substr(save->c_str(), 5, save->length() - 5);
												nick = checkRN(nick);
											}
											if (strncmp(save->c_str(), "USER ", 5) == 0)
											{
												user = ft_substr(save->c_str(), 5, save->length() - 5);
												user = checkRN(user);
											}
										}
										pass_error(pass);
										nick_error(nick);
										users[event_fd]->nickname = nick;
										if (user)
										{
											char **user_infos = ft_split(user, ' ');
											if (user_infos && user_infos[0] && user_infos[1] && user_infos[2] && user_infos[3])
												users[event_fd]->username = user_infos[0];
											else
											{
												if (send(event_fd, "461 : Not enough parameters\r\n", 40, 0) < 0)
													perror("send error");
												checkPassword[event_fd - 5] = -3;
											}
											free_tab(user_infos);
										}
									}
									delete user;
									delete pass;
									delete nick;
								}
							}
						}
						welcomeRPL();
						if (checkPassword[event_fd - 5] == 2)
						{
							
							if (ft_strlen(res) == 0)
								continue;
							char *buf2 = NULL;
							buf2 = strdup(res);
							buf2 = checkRN(buf2);
							char **params = ft_split(buf2, ' ');
							kick_command(params);
							list_command(params);
							bot_command(params);
							if (strcmp(params[0], "OPER") == 0)
								ope_command(params); 
							else if (strcmp(params[0], "PRIVMSG") == 0)
								msg_command(params);
							else if (strcmp(params[0], "TOPIC") == 0)
								topic_command(params);
							else if (strncmp("JOIN ", buf2, 5) == 0)
								join_command(buf2, event_fd);
							else if (strcmp(params[0], "PART") == 0)
								part_command(params);
							else if (strcmp(params[0], "MODE") == 0)
							{
								if (strcmp(params[1], users[event_fd]->nickname.c_str()) == 0)
									;
								else
									mode_command(params);
							}
							else if (strcmp(params[0], "NAMES") == 0)
								names_command(params);
							else if (strcmp(params[0], "INVITE") == 0)
								invite_command(params);
							else if (strcmp(params[0], "QUIT") == 0)
							{
								std::cout << "Client has disconnected" << std::endl;;
								checkPassword[event_fd - 5] = -3;
								users.erase(event_fd);
								close(event_fd);
							}
							else if (strcmp(params[0], "NICK") == 0)
								nick_command(params);
							else if (strcmp(params[0], "userhost") == 0)
								user_command(params);
							delete (buf2);
							free_tab(params);
							bzero(res, 1024);
						}
					}
				}
			}
			close(socketfd);
			closeAllFd();
		}
		void	bot_command(char **str)
		{
			if (str && ft_strlen_tab(str) == 3 && strncmp(str[2], ":!BOT", 4) == 0)
			{
				std::string bot = "                       #####                      \n                      ##***##                     \n                      #######                     \n                        ###                       \n                        ###                       \n       #####################################      \n      #######################################     \n      #######################################     \n  ****########,@@@@*(#########,/@@@@,########**** \n  ****######,@@@@@@@@,#######,@@@@@@@@#######**** \n  ****######,@@@@@@@@,#######,@@@@@@@/#######**** \n  ****########,*@@**####**(####,(@&*/########**** \n  ****#################*****#################**** \n  ****#######################################**** \n  ****##########@@*@@,@@@,@@&#@@,@@##########**** \n  ****#########@@@/@@*@@@*@@@%@@*@@@#########**** \n      #######################################     \n      #######################################     \n       #####################################      \r\n";
				if (send(event_fd, bot.c_str(), bot.length(), 0) < 0)
					perror("send error");
			}
		}
		void	nick_command(char **str)
		{
			std::map<int , Node *>::iterator it = users.begin();
			std::map<int , Node *>::iterator ite = users.end();
			if(ft_strlen_tab(str) == 1 || ft_strlen_tab(str) > 2)
			{
				std::string message = ":localhost 431 :No nickname given\r\n";
				send(event_fd, message.c_str(), message.length(), 0);
			}
			else
			{
				for(; it != ite; it++)
				{
					if (strcmp(it->second->nickname.c_str(), str[1]) == 0)
					{
						std::string message = ":localhost 433 " + users[event_fd]->nickname + " " + str[1] + " :Nickname is already in use.\r\n";
						send(event_fd, message.c_str(), message.length(), 0);
						break;
					}
				}
				if (it == ite && strcmp(it->second->nickname.c_str(), str[1]) != 0)
				{
					std::string message = ":" + users[event_fd]->nickname + "@" + users[event_fd]->username + " NICK :" + str[1] + "\r\n";
					send(event_fd, message.c_str(), message.length(), 0);
					users[event_fd]->nickname = str[1];
				}
			}
		}

		void	user_command(char **str)
		{
			if (ft_strlen_tab(str) < 3)
			{
				std::string message = ":localhost 461 " + users[event_fd]->nickname  + " USERHOST :Not enough parameters.\r\n";
				send(event_fd, message.c_str(), message.length(), 0);
			}
			else
			{
				if (ft_strlen_tab(str) >= 5 && strcmp(str[2], "localhost") == 0)
				{
					std::string message = ":localhost 302 " + users[event_fd]->nickname + " :localhost=+localhost@localhost\r\n";
					send(event_fd, message.c_str(), message.length(), 0);
				}
				else
				{
					std::string message = ":localhost 302 " + users[event_fd]->nickname + " :\r\n";
					send(event_fd, message.c_str(), message.length(), 0);
					int i = 1;
					std::string localhost;
					while (str[i])
					{
						localhost = localhost + str[i];
						if (!str[i + 1])
							localhost = localhost + " ";
						i++;
					}
					users[event_fd]->username = localhost;
				}
			}
		}
		char	*checkRN(char *str)
		{
			if (str)
			{
				int i = ft_strlen(str);
				if (str[i - 1] == '\r')
				{
					str[i - 1] = '\0';
				}
				if (str[i - 1] == '\n')
				{
					str[i - 1] = '\0';
				}
				if (str[i - 2] == '\r')
				{
					str[i - 2] = '\0';
				}
				return (str);
			}
			return (NULL);
		}
		int	user_is_in_chan(const char *str, const char *str2)
		{
			std::map<t_channels *, std::vector<int> >::iterator it = canals.begin();
			std::map<t_channels *, std::vector<int> >::iterator ite = canals.end();	
			for (; it != ite; it++)
			{	
				if (strcmp(it->first->name_channels.c_str(), str2) == 0)
				{	
					std::vector<int>::iterator it2 = it->second.begin();
					std::vector<int>::iterator ite2 = it->second.end();
					for(;it2 != ite2; it2++){
						if (strcmp(str, users[*it2]->nickname.c_str()) == 0)
							return (1);
					}
				}
			}
			return (0);
		}
		int user_exist(char *str)
		{
			std::map<int, Node*>::iterator it = users.begin();
			std::map<int, Node*>::iterator ite = users.end();
			for(;it != ite; it++){
				if(strcmp(str, users[it->first]->nickname.c_str()) == 0)
				{	
					return (1);
				}
			}
			return 0;			
		}
		void	invite_command(char **str)
		{
			if (ft_strlen_tab(str) >= 3)
			{
				if(user_exist(str[1]) == 1)// si le gars existe on rentre dans le if
				{
					if(is_in_chan(str[2]) == 1) // si le channel existe alors on rentre dans le if
					{
						if (user_is_in_chan(users[event_fd]->nickname.c_str(), str[2]) == 1) // si l'utilisateur et dans le channel
						{
							std::string invite_message = ":localhost 341 " + users[event_fd]->nickname + " " + str[1] + " :" + str[2] + "\r\n";
							std::map<int, Node*>::iterator it = users.begin();
							std::map<int, Node*>::iterator ite = users.end();
							for(;it != ite; it++){
								if(strcmp(str[1], users[it->first]->nickname.c_str()) == 0)
								{
									std::string channel = str[2];
									users[it->first]->i.push_back(channel);
									send(it->first, invite_message.c_str(), invite_message.length(), 0);
									send(event_fd, invite_message.c_str(), invite_message.length(), 0);
								}
							}
						}
						else
						{
							std::string erreur_notchannel = ":localhost 442 " + users[event_fd]->nickname + " " + str[2] + " :You're not on that channel!\r\n";
							send(event_fd, erreur_notchannel.c_str(), erreur_notchannel.length(), 0);
						}
					}
					else
					{
						std::string erreur_channel = ":localhost 403 " + users[event_fd]->nickname + " " + str[2] + " :No such channel\r\n";
						send(event_fd, erreur_channel.c_str(), erreur_channel.length(), 0);
					}
				}
				else
				{
					std::string erreur_nick = ":localhost 401 " + users[event_fd]->nickname + " " + str[1] + " :No such nick\r\n";
					send(event_fd, erreur_nick.c_str(), erreur_nick.length(), 0);
				}
			}

		}
		void 	moins_o_command(char **str)
		{
			std::map<int, Node*>::iterator save;
			//send commande ope :<nickname>!<username>@localhost MODE #<channel> +o :<nickname>
			std::map<int, Node*>::iterator itt = users.begin();
			std::map<int, Node*>::iterator itte = users.end();
			for(; itt != itte; itt++)
			{
				char *tmp = &itt->second->nickname[0]; 
				if(strcmp(str[3], tmp) == 0)
				{
					save = 	itt;
					break;
				}
			}
			std::string oper = ":";
			std::string a = users[event_fd]->nickname;
			std::string b = users[event_fd]->username;
			std::map<t_channels *, std::vector<int> >::iterator it2 = canals.begin();
			std::map<t_channels *, std::vector<int> >::iterator ite2 = canals.end();
			for(; it2 != ite2; it2++)
			{
				if (strcmp(it2->first->name_channels.c_str(), str[1]) == 0)
				{
					size_t is = is_ope(str[1]);
					if (is)
					{
						std::vector<int>::iterator len = it2->second.begin();
						std::vector<int>::iterator it = it2->second.begin();
						std::vector<int>::iterator ite = it2->second.end();
						oper = oper + a + "!" + b + "localhost " + "MODE " + it2->first->name_channels + " -o " + ":" + users[save->first]->nickname + "\r\n";
						if (it2->first->mode.o == false)
						{
							it2->first->mode.o = true;
							for(; it != ite; it++)
							{
								send(*it, oper.c_str(), oper.length(), 0);
							}
							it2->first->ope.erase(len + (is - 1));
						}
					}						
				}
			}			
		}
		int	is_ope(char *channel)
		{
			size_t i = 1;
			std::map<t_channels *, std::vector<int> >::iterator it2 = canals.begin();
			std::map<t_channels *, std::vector<int> >::iterator ite2 = canals.end();	
			for(; it2 != ite2; it2++)
			{
				if (strcmp(it2->first->name_channels.c_str(), channel) == 0)
				{	
					std::vector<int>::iterator itt = it2->first->ope.begin();
					std::vector<int>::iterator itte = it2->first->ope.end();
					for (; itt != itte; itt++)
					{
						if (*itt == event_fd)
							return (i);
						i++;
					}
				}
			}
			return (0);
		}
		void	plus_o_command(char **str)
		{
			std::map<int, Node*>::iterator save;
			//send commande ope :<nickname>!<username>@localhost MODE #<channel> +o :<nickname>
			std::map<int, Node*>::iterator itt = users.begin();
			std::map<int, Node*>::iterator itte = users.end();
			for(; itt != itte; itt++)
			{
				char *tmp = &itt->second->nickname[0]; 
				if(strcmp(str[3], tmp) == 0)
				{
					save = 	itt;
					break;
				}
			}
			std::string oper = ":";
			std::string a = users[event_fd]->nickname;
			std::string b = users[event_fd]->username;
			std::map<t_channels *, std::vector<int> >::iterator it2 = canals.begin();
			std::map<t_channels *, std::vector<int> >::iterator ite2 = canals.end();
			for(; it2 != ite2; it2++)
			{
				if (strcmp(it2->first->name_channels.c_str(), str[1]) == 0)
				{
					if (is_ope(str[1]))
					{
						std::vector<int>::iterator it = it2->second.begin();
						std::vector<int>::iterator ite = it2->second.end();
						oper = oper + a + "!" + b + "localhost " + "MODE " + it2->first->name_channels + " +o " + ":" + users[save->first]->nickname + "\r\n";
						if (it2->first->mode.o == false)
						{
							it2->first->mode.o = true;
							for(; it != ite; it++)
							{
								send(*it, oper.c_str(), oper.length(), 0);
							}
							it2->first->ope.push_back(save->first);
						}
					}						
				}
			}			
		}
		void	plus_l_command(char **str)
		{	
			std::map<t_channels *, std::vector<int> >::iterator it = canals.begin();
			std::map<t_channels *, std::vector<int> >::iterator ite = canals.end();
			std::string oper = ":";
			std::string a = users[event_fd]->nickname;
			std::string b = users[event_fd]->username;
			for (; it != ite; it++)
			{
				if (strcmp(it->first->name_channels.c_str(), str[1]) == 0)
				{
					if (is_ope(str[1]))
					{
						std::vector<int>::iterator itt = it->second.begin();
						std::vector<int>::iterator itte = it->second.end();
						oper = oper + a + "!" + b + "localhost " + "MODE " + it->first->name_channels + " :" + "+l" + "\r\n";
						if (it->first->mode.l == false)
						{
							it->first->mode.l = true;
							it->first->ch_nbr_max = std::atoi(str[3]);
							for (; itt != itte; itt++)
								send(*itt, oper.c_str(), oper.length(), 0);
						}
					}
				}
			}
		}
		void	moins_l_command(char **str)
		{	
			std::map<t_channels *, std::vector<int> >::iterator it = canals.begin();
			std::map<t_channels *, std::vector<int> >::iterator ite = canals.end();
			std::string oper = ":";
			std::string a = users[event_fd]->nickname;
			std::string b = users[event_fd]->username;
			for (; it != ite; it++)
			{
				if (strcmp(it->first->name_channels.c_str(), str[1]) == 0)
				{
					if (is_ope(str[1]))
					{
						std::vector<int>::iterator itt = it->second.begin();
						std::vector<int>::iterator itte = it->second.end();
						oper = oper + a + "!" + b + "localhost " + "MODE " + it->first->name_channels + " :" + "-l" + "\r\n";
						if (it->first->mode.l == true)
						{
							it->first->ch_nbr_max = 0;
							it->first->mode.l = false;
							for (; itt != itte; itt++)
								send(*itt, oper.c_str(), oper.length(), 0);
						}
					}
				}
			}
		}
		void	plus_p_command(char **str)
		{
			if (is_ope(str[1]))
			{
				std::map<t_channels *, std::vector<int> >::iterator it = canals.begin();
				std::map<t_channels *, std::vector<int> >::iterator ite = canals.end();
				std::string oper = ":";
				std::string a = users[event_fd]->nickname;
				std::string b = users[event_fd]->username;
				for (; it != ite; it++)
				{
					if (strcmp(it->first->name_channels.c_str(), str[1]) == 0)
					{
						std::vector<int>::iterator itt = it->second.begin();
						std::vector<int>::iterator itte = it->second.end();
						oper = oper + a + "!" + b + "localhost " + "MODE " + it->first->name_channels + " :" + "+p" + "\r\n";
						if (it->first->mode.p == false)
						{
							it->first->mode.p = true;
							for (; itt != itte; itt++)
								send(*itt, oper.c_str(), oper.length(), 0);
						}
					}
				}
			}	
		}
		void	moins_p_command(char **str)
		{
			if (is_ope(str[1]))
			{
				std::map<t_channels *, std::vector<int> >::iterator it = canals.begin();
				std::map<t_channels *, std::vector<int> >::iterator ite = canals.end();
				std::string oper = ":";
				std::string a = users[event_fd]->nickname;
				std::string b = users[event_fd]->username;
				for (; it != ite; it++)
				{
					if (strcmp(it->first->name_channels.c_str(), str[1]) == 0)
					{
						std::vector<int>::iterator itt = it->second.begin();
						std::vector<int>::iterator itte = it->second.end();
						oper = oper + a + "!" + b + "localhost " + "MODE " + it->first->name_channels + " :" + "-p" + "\r\n";
						if (it->first->mode.p == true)
						{
							it->first->mode.p = false;
							for (; itt != itte; itt++)
								send(*itt, oper.c_str(), oper.length(), 0);
						}
					}
				}
			}	
		}
		void	plus_s_command(char **str)
		{
			if (is_ope(str[1]))
			{
				std::map<t_channels *, std::vector<int> >::iterator it = canals.begin();
				std::map<t_channels *, std::vector<int> >::iterator ite = canals.end();
				std::string oper = ":";
				std::string a = users[event_fd]->nickname;
				std::string b = users[event_fd]->username;
				for (; it != ite; it++)
				{
					if (strcmp(it->first->name_channels.c_str(), str[1]) == 0)
					{
						std::vector<int>::iterator itt = it->second.begin();
						std::vector<int>::iterator itte = it->second.end();
						oper = oper + a + "!" + b + "localhost " + "MODE " + it->first->name_channels + " :" + "+s" + "\r\n";
						if (it->first->mode.s == false)
						{
							it->first->mode.s = true;
							for (; itt != itte; itt++)
								send(*itt, oper.c_str(), oper.length(), 0);
						}
					}	
				}
			}		
		}
		void	moins_s_command(char **str)
		{
			if (is_ope(str[1]))
			{
				std::map<t_channels *, std::vector<int> >::iterator it = canals.begin();
				std::map<t_channels *, std::vector<int> >::iterator ite = canals.end();
				std::string oper = ":";
				std::string a = users[event_fd]->nickname;
				std::string b = users[event_fd]->username;
				for (; it != ite; it++)
				{
					if (strcmp(it->first->name_channels.c_str(), str[1]) == 0)
					{
						std::vector<int>::iterator itt = it->second.begin();
						std::vector<int>::iterator itte = it->second.end();
						oper = oper + a + "!" + b + "localhost " + "MODE " + it->first->name_channels + " :" + "-s" + "\r\n";
						if (it->first->mode.s == true)
						{
							it->first->mode.s = false;
							for (; itt != itte; itt++)
								send(*itt, oper.c_str(), oper.length(), 0);
						}
					}
				}
			}			
		}
		void	moins_i_command(char **str)
		{
			if (is_ope(str[1]))
			{
				std::map<t_channels *, std::vector<int> >::iterator it = canals.begin();
				std::map<t_channels *, std::vector<int> >::iterator ite = canals.end();
				std::string oper = ":";
				std::string a = users[event_fd]->nickname;
				std::string b = users[event_fd]->username;
				for (; it != ite; it++)
				{
					if (strcmp(it->first->name_channels.c_str(), str[1]) == 0)
					{
						std::vector<int>::iterator itt = it->second.begin();
						std::vector<int>::iterator itte = it->second.end();
						oper = oper + a + "!" + b + "localhost " + "MODE " + it->first->name_channels + " :" + "-i" + "\r\n";
						if (it->first->mode.i == true)
						{
							it->first->mode.i = false;
							for (; itt != itte; itt++)
								send(*itt, oper.c_str(), oper.length(), 0);
						}
					}
				}	
			}
		}
		void	plus_i_command(char **str)
		{
			if (is_ope(str[1]))
			{
				std::map<t_channels *, std::vector<int> >::iterator it = canals.begin();
				std::map<t_channels *, std::vector<int> >::iterator ite = canals.end();
				std::string oper = ":";
				std::string a = users[event_fd]->nickname;
				std::string b = users[event_fd]->username;
				for (; it != ite; it++)
				{
					if (strcmp(it->first->name_channels.c_str(), str[1]) == 0)
					{
						std::vector<int>::iterator itt = it->second.begin();
						std::vector<int>::iterator itte = it->second.end();
						oper = oper + a + "!" + b + "localhost " + "MODE " + it->first->name_channels + " :" + "+i" + "\r\n";
						if (it->first->mode.i == false)
						{
							it->first->mode.i = true;
							for (; itt != itte; itt++)
								send(*itt, oper.c_str(), oper.length(), 0);
						}
					}
				}	
			}
		}
		void mode_command(char **str)
		{
			if (ft_strlen_tab(str) > 2)
			{
				if (is_in_chan(str[1]))
				{
					if (str[2][0] == '+')
					{
						if (is_in(str[2], 'o'))
							plus_o_command(str);	
						if (is_in(str[2], 'l'))
							plus_l_command(str);
						if (is_in(str[2], 's'))
							plus_s_command(str);
						if (is_in(str[2], 'p'))
							plus_p_command(str);
						if (is_in(str[2], 'i'))
							plus_i_command(str);
					}
					else if (str[2][0] == '-')
					{
						if (is_in(str[2], 'o'))
							moins_o_command(str);		
						if (is_in(str[2], 'l'))
							moins_l_command(str);
						if (is_in(str[2], 's'))
							moins_s_command(str);
						if (is_in(str[2], 'p'))
							moins_p_command(str);
						if (is_in(str[2], 'i'))
							moins_i_command(str);
					}
				}
				else
				{
					char *welcome = NULL;
					std::string a;
					std::string c = "403";
					std::string d = " :No such channel";
					a = c + d + "\r\n";
					welcome = &a[0];
					send(event_fd, welcome, ft_strlen(welcome), 0);
				}
			}	
		}		
		void names_command(char **str)
		{
			char **channel_split = ft_split(str[1] , ',');
			int i = 0;
			std::string message, messagee;
			while (channel_split[i])
			{
				std::map<t_channels*, std::vector<int> >::iterator it = canals.begin();
				std::map<t_channels*, std::vector<int> >::iterator ite = canals.end();
				for(; it != ite; it++)
				{
					if (strcmp(channel_split[i], it->first->name_channels.c_str()) == 0)
					{
						std::vector<int>::iterator it2 = it->second.begin();
						std::vector<int>::iterator ite2 = it->second.end();
						message = ":localhost 353 " + users[event_fd]->nickname + " = " + it->first->name_channels + " :";
						for(;it2 != ite2; it2++){
							message = message + users[*it2]->nickname + " ";
						}
						message = message + "\r\n";
						send(event_fd, message.c_str(), message.length(), 0);
						messagee = ":localhost 366 " + users[event_fd]->nickname + " " + it->first->name_channels + " " + ":End of /NAMES list\r\n";
						send(event_fd, messagee.c_str(), messagee.length(), 0);
					}
				}
				i++;
			}
			free_tab(channel_split);
		}
		void	list_command(char **str)
		{
			if (strcmp(str[0], "LIST") == 0)
			{
				std::string first_list = ":localhost 321 " + users[event_fd]->nickname + " Channel :Users Name" + "\r\n";
				send(event_fd, first_list.c_str(), first_list.length(), 0);
				std::map<t_channels *, std::vector<int> >::iterator it = canals.begin();
				std::map<t_channels *, std::vector<int> >::iterator ite = canals.end();
				for(; it != ite; it++)
				{
					if (strcmp(it->first->name_channels.c_str(), str[1]) == 0)
					{

						if (user_is_in_chan(users[event_fd]->nickname.c_str(), it->first->name_channels.c_str()) || (it->first->mode.p == false && it->first->mode.s == false))
						{
							char	*ito = ft_itoa(it->first->number_of_members);
							char *topic = &it->first->topic[0];
							char *yo = ft_substr(topic, 1, ft_strlen(topic));
							std::string second_list = ":localhost 322 " + users[event_fd]->nickname + " " + str[1] + " " + ito + " :" + yo + "\r\n";
							send(event_fd, second_list.c_str(), second_list.length(), 0);
							delete(ito);
							delete yo;
						}
					}
				}
				std::string third_list = ":localhost 322 " + users[event_fd]->nickname + " :End of channel list." + "\r\n";
				send(event_fd, third_list.c_str(), third_list.length(), 0);
			}
		}
		int is_operateur(const char *str)
		{
			std::map<t_channels*, std::vector<int> >::iterator it = canals.begin();
			std::map<t_channels*, std::vector<int> >::iterator ite = canals.end();
			for(; it != ite; it++)
			{
				if (strcmp(it->first->name_channels.c_str(), str) == 0)
				{
					std::vector<int>::iterator it2 = it->first->ope.begin();
					std::vector<int>::iterator ite2 = it->first->ope.end();
					for(; it2 != ite2; it2++)
					{
						if (event_fd == *it2)
							return 1;
					}
				}
			}
			return 0;		
		}
		void topic_command(char **str)
		{
			std::cout << "JE SUIS REMTRER ICI\n";
			if (ft_strlen_tab(str) >= 2)
			{
				if (ft_strlen_tab(str) == 2)
				{
					std::map<t_channels*, std::vector<int> >::iterator it = canals.begin();
					std::map<t_channels*, std::vector<int> >::iterator ite = canals.end();
					for(;it != ite; it++)
					{
						if (strcmp(it->first->name_channels.c_str(), str[1]) == 0)
						{
							std::string error_topic = ":localhost 332 ";
							std::string	name = users[event_fd]->nickname;
							std::string c = error_topic + name + " " + str[1] + " " + it->first->topic +"\r\n";
							send(event_fd, c.c_str(), c.length(), 0);
							break ;
						}
					}
					if(it == ite)
					{
						std::string error_topic = ":localhost 403 ";
						std::string	name = users[event_fd]->nickname;
						std::string c = error_topic + name + " " + str[1] + " :No such channel\r\n";
						send(event_fd, c.c_str(), c.length(), 0);
					}
				}
				else
				{
					std::map<t_channels*, std::vector<int> >::iterator it = canals.begin();
					std::map<t_channels*, std::vector<int> >::iterator ite = canals.end();
					for(;it != ite; it++)
					{
						std::cout << "name of chan -->" << it->first->name_channels << std::endl;
						if (strcmp(it->first->name_channels.c_str(), str[1]) == 0)
						{
							
							if(is_operateur(it->first->name_channels.c_str()) == 1)
							{
								
								int i = 2;
								std::string new_topic;
								for (;str[i] ;i++)
								{
									if(str[i + 1])
										new_topic = new_topic + str[i] + " ";
									else
										new_topic = new_topic + str[i];
								}
								it->first->topic = new_topic;
								std::string c = ":" + users[event_fd]->nickname + "!" + users[event_fd]->username +" TOPIC " + str[1] + " " + new_topic + "\r\n";
								std::vector<int>::iterator it2 = it->second.begin();
								std::vector<int>::iterator ite2 = it->second.end();
								for(; it2 != ite2; it2++)
								{
									send(*it2, c.c_str(), c.length(), 0);
								}
							}
						}
					}
					if(it == ite)
					{
						std::string error_topic = ":localhost 403 ";
						std::string	name = users[event_fd]->nickname;
						std::string c = error_topic + name + " " + str[1] + " :No such channel\r\n";
						send(event_fd, c.c_str(), c.length(), 0);
					}
				}
			}
			else
			{
				std::string error_topic = ":localhost 461 ";
				std::string	name = users[event_fd]->nickname;
				std::string c = error_topic + name + " TOPIC Not enough parameters.\r\n";
				send(event_fd, c.c_str(), c.length(), 0);
			}
		}
		void	kick_command(char **str)
		{
			int checkChannel = 0;
			int checkOpe = 0;
			if (!str)
				return ;
			if (str && strcmp(str[0], "KICK") == 0)
			{
				if (ft_strlen_tab(str) < 2)
				{
					std::string no_params = "461 KICK :Not enough parameters\r\n";
					if (send(event_fd, no_params.c_str(), no_params.length(), 0) < 0)
						perror("send error");
					return ;
				}
				std::map<t_channels *, std::vector<int> >::iterator it6 = canals.begin();
				std::map<t_channels *, std::vector<int> >::iterator ite6 = canals.end();
				for(; it6 != ite6; it6++)
				{
					if (strcmp(it6->first->name_channels.c_str(), str[1]) == 0)
					{
						std::vector<int>::iterator ite8 = it6->first->ope.end();
						std::vector<int>::iterator it8 = it6->first->ope.begin();
						checkChannel = 1;
						for(; it8 != ite8 ; it8++)
						{
							if(*it8 == event_fd)
							{
								checkOpe = 1;
								std::map<int, Node* >::iterator save;
								std::map<int, Node* >::iterator it3 = users.begin();
								std::map<int, Node* >::iterator ite3 = users.end();
								for(;it3 != ite3; it3++)
								{
									if (strcmp(it3->second->nickname.c_str(), str[2]) == 0)
									{
										save = it3;
										break;
									}
								}
								std::map<t_channels*, std::vector<int> >::iterator it = canals.begin();
								std::map<t_channels*, std::vector<int> >::iterator ite = canals.end();
								for(;it != ite; it++)
								{
									if (strcmp(it->first->name_channels.c_str(), str[1]) == 0)
									{
										std::vector<int>::iterator it2 = it->second.begin();
										std::vector<int>::iterator ite2 = it->second.end();
										for(;it2 != ite2; it2++)
										{
											std::string kick_send = ":" + users[event_fd]->nickname + "!" + users[event_fd]->username + "@localhost KICK " + str[1] + " " + str[2] + " :";
											for (int i = 2; str[i] ;i++)
											{
												if (str[i] != NULL && str[i + 1] != NULL)
													kick_send = kick_send + str[i] + " ";
												else
													kick_send = kick_send + str[i];
											}
											kick_send = kick_send + "\r\n";
											send(*it2, kick_send.c_str(), kick_send.length(), 0);
											if (*it2 == save->first)
												it->second.erase(it2);
										}
									}
								}			
							}
							if (checkOpe != 1)
							{
								std::string erreur_ope = "482 ";
								erreur_ope += str[1];
								erreur_ope += " :You're not channel operator\r\n";
								if (send(event_fd, erreur_ope.c_str(), erreur_ope.length(), 0) < 0)
									perror("send error");
								return ;				
							}
						}
					}
				}
				if (checkChannel != 1)
				{
					std::string erreur_notchannel = "442 ";
					erreur_notchannel += str[1];
					erreur_notchannel += " :You're not on that channel!\r\n";
					if (send(event_fd, erreur_notchannel.c_str(), erreur_notchannel.length(), 0) < 0)
						perror("send error");
					return ;
				}
			}
		}
		void	part_command(char **str)
		{
			if (ft_strlen_tab(str) <= 1)
			{
				send(event_fd, "461 PART : Not enough parameters\r\n", 35, 0);
				return;
			}
			int	checkChannel = 0;
			std::map<t_channels*, std::vector<int> >::iterator it = canals.begin();
			std::map<t_channels*, std::vector<int> >::iterator ite = canals.end();
			for(;it != ite; it++)
			{
				if (ft_strlen_tab(str) >= 2 && strcmp(it->first->name_channels.c_str(), str[1]) == 0)
				{
					checkChannel = 1;
					std::vector<int>::iterator it2 = it->second.begin();
					std::vector<int>::iterator ite2 = it->second.end();
					for (; it2 != ite2; it2++)
					{
						char *welcome = NULL;
						std::string a;
						std::string c = users[event_fd]->nickname;
						std::string d = users[event_fd]->username;
						a = ":" + c + "!" + d + "@localhost " + "PART " + str[1];
						if (str[2])
						{
							int i = 2;
							a = a + " ";
							while (str[i])
							{
								a = a + str[i] + " ";
								i++;
							}
						}
						a = a + "\r\n";
						welcome = &a[0];
						if (send(*it2, welcome, ft_strlen(welcome), 0) < 0)
							perror("send error");
					}
					if (it->second.size() == 1)
					{
						delete it->first;
						canals.erase(it);
						break;
					}
					else if (it->second.size() > 1)
					{
						it2 = it->second.begin();
						ite2 = it->second.end();
						for(; it2 != ite2; it2++)
						{
							if(*it2 == event_fd)
							{
								it->second.erase(it2);
								it->first->number_of_members--;
							}
						}
					}
				}
			}
			if (checkChannel != 1)
			{
				std::string erreur_notchannel = "442 ";
				erreur_notchannel += str[1];
				erreur_notchannel += " :You're not on that channel!\r\n";
				if (send(event_fd, erreur_notchannel.c_str(), erreur_notchannel.length(), 0) < 0)
					perror("send error");
				return ;
			}
		}
		void msg_command(char **str)
		{
			if (str[1][0] != '#')
			{
				std::map<int, Node*>::iterator it = users.begin();
				std::map<int, Node*>::iterator ite = users.end();
				for (; it != ite; it++)
				{
					char *user = &it->second->nickname[0];
					if (strcmp(user, str[1]) == 0)
					{
						char *welcome = NULL;
						std::string a;
						std::string c = users[event_fd]->nickname;
						std::string d = users[event_fd]->username;
						a = c + "!" + d + "@localhost " + "PRIVMSG " + it->second->nickname + " ";
						for (int i = 2; str[i]; i++)
						{
							if (str[i] != NULL && str[i + 1] != NULL)
								a = a + str[i] + " ";
							else
								a = a + str[i];
						}
						a = a + "\r\n";
						welcome = &a[0];
						if (send(it->first, welcome, ft_strlen(welcome), 0) < 0)
							perror("send error");
					}
				}
			}
			else
			{
				std::map<t_channels*, std::vector<int> >::iterator it = canals.begin();
				std::map<t_channels*, std::vector<int> >::iterator ite = canals.end();
				for (; it != ite; it++)
				{

					if (strcmp(it->first->name_channels.c_str(), str[1]) == 0)
					{
						std::vector<int>::iterator it2 = it->second.begin();
						std::vector<int>::iterator ite2 = it->second.end();
						for(; it2 != ite2; it2++)
						{
							char *welcome = NULL;
							std::string a;
							std::string c = users[event_fd]->nickname;
							std::string d = users[event_fd]->username;
							a = ":" + c + "!" + d + "@localhost " + "PRIVMSG " + str[1] + " ";
							for (int i = 2; str[i]; i++)
							{
								if (str[i] != NULL && str[i + 1] != NULL)
									a = a + str[i] + " ";
								else
									a = a + str[i];
							}
							a = a + "\r\n";
							welcome = &a[0];
							if(*it2 != event_fd)
							{
								if (send(*it2, welcome, ft_strlen(welcome), 0) < 0)
									perror("send error");
							}
						}
					}

				}			
			}
		}
		void	ope_command(char **str)
		{
			char* tmp = &ope_password[0];
			char *str2 = ft_substr(str[2], 0, ft_strlen(str[2]) - 2);
			if (str && ft_strlen_tab(str) == 3)
			{
				if (strcmp(tmp, str2) == 0)
				{
					if(users[event_fd]->ope == false)
					{
						users[event_fd]->ope = true;
						if (send(event_fd, "381 : You are now an IRC operator\r\n", 50, 0) < 0)
							perror("send error");
					}
				}
				else
					if (send(event_fd, "464 : Password incorrect\r\n", 40, 0) < 0)
						perror("send error");
			}
			else
				if (send(event_fd, "461 :Not enough parameters\r\n", 40, 0) < 0)
					perror("send error");
		delete (str2);
		}
		char *ft_strcat(char *dest, char *src)
		{
			int i;
			int j;

			i = 0;
			while (dest[i] != '\0')
				i++;
			j = 0;
			while (src[j] != '\0')
			{
				dest[i + j] = src[j];
				j++;
			}
			return (dest);
		}
		void	welcomeRPL()
		{
			if (checkPassword[event_fd - 5] != -1 && checkPassword[event_fd - 5] != -2 && checkPassword[event_fd - 5] != 2 && checkPassword[event_fd - 5] != -3)
			{
				char *welcome = NULL;
				std::string a = "001 ";
				std::string b = " :Welcome to the Internet Relay Network ";
				std::string c = users[event_fd]->nickname;
				std::string d = users[event_fd]->username;
				a = a + users[event_fd]->nickname + b + c + "!" + d + "@localhost\r\n";
				welcome = &a[0];
				checkPassword[event_fd - 5] = 2;
				if (send(event_fd, welcome, ft_strlen(welcome), 0) < 0)
					perror("send error");
			}		
		}
		void	user_error(char *str)
		{
			std::map<int, Node*>::iterator it = users.begin();
			std::map<int, Node*>::iterator ite = users.end();
			for (; it != ite; it++)
			{
				char *user = &it->second->username[0];
				if (strcmp(user, str) == 0)
				{
					if (send(event_fd, "462 : Unauthorized command (already registered)\r\n", 45, 0) < 0)
						perror("send error");
					checkPassword[event_fd - 5] = -2;
				}
			}
		}
		void	pass_error(char *str)
		{
			char no_param[] = "432 : Not enough parameters\r\n";
			if (!ft_strlen(str))
				if (send(event_fd, no_param, ft_strlen(no_param), 0) < 0)
					perror("send error");
			if (strcmp(password, str) == 0 && checkPassword[event_fd - 5] != -2)
			{
				std::cout << " PASS = " << ft_strlen(password) << "\n";
				std::cout << " STR = " << ft_strlen(str) << "\n";
				checkPassword[event_fd - 5] = 1;
			}
			else
			{
				char falsePass[] = "433 : Password incorrect\r\n";
				char falsePass2[] = "Password incorrect\r\n";
				if (send(event_fd, falsePass, ft_strlen(falsePass), 0) < 0)
					perror("send error");
				if (send(event_fd, falsePass2, ft_strlen(falsePass2), 0) < 0)
					perror("send error");
			}
		}
		void	nick_error(char *str)
		{
			std::map<int, Node*>::iterator it = users.begin();
			std::map<int, Node*>::iterator ite = users.end();
			for (; it != ite; it++)
			{
				char *user = &it->second->nickname[0];
				if (strcmp(user, str) == 0)
				{
					std::string error_nickname = "433 " + users[event_fd]->nickname + " : Nickname is already in use.\r\n";
					char error_msg[] = "Nickname is already in use\r\n";
					if (send(event_fd, error_nickname.c_str(), error_nickname.length(), 0) < 0)
						perror("send error");
					if (send(event_fd, error_msg, ft_strlen(error_msg), 0) < 0)
						perror("send error");
					checkPassword[event_fd - 5] = -2;
				}
			}
			char no_nickname[] = "432 : No nickname given\r\n";
			if (!ft_strlen(str))
				if (send(event_fd, no_nickname, ft_strlen(no_nickname), 0) < 0)
					perror("send error");
			char error_nick[] = "433 : Erroneous nickname\r\n";
			char error_nick2[] = "Erroneous nickname\r\n";
			if (ft_strlen(str) > 9)
			{
				if (send(event_fd, error_nick, ft_strlen(error_nick), 0) < 0) 
					perror("send error");
				if (send(event_fd, error_nick2, ft_strlen(error_nick2), 0) < 0)
					perror("send error");
				checkPassword[event_fd - 5] = -2;
			}
		}
		int	init_accept()
		{
			int d;
			int l;
			sockaddr_in	c;
			if ((d = accept(event_fd, (sockaddr *)&c, (socklen_t *)&l)) == -1)
			{
				std::cout << "\t--Accept error\n";
				exit(EXIT_FAILURE);  
			}
			std::cout << "\t--New client connect from port no. " << ntohs(sin.sin_port) << "\n";
			std::cout << "\n";
			std::cout << "\t##### PLEASE ENTER THESE FOLLOWING COMMANDS : #####" << "\n";
			std::cout << "\n";
			std::cout << "\tCAP LS" << "\n";
			std::cout << "\tPASS --> <password>" << "\n";
			std::cout << "\tNICK --> <nickname> " << "\n";
			std::cout << "\tUSER --> <username> <username> <localhost> :<realname> " << "\n";
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
			for (int i = 0; i < 255 ; i++)
			{
				checkPassword[i] = -3;
			}
		}
		void	fillBool()
		{
			for (int i = 0; i < 255 ; i++)
			{
				check[i] = 0;
			}
		}
		char	*ft_substr(const char *s, unsigned int start, size_t len)
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

		int good_name_channel(char *str)
		{
			int i = 0;
			if (ft_strlen(str) > 50)
				return 0;
			if (str)
			{
				if (str[0] == '#' || str[0] == '&' || str[0] == '!' || str[0] == '+')
				{
					i++;
					while (str[i])
					{
						if (str[i] == ' ')
							return 0;
						i++;
					}
					return 1;
				}
				return 0;
			}
			return 0;
		}

		int		channel_already_exist(char *str)
		{
			std::map<t_channels*, std::vector<int > >::iterator it = canals.begin();
			std::map<t_channels*, std::vector<int > >::iterator ite = canals.end();
			for (; it != ite; it++)
			{
				if (strcmp(it->first->name_channels.c_str(), str) == 0)
					return 0;
			}
			return 1;
		}


		int good_password_channel(char *str, char *channel_name)
		{
			std::map<t_channels*, std::vector<int > >::iterator it = canals.begin();
			std::map<t_channels*, std::vector<int > >::iterator ite = canals.end();
			for (; it != ite; it++)
			{
				if (strcmp(it->first->name_channels.c_str(), channel_name) == 0)
				{
					if (strcmp(it->first->password_channel.c_str(), str) == 0)
						return 0;
				}
			}
			return 1;
		}
		int	add_operator(int user, char *channel_split)
		{
			std::string oper = ":";
			oper = oper + "localhost 353 " + users[user]->nickname + " = " + channel_split + " :@" + users[user]->nickname + "\r\n";
			std::map<t_channels *, std::vector<int> >::iterator it2 = canals.begin();
			std::map<t_channels *, std::vector<int> >::iterator ite2 = canals.end();
			for(; it2 != ite2; it2++)
			{
				if (strcmp(it2->first->name_channels.c_str(), channel_split) == 0)
					return (0);
			}
			send(user, oper.c_str(), oper.length(), 0);
			return (1);
		}
		int	join_command(char *buffer, int user)
		{
			struct timeval time_now;
    		gettimeofday(&time_now, nullptr);
			char **buffer_split = ft_split(buffer, ' ');
			t_channels *canaux;
			if (ft_strlen_tab(buffer_split) > 1 && ft_strlen_tab(buffer_split) < 4)
			{
				if (users[user]->nbr_channel < 3)
				{
					char **channel_split = ft_split(buffer_split[1], ',');
					int i = 0;
					while (channel_split[i])
					{
						if(good_name_channel(channel_split[i]) == 1) // si le nom du channel est juste
						{
							if (add_operator(user, channel_split[i]) == 1)
							{
								canaux = new t_channels;
								canaux->number_of_members = 1;
								canaux->topic = "";
								canaux->name_channels = channel_split[i];
								if(buffer_split[2])
								{
									char **password_buffer = ft_split(buffer_split[2], ',');
									canaux->password_channel = password_buffer[i];
									free_tab(password_buffer);
								}
								canaux->ope.push_back(user);
								canals[canaux].push_back(user);
							}
							users[user]->nbr_channel++;
							std::map<t_channels *, std::vector<int> >::iterator ip = canals.begin();
							std::map<t_channels *, std::vector<int> >::iterator ipe = canals.end();
							for (; ip != ipe; ip++)
							{
								if (strcmp(ip->first->name_channels.c_str(), channel_split[i]) == 0)
								{
									int erreur = 0;
									std::map<int, Node*>::iterator save = users.find(user);
									std::vector<std::string>::iterator vec = save->second->i.begin();
									std::vector<std::string>::iterator vec1 = save->second->i.end();
									for (; vec != vec1; vec++)
									{
										std::string vec2 = *vec;
										if (strcmp(vec2.c_str(), channel_split[i]) == 0)
											erreur = 1;
									}
									if (ip->first->mode.i && erreur == 0)
									{
										std::string i;
										std::string a = ":localhost ";
										std::string c = "473 ";
										std::string d = " :Cannot join channel (invite only)";
										i = a + c + users[event_fd]->username + " " + ip->first->name_channels + " " + d + "\r\n";
										send(event_fd, i.c_str(), i.length(), 0);	
										return (0);	
									}
								}
							}
							std::map<t_channels *, std::vector<int> >::iterator it0 = canals.begin();
							std::map<t_channels *, std::vector<int> >::iterator ite0 = canals.end();
							for (; it0 != ite0; it0++)
							{
								if (strcmp(it0->first->name_channels.c_str(), channel_split[i]) == 0)
								{
									if (it0->first->mode.l)
									{
										if (it0->first->number_of_members >= it0->first->ch_nbr_max)
										{
											std::string i;
											std::string a = ":localhost ";
											std::string c = "471 ";
											std::string d = " :Cannot join channel (channel is full)";
											i = a + c + users[event_fd]->username + " " + it0->first->name_channels + " " + d + "\r\n";
											send(event_fd, i.c_str(), i.length(), 0);	
											return (0);	
										}
									}
								}
							}
							std::map<t_channels *, std::vector<int> >::iterator it5 = canals.begin();
							std::map<t_channels *, std::vector<int> >::iterator ite5 = canals.end();
							for(; it5 != ite5; it5++)
							{
								if (strcmp(it5->first->name_channels.c_str(), channel_split[i]) == 0)
								{
									std::vector<int>::iterator it0 = it5->first->ope.begin();
									std::vector<int>::iterator ite0 = it5->first->ope.end();
									for(; it0 != ite0; it0++)
									{
										if (*it0 != user)
										{
											it5->first->number_of_members++;
											it5->second.push_back(user);
										}
									}
								}
							}
							std::map<t_channels *, std::vector<int> >::iterator it2 = canals.begin();
							std::map<t_channels *, std::vector<int> >::iterator ite2 = canals.end();
							for(; it2 != ite2; it2++)
							{
								if (strcmp(it2->first->name_channels.c_str(), channel_split[i]) == 0)
								{
									std::vector<int>::iterator it3 = it2->second.begin();
									std::vector<int>::iterator ite3 = it2->second.end();
									for(; it3 != ite3; it3++)
									{
										char *welcome = NULL;
										std::string a;
										std::string c = users[user]->nickname;
										std::string d = users[user]->username;
										a = ":" + c + "!" + d + "@localhost JOIN " + it2->first->name_channels +"\r\n";
										welcome = &a[0];
										send(*it3, welcome, ft_strlen(welcome), 0);
									}
								}
							}
							std::map<t_channels *, std::vector<int> >::iterator it9 = canals.begin();
							std::map<t_channels *, std::vector<int> >::iterator ite9 = canals.end();
							for(; it9 != ite9; it9++)
							{
								if (strcmp(it9->first->name_channels.c_str(), channel_split[i]) == 0)
								{
									if (it9->first->topic != "")
									{
										std::string a = ":localhost 332 ";
										std::string b = users[user]->nickname;
										a = a + b + " " + it9->first->name_channels + " " + ":" + it9->first->topic +"\r\n";
										if (send(event_fd, a.c_str(), a.length(), 0) < 0)
											perror("send error");
									}
								}
							}
							std::map<t_channels *, std::vector<int> >::iterator it6 = canals.begin();
							std::map<t_channels *, std::vector<int> >::iterator ite6 = canals.end();
							for(; it6 != ite6; it6++)
							{
								if (strcmp(it6->first->name_channels.c_str(), channel_split[i]) == 0)
								{
									char *welcomee = NULL;
									std::string c = ":localhost 353 ";
									std::string d = users[user]->nickname;
									c = c + d + " = " + it6->first->name_channels + " :";
									std::vector<int>::iterator it7 = it6->second.begin();
									std::vector<int>::iterator ite7 = it6->second.end();

									std::vector<int>::iterator it8 = it6->first->ope.begin();
									std::vector<int>::iterator ite8 = it6->first->ope.end();
									for (; it7 != ite7 ; it7++)
									{
										for(; it8 != ite8 ; it8++)
										{
											if(*it8 == *it7)
												c = c + " @";
										}
										c = c + users[*it7]->nickname + " ";
									}
									c = c + "\r\n";
									welcomee = &c[0];
									if (send(user, welcomee, ft_strlen(welcomee), 0) < 0)
										perror("send error");
								}
							}
							std::map<t_channels *, std::vector<int> >::iterator it11 = canals.begin();
							std::map<t_channels *, std::vector<int> >::iterator ite11 = canals.end();
							for(;it11 != ite11 ; it11++){
								if (strcmp(it11->first->name_channels.c_str(), channel_split[i]) == 0)
								{
									char *welcomeee = NULL;
									std::string e = ":localhost 366 ";
									std::string f = users[user]->nickname;
									e = e + f + " " + it11->first->name_channels + " " + ":End of NAMES list\r\n";
									welcomeee = &e[0];
									if (send(user, welcomeee, ft_strlen(welcomeee), 0) < 0)
										perror("send error");
								}
							}

							std::map<t_channels *, std::vector<int> >::iterator it10 = canals.begin();
							std::map<t_channels *, std::vector<int> >::iterator ite10 = canals.end();
							for(; it10 != ite10; it10++)
							{
								if (strcmp(it10->first->name_channels.c_str(), channel_split[i]) == 0)
								{
									std::string a = ":localhost 329 ";
									std::string b = users[user]->nickname;
									char *time = ft_itoa(time_now.tv_sec);
									a = a + b + " " + it10->first->name_channels + " " + ":" + time +"\r\n";
									delete (time);
									if (send(event_fd, a.c_str(), a.length(), 0) < 0)
										perror("send error");
								}
							}
							i++;
						}
						else
						{
							char *welcome = NULL;
							std::string a;
							std::string c = "403";
							std::string d = " :No such channel";
							a = c + d + "\r\n";
							welcome = &a[0];
							send(user, welcome, ft_strlen(welcome), 0);
						}
					}
					free_tab(channel_split);
				}
				else
				{
					char *welcome = NULL;
					std::string a;
					std::string c = "405";
					std::string d = " :You have joined too many channels";
					a = c + d + "\r\n";
					welcome = &a[0];
					send(user, welcome, ft_strlen(welcome), 0);
				}
				
			}
			else
			{
				char *welcome = NULL;
				std::string a;
				std::string c = "461";
				std::string d = " :Not enough parameters";
				a = c + d + "\r\n";
				welcome = &a[0];
				send(user, welcome, ft_strlen(welcome), 0);
			}
			free_tab(buffer_split);
			return (0);
		}

	private:
		struct Node
		{
			std::string 				nickname;
			std::string 				username;
			std::vector<std::string> 	i;
			std::vector<std::string>	init;
			unsigned int 				nbr_channel;
			bool						ope;
			int							ok;
		};
		typedef struct	s_mode{
			bool o;
			bool p;
			bool s;
			bool i;
			bool t;
			bool n;
			bool m;
			bool l;
			bool b;
			bool v;
			bool k;
		}				t_mode;
		typedef struct	s_channels{
			t_mode  mode;
			std::string name_channels;
			std::string password_channel;
			int number_of_members;
			static unsigned int actif_members;
			std::vector<int>	ope;
			bool	fill;
			int	ch_nbr_max;
			std::string topic;
		}		t_channels;
		int					socketfd;
		char*				password;
		std::map<int, Node*>	users;
		std::vector<int>		fd_channels;
		std::map<t_channels*, std::vector<int> >	canals;
		sockaddr_in			sin;
		int 				event_fd;
		struct kevent 		change_event[256], event[256];
		int					checkPassword[256];
		bool				check[256];
		std::string			ope_password;
		char res[1024];
};


#endif
