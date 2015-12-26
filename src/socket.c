  /* Server code in C */
     
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <fcntl.h>
  
  int main(void)
  {
    struct sockaddr_in6 sa;
    int SocketFD = socket(PF_INET6, SOCK_STREAM, IPPROTO_TCP);
  
    if (-1 == SocketFD) {
      perror("cannot create socket");
      exit(EXIT_FAILURE);
    }
  
    memset(&sa, 0, sizeof sa);
  
    sa.sin6_family = AF_INET6;
    sa.sin6_port = htons(10100);
    /* sa.sin6_addr.s_addr = htonl(INADDR_ANY); */
    sa.sin6_addr = in6addr_any;
  
    if (-1 == bind(SocketFD,(struct sockaddr *)&sa, sizeof sa)) {
      perror("bind failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
  
    if (-1 == listen(SocketFD, 10)) {
      perror("listen failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

    int dev_zero = open("/dev/zero",O_RDONLY);
    size_t copied = 0;
    size_t copysize = 10240000; /* 10 MB */
  
    for (;;) {
      int ConnectFD = accept(SocketFD, NULL, NULL);
  
      if (0 > ConnectFD) {
        perror("accept failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
      }

      while ( copied < copysize )
      {
	    char myRandomDataBuf[102400];
	    ssize_t result = read(dev_zero, myRandomDataBuf, (sizeof myRandomDataBuf));
    	    if (result < 0)
	    {
    		// error, unable to read /dev/random 
    	    }
	    copied += (sizeof myRandomDataBuf);
	    printf("Sending %i bytes of zeros ...\n", (sizeof myRandomDataBuf));
	    write(ConnectFD,myRandomDataBuf,(sizeof myRandomDataBuf));
       }
      close(dev_zero);
  
      if (-1 == shutdown(ConnectFD, SHUT_RDWR)) {
        perror("shutdown failed");
        close(ConnectFD);
        close(SocketFD);
        exit(EXIT_FAILURE);
      }
      close(ConnectFD);
    }

    close(SocketFD);
    return EXIT_SUCCESS;  
}
