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
  int fd[3];
  char buf[256];
  int len;
  int fdix;
  fd_set watchset;
  fd_set inset;
  int maxfd;
  int i;
  
  if(argc <4)
  {
    printf("Usage: 3 pathes of files\n");
    return 0;
  }

  for (i = 0; i<3; i++)
  {
    fd[i] = open(argv[i+1], O_RDONLY | O_NONBLOCK);
    if(fd[i] < 0)
    {
      perror("open");
      return EXIT_FAILURE;
    }
  }
 
  while(1)
  {
    FD_ZERO(&watchset);
    maxfd = 0;
    for ( i=0; i<3; i++)
    {
      FD_SET(fd[i], &watchset);
      if(fd[i] > maxfd)
        maxfd = fd[i];
    }

    inset = watchset;
    if(select(maxfd + 1, &inset, NULL, NULL, NULL) < 0)
    {
      perror("select");
      return EXIT_FAILURE;
    }

    for ( i=0; i<3; i++)
    {
      if(FD_ISSET(fd[i], &inset))
      {
        len = read(fd[i], buf, sizeof(buf));
        if(len == 0)
        {
          printf("A pipe %d lezarult!\n", i);
          return EXIT_SUCCESS;
        }
        else if((len < 0) && (errno != EAGAIN))
        {
          perror("read");
          return EXIT_FAILURE;
        }
        else if(len > 0)
        {
        printf("Userpace read: %x\n",*(unsigned int*)&buf[0]);
        }
      }
    }
  }

  for (i = 0; i<3; i++)
  {
    close(fd[i]);
  }
  
  return EXIT_SUCCESS;
}
