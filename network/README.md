

My tcp/udp code is in MyNETBench-TCP.c/MyNETBench-UDP.c file. If you want to compile this code, do this below:
for tcp server, compile command is:
gcc MyNETBench-TCP.c -pthread -lm- o tcp_server
for udp server, compile command is:
gcc MyNETBench-UDP.c -pthread -lm- o udp_server
for tcp client, the command is:
gcc MyNETBench-TCP.c -pthread -lm- o tcp_client
for udp client, the command is:
gcc MyNETBench-UDP.c -pthread -lm- o udp_client

Because my code is very sensitive to compiled name, please do not change tcp/udp_server/client to other name.

For server, you just type ./tcp_server for establish tcp server; run ./udp_server to establish udp server
and then you use other terminal run client: ./tcp_client for TCP client query server;./udp_client for UDP client query server.


You can run different type of data, just put data to data folder.

if you don't have data folder, please generate it.