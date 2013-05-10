/*
This file is part of IPoEM.

IPoEM is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

IPoEM is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with IPoEM. If not, see <http://www.gnu.org/licenses/>.
*/

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include <linux/if.h>
#include <linux/if_tun.h>

int tun_alloc(char *dev)
{
	struct ifreq ifr;
	int fd, err;

	if ((fd = open("/dev/net/tun", O_RDWR)) < 0)
		return fd;

	memset(&ifr, 0, sizeof(ifr));

	/* Flags: IFF_TUN   - TUN device (no Ethernet headers)
	*        IFF_TAP   - TAP device
	*
	*        IFF_NO_PI - Do not provide packet information
	*/
	ifr.ifr_flags = IFF_TAP| IFF_NO_PI;
	if (dev && *dev)
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);

	if ((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
		close(fd);
		return err;
	}
	return fd;
}

int main(int argc, char *argv[])
{
	fd_set rfds;
	int fd = tun_alloc(argv[1]);

	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	FD_SET(fd, &rfds);

	for (;;) {
		char buf[2048];
		int i, retval = select(fd + 1, &rfds, NULL, NULL, NULL);
		if ((retval < 0) && (errno != EINTR))
			break;

		if (FD_ISSET(0, &rfds)) {
			retval = read(0, buf, 2048);
			if (retval > 0)
				write(fd, buf, retval);
		}
		if (FD_ISSET(fd, &rfds)) {
			retval = read(fd, buf, 2048);
			if (retval < 0)
				continue;

			if (!strcmp(buf, "exit"))
				break;

			for (i=0; i<retval; ++i)
				fprintf(stdout, "%02hhx", buf[i]);
			fprintf(stdout, "\n");
		}
	}
	close(fd);
	return 0;
}
