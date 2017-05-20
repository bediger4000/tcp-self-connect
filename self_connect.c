#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>

int failcount = 0;

void
sighandler(int signo)
{
	fprintf(stderr, "Signal handler got signal %d, failcount %d\n",
		signo, failcount);
	exit(3);
}


int
main(int ac, char **av)
{
	struct sockaddr_in sin;
	short port;
	struct hostent *server;

	signal(SIGINT, sighandler);

	server = gethostbyname(av[1]);
	if (!server)
	{
		fprintf(stderr, "No server named \"%s\"\n", av[1]);
		exit(1);
	}

	port = atoi(av[2]);

	bzero((char *)&sin, sizeof(sin));

	sin.sin_family = AF_INET;
	bcopy(
		(char *)server->h_addr,
		(char *)&sin.sin_addr.s_addr,
		server->h_length
	);
    sin.sin_port = htons(port);


	while (1)
	{
		int socketfd;
		if (0 > (socketfd = socket(PF_INET, SOCK_STREAM,  IPPROTO_TCP)))
		{
			fprintf(stderr, "Problem with socket(): %s\n", strerror(errno));
			break;
		}

		if (0 > connect(socketfd, (struct sockaddr *)&sin, sizeof(sin)))
		{
			++failcount;
		} else {
			char buffer[2048];
			int cc;
			struct sockaddr_in m;
			socklen_t msz = sizeof(m);
			fprintf(stderr, "worked after fail count %d\n", failcount);
			if (getsockname(socketfd, (struct sockaddr *)&m, &msz) >= 0)
			{
				fprintf(stderr, "my socket bound to %u.%u.%u.%u:%hd\n", 
					m.sin_addr.s_addr & 0xff,
					(m.sin_addr.s_addr >> 8) & 0xff,
					(m.sin_addr.s_addr >> 16) & 0xff,
					(m.sin_addr.s_addr >> 24) & 0xff,
					m.sin_port
				);
			} else {
				fprintf(stderr, "getsockname() failed: %s\n",
					strerror(errno));
			}
			if (getpeername(socketfd, (struct sockaddr *)&m, &msz) >= 0)
			{
				fprintf(stderr, "my peer bound to %u.%u.%u.%u:%hd\n", 
					m.sin_addr.s_addr & 0xff,
					(m.sin_addr.s_addr >> 8) & 0xff,
					(m.sin_addr.s_addr >> 16) & 0xff,
					(m.sin_addr.s_addr >> 24) & 0xff,
					m.sin_port
				);
			} else {
				fprintf(stderr, "getsockname() failed: %s\n",
					strerror(errno));
			}
			write(socketfd, "hello world!", 12);
			if ((cc = read(socketfd, buffer, sizeof(buffer))) > 0)
				fprintf(stderr, "Received \"%.*s\" from socket\n", cc, buffer);

			fprintf(stderr, "Pause, time for netstat -t, and ls -l /proc/%d/fd\n", getpid());
			fgets(buffer, sizeof(buffer), stdin);

			close(socketfd);
			break;
		}

		close(socketfd);
	}
	return 0;
}
