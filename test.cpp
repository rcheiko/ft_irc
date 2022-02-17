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

int main()
{
	std::string str;
	char *ok;

	str = "YO CA VA";

	ok = &str[0];
	std::cout << ok << std::endl;
	return (0);
}
