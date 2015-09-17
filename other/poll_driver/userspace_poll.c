#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#include <sys/select.h>

int main(int argc, char* argv[])
{
  int fd;
  char buf[256];
  int len;
  int fdix;
  fd_set watchset;
  fd_set inset;
  int maxfd;
  
  if(argc <2)
  {
		printf("Usage: <path of file>\n");
    return 0;
  }
  fd = open(argv[1], O_RDONLY | O_NONBLOCK);
  if(fd < 0)
  {
    perror("open");
    return EXIT_FAILURE;
  }
  
  
  FD_ZERO(&watchset);
  FD_SET(fd, &watchset);
  maxfd = fd;
  
  while(1)
  {
    inset = watchset;
    if(select(maxfd + 1, &inset, NULL, NULL, NULL) < 0)
    {
      perror("select");
      return EXIT_FAILURE;
    }
    
      if(FD_ISSET(fd, &inset))
      {
	len = read(fd, buf, sizeof(buf));
	if(len == 0)
	{
	  printf("A pipe%d lezarult!\n", fdix + 1);
	  return EXIT_SUCCESS;
	}
	else if((len < 0) && (errno != EAGAIN))
	{
	  perror("read");
	  return EXIT_FAILURE;
	}
	else if(len > 0)
	{
	  write(STDOUT_FILENO, buf, len);
	}
      }
  }
  
  close(fd);
  
  return EXIT_SUCCESS;
}
