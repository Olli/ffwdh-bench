/* 
    Code is Public Domain 
    Server pipes zeros on port 10100


*/
     
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <stdlib.h>
  
  int main(int argc, char *argv[]) {
    char *version = "0.1";
    struct sockaddr_in6 sa;
    int SocketFD = socket(PF_INET6, SOCK_STREAM, IPPROTO_TCP);
    int dev_zero; /* fd for /dev/zero */
    int ConnectFD; /* fd for connection */
    size_t buffersize;
    printf("Server version: %s\n\n",version);


    if (-1 == SocketFD) {
      perror("cannot create socket");
      exit(EXIT_FAILURE);
    }
  
    memset(&sa, 0, sizeof sa);
  
    sa.sin6_family = AF_INET6;
    sa.sin6_port = htons(10100);
    /* bind on any address */
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

    dev_zero = open("/dev/zero",O_RDONLY);

    buffersize = 10240;
    if(argc > 1) {
      const char *bytes = argv[1];
      unsigned long num = strtoul(bytes,NULL,10);
      if(num > 0)
	 buffersize = (size_t) num;
    }
	

    printf("Buffersize %i bytes\n",buffersize);
    
    
    for (;;) {
       
      ConnectFD = accept(SocketFD, NULL, NULL);
  
      if (0 > ConnectFD) {
        perror("accept failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
      }
	
      char data_buffer[buffersize];

      while(read(dev_zero,data_buffer, (sizeof data_buffer))) {
	    printf("Sending %i bytes of zeros ...\n", buffersize);
	    send(ConnectFD,data_buffer,(sizeof data_buffer), MSG_DONTROUTE);
      }

      close(dev_zero);
      if (-1 == shutdown(ConnectFD, SHUT_RDWR)) {
        perror("shutdown failed");
      }
      close(ConnectFD);
    }

    close(SocketFD);
    return EXIT_SUCCESS;  
}
