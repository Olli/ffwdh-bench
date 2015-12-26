# FFWDH Bench - simple benchmark tool

The tool aims to be a simple benchmarking tool for 
wireless connections in mesh networks.
This software is absolutly alpha status. I'm not
a C coder and this software is likly full of bugs 
and wrong doings. 
I think this software works for its purpose. 

Just do ./server  and on the client ./client »link local ipv6 address«%»interface« (without »«)

If you set on the server a buffer other than the 100000 bytes default
you should use the same buffersize on client with -b . 
If you want transfer more than 10MB you should add the -s 100 on the client for 100MB 
transfer.

Fork it and fix the bugs please - thx :)

This software is Public Domain - you don't have to do anything
to use, modify and distribute it. Just do it and don't ask.
