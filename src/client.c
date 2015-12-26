/* Client code in C */

  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <ctype.h>
  #include <netdb.h>
  #include <sys/time.h>


  void usage() {
    printf("-b <buffersize in bytes> - default are 10kb \n");
    printf("-s <size in megabytes> - default are 10MB\n");
    printf("-h -? - this help\n");
  }


  int main(int argc, char *argv[])
  {
    char *version = "0.1";
    int res;
    int ch;
    struct in_addr ip;
    struct hostent *hp;
    
    struct addrinfo *addrres,hints; 
    int error = 0;

    const char *ipstr;
    const char *tcpport = "10100";
    
    struct timeval tval_before, tval_after, tval_result;
    float sec;

    /* read 10 mbyte */
    size_t read_size = 10000000; /* 10MB */
    size_t actual_read = 0;
    size_t read_buffer_size = 10000; /* 10kb */

    float bps;

    printf("Client version: %s\n\n",version);


    while ((ch = getopt(argc, argv,
        "b:hs:?")) != -1) {
	switch (ch) {
	    case 'b':
		read_buffer_size = strtoul(optarg,NULL,10);
		break;
	    case 's':
		read_size = strtoul(optarg,NULL,10) * 1000 * 1000;
		break;
	    case '?':
	    case 'h':
	    default:
		usage();
	}
    }

    argc -= optind;
    argv += optind;

    if(argv[0] && !argv[1]) {

      ipstr = argv[0];

    } else {

      usage();
      perror("please provide a ipv6 ip where the server is running");
      exit(EXIT_FAILURE);
    }


    int SocketFD = socket(PF_INET6, SOCK_STREAM, IPPROTO_TCP);
  
    if (-1 == SocketFD) {
      perror("cannot create socket");
      exit(EXIT_FAILURE);
    }


    memset(&hints, 0, sizeof(hints)); 
    hints.ai_family = PF_INET6;

    error = getaddrinfo(ipstr,tcpport,&hints,&addrres);

    if (error != 0) {
	fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(error));
        exit(EXIT_FAILURE);
    }
    
    if (-1 == connect(SocketFD, addrres->ai_addr,addrres->ai_addrlen)) {
      perror("connect failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
  
    /* take time before */
    gettimeofday(&tval_before, NULL);

    for(actual_read; actual_read <= read_size; actual_read += read_buffer_size) {
	char buffer[read_buffer_size];
	read(SocketFD,buffer,read_buffer_size);
    }
    
    /* take time after reading and divide */
    gettimeofday(&tval_after, NULL);

    timersub(&tval_after, &tval_before, &tval_result); 
    
    /* sec + usec */
    sec = tval_result.tv_sec + (tval_result.tv_usec * 0.000001);
    /* bytes * 8 = bits */
    /* bps = (float) (read_size  / tval_result.tv_usec); */
    bps = (read_size  * 8.0 /  sec);
    printf("Ready reading %i MB\n", read_size / 1000 / 1000);
    printf("Reading with  %f MBit/s\n", bps / 1000 / 1000 );
 
    (void)shutdown(SocketFD, SHUT_RDWR);

    freeaddrinfo(addrres);

    close(SocketFD);
    return EXIT_SUCCESS;
  }