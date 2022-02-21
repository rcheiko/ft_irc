/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmontiel <pmontiel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 11:50:46 by rcheiko           #+#    #+#             */
/*   Updated: 2022/02/21 14:53:59 by whamoumi         ###   ########.fr       */
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
# include <arpa/inet.h>
# include <fcntl.h>
# include <vector>


class server
{

	public:
		server(void): ope_password("987"){
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
		int ft_strlen(char *str)
		{
			int i = 0;
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
				free(str[i]);
				str[i] = NULL;
			}
			free(str);
		}
		int is_in(char *str)
		{
			int i = 0;
			if (str)
			{
				while (str[i])
				{
					if (str[i] == ',')
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
					//std::cout << "FD = " << event_fd << "\n";
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
						{
							perror("kevent error");
						}
					}
					else if (event[i].filter & EVFILT_READ)
					{
						char buf[1024];
						bzero(buf, 1024);
						checkConnection();
						welcomeRPL();
						bzero(buf, 1024);
						if (recv(event_fd, buf, 1024, 0) < 0)
							perror("recv error");
						//std::cout << "USERS : " << users[event_fd]->nickname << std::endl;
						//std::cout << "USERS LENGTH : " << users[event_fd]->nickname.length() << std::endl;
						std::cout << "BUFFER : " << buf << std::endl;
						char **params = ft_split(ft_substr(buf, 0, ft_strlen(buf) - 2), ' ');
						if (strcmp(params[0], "OPER") == 0)
						{
							ope_command(params); //ajouter la dimension channel de wanis
						}
						else if (strcmp(params[0], "PRIVMSG") == 0)
							msg_command(params);
						else if (strncmp("JOIN ", buf, 5) == 0)
							join_command(ft_substr(buf, 0, ft_strlen(buf) - 2), event_fd);
						else if (strcmp(params[0], "PART") == 0)
								part_command(params);
					}
				}				
			}
			close(socketfd);
			closeAllFd();
		}
		void part_command(char **str)
		{
			std::map<t_channels*, std::vector<int> >::iterator it = canals.begin();
			std::map<t_channels*, std::vector<int> >::iterator ite = canals.end();
			std::vector<int>::iterator it2 = fd_channels.begin();
			std::vector<int>::iterator ite2 = fd_channels.end();
			for (; it != ite; it++)
			{
				std::cout << it->first->name_channels <<"=" << str[1] << "r" << std::endl;
				if (strcmp(it->first->name_channels, str[1]) == 0)
				{
					for(; it2 != ite2; it2++)
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
						std::cout << a;
						if (send(*it2, welcome, ft_strlen(welcome), 0) < 0)
							perror("send error");
					}
				}
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
						a = c + "!" + d + "@localhost " + "PRIVMSG " + it->second->nickname + " " + str[2] + "\r\n";
						welcome = &a[0];
						welcome[a.length()] = '\0';
						if (send(it->first, welcome, ft_strlen(welcome), 0) < 0)
							perror("send error");
						//	std::cout << "IT->FIRST = " << it->first << "\n";
						//	send(it->first, str[2], ft_strlen(str[2]), 0);
					}
				}
			}
			else
			{
				std::map<t_channels*, std::vector<int> >::iterator it = canals.begin();
				std::map<t_channels*, std::vector<int> >::iterator ite = canals.end();
				std::vector<int>::iterator it2 = fd_channels.begin();
				std::vector<int>::iterator ite2 = fd_channels.end();
				for (; it != ite; it++)
				{

					if (strcmp(ft_substr(it->first->name_channels, 0 ,ft_strlen(it->first->name_channels) - 2), str[1]) == 0)
					{
						for(; it2 != ite2; it2++)
						{
							char *welcome = NULL;
							std::string a;
							std::string c = users[event_fd]->nickname;
							std::string d = users[event_fd]->username;
							a = ":" + c + "!" + d + "@localhost " + "PRIVMSG " + str[1] + " " + str[2] + "\r\n";
							welcome = &a[0];
							welcome[a.length()] = '\0';
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
			str[2] = ft_substr(str[2], 0, ft_strlen(str[2]) - 2);
			if (str && ft_strlen_tab(str) == 3)
			{
				if (strcmp(tmp, str[2]) == 0)
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
		}
		void	checkConnection()
		{
			char buf[1024];
			char res[1024];
			char **buf_info;
			bzero(res, 1024);
			bzero(buf, 1024);
			if (checkPassword[event_fd -5] == -3)
			{
				while (1)
				{
					if (recv(event_fd, buf, 1024, 0) < 0)
						perror("recv error");
					std::cout << "BUFFER : " << buf << std::endl;
					if (strcmp(buf, "") != 0)
					{
						strcat(res, buf);
						buf_info = ft_split(res, '\n');
						if (ft_strlen_tab(buf_info) == 4)
						{
							checkPassword[event_fd - 5] = -1;
							break;
						}
						else
							free_tab(buf_info);

					}
					bzero(buf, 1024);
				}
			}
			char *user = NULL;
			char *pass = NULL;
			char *nick = NULL;
			if (checkPassword[event_fd - 5] == -1)
			{
				if (buf_info && buf_info[0] && buf_info[1] && buf_info[2] && buf_info[3])
				{
					pass = ft_substr(buf_info[1], 5, ft_strlen(buf_info[1]) - 5);
					pass[ft_strlen(pass) - 1 ] = '\0';

					nick = ft_substr(buf_info[2], 5, ft_strlen(buf_info[2]) - 5);
					nick[ft_strlen(nick) - 1] = '\0';

					user = ft_substr(buf_info[3], 5, ft_strlen(buf_info[3]) - 5);
					user[ft_strlen(user) - 1] = '\0';

					pass_error(pass);
					nick_error(nick);
					users[event_fd]->nickname = nick;
				}
				if (user)
				{
					char **user_infos = ft_split(user, ' ');
					if (user_infos && user_infos[0] && user_infos[1] && user_infos[2] && user_infos[3])
					{
						//user_error(user_infos[0]);
						users[event_fd]->username = user_infos[0];
					}
					else
						if (send(event_fd, "461 : Not enough parameters\r\n", 40, 0) < 0)
							perror("send error");
				}
			}

		}
		void	welcomeRPL()
		{
			if (checkPassword[event_fd - 5] != -1 && checkPassword[event_fd - 5] != -2 && checkPassword[event_fd - 5] != 2)
			{
				char *welcome = NULL;
				std::string a = "001 ";
				std::string b = " :Welcome to the Internet Relay Network ";
				std::string c = users[event_fd]->nickname;
				std::string d = users[event_fd]->username;
				a = a + users[event_fd]->nickname + b + c + "!" + d + "@localhost\r\n";
				welcome = &a[0];
				welcome[a.length()] = '\0';
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
				checkPassword[event_fd - 5] = 1;
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
				std::cout << user << std::endl;
				if (strcmp(user, str) == 0)
				{
					char error_nickname[] = "433 rcheiko : Nickname is already in use.\r\n";
					char error_msg[] = "Nickname is already in use\r\n";
					if (send(event_fd, error_nickname, ft_strlen(error_nickname), 0) < 0)
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
			if (send(event_fd, "Please fill the following informations : \n", 41, 0) < 0)
				perror("send error");
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

		int good_name_channel(char *str)
		{
			std::cout << "str " <<str << std::endl;
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
						{
							std::cout << "oui"<< std::endl;
							return 0;
						}
						i++;
					}
					return 1;
				}
				std::cout << "oui"<< std::endl;
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
				if (strcmp(it->first->name_channels, str) == 0)
				{
					std::cout << "ALREADY EXIST\n";
					return 0;
				}
			}
			return 1;
		}


		int good_password_channel(char *str, char *channel_name)
		{
			std::map<t_channels*, std::vector<int > >::iterator it = canals.begin();
			std::map<t_channels*, std::vector<int > >::iterator ite = canals.end();
			for (; it != ite; it++)
			{
				if (strcmp(it->first->name_channels, channel_name) == 0)
				{
					if (strcmp(it->first->password_channel, str) == 0)
						return 0;
				}
			}
			return 1;
		}
		void	join_command(char *buffer, int user)
		{

			char **buffer_split = ft_split(buffer, ' ');
			if (ft_strlen_tab(buffer_split) > 1 && ft_strlen_tab(buffer_split) < 4)
			{

				if (users[user]->nbr_channel < 1000) 
				{
					char **channel_split = ft_split(buffer_split[1], ',');
					int i = -1;
					while (channel_split[++i]) 					
					{
						if(good_name_channel(channel_split[i]) == 1) // si le nom du channel est juste
						{
							t_channels *canaux = new t_channels;
							canaux->name_channels = channel_split[i];
							users[user]->nbr_channel++;
							if(buffer_split[2])
							{	
								char **password_buffer = ft_split(buffer_split[2], ',');
								canaux->password_channel = password_buffer[i];
							}
							fd_channels.push_back(user);
							canals[canaux] = fd_channels;
							std::vector<int>::iterator it2 = fd_channels.begin();
							std::vector<int>::iterator ite2 = fd_channels.end();
							for(; it2 != ite2; it2++)
							{
								char *welcome = NULL;
								std::string a;
								std::string c = users[user]->nickname;
								std::string d = users[user]->username;
								a = ":" + c + "!" + d + "@localhost JOIN " + canaux->name_channels +"\r\n";
								welcome = &a[0];
								welcome[a.length()] = '\0';
								if (send(*it2, welcome, ft_strlen(welcome), 0) < 0)
									perror("send error");
							}
							char *welcome = NULL;
							std::string a = ":localhost 332 ";
							std::string b = users[user]->nickname;
							a = a + b + " " + canaux->name_channels+ " " + ":VOICI LE TOPIC DE CE CHANNEL\r\n";
							welcome = &a[0];
							welcome[a.length()] = '\0';
							if (send(event_fd, welcome, ft_strlen(welcome), 0) < 0)
								perror("send error");
							char *welcomee = NULL;
							std::string c = ":localhost 353 ";
							std::string d = users[user]->nickname;
							c = c + d + " = " + ft_substr(canaux->name_channels, 0, ft_strlen(canaux->name_channels) - 2) + " :";
							std::vector<int>::iterator it3 = fd_channels.begin();
							std::vector<int>::iterator ite3 = fd_channels.end();
							for (; it3 != ite3 ; it3++)
							{
								c = c + " " + users[*it3]->nickname;
							}
							c = c + "\r\n";
							welcomee = &c[0];
							welcomee[c.length()] = '\0';
							if (send(event_fd, welcomee, ft_strlen(welcomee), 0) < 0)
								perror("send error");
							char *welcomeee = NULL;
							std::string e = ":localhost 366 ";
							std::string f = users[user]->nickname;
							e = e + f + " " + canaux->name_channels + " " + ":End of NAMES list\r\n";
							welcomeee = &e[0];
							welcomeee[e.length()] = '\0';
							if (send(event_fd, welcomeee, ft_strlen(welcomeee), 0) < 0)
								perror("send error");
							//std::cout << "premiere fois channel cree" << std::endl;

						}
						else
						{
							char *welcome = NULL;
							std::string a;
							std::string c = "403";
							std::string d = " :No such channel";
							a = c + d + "\r\n";
							welcome = &a[0];
							welcome[a.length()] = '\0';
							send(user, welcome, ft_strlen(welcome), 0);
						}
					}
				}
				else
				{
					char *welcome = NULL;
					std::string a;
					std::string c = "405";
					std::string d = " :You have joined too many channels";
					a = c + d + "\r\n";
					welcome = &a[0];
					welcome[a.length()] = '\0';
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
				welcome[a.length()] = '\0';
				send(user, welcome, ft_strlen(welcome), 0);
			}
			//std::cout << " end" << std::endl;
		}
	private:
		struct Node
		{
			std::string nickname;
			std::string username;
			std::string channel;
			unsigned int nbr_channel;
			bool		ope;
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
			char *name_channels;
			char *password_channel;
			int number_of_members;
			static unsigned int actif_members;
			bool	ope;
			bool	fill;
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
};


#endif
