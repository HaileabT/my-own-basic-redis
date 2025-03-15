/**
 * Simple socket client
 * System calls used socket(), connect(), write(), read(), close()
 * Writes hello to server socket connection
 */
#include <arpa/inet.h>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

constexpr int ON = 1;
constexpr int OFF = 0;
constexpr int PORT = 4000;
constexpr int IP_ADDRESS = 0;
constexpr int READ_BUFFERS_SIZE = 64;


static void app_logic(const int conn_fd) {
  // Write to socket server
  constexpr char w_buf[] = "hello";
  write(conn_fd, w_buf, strlen(w_buf));

  // Read and print out response
  char r_buf[READ_BUFFERS_SIZE] = {};
  if (const ssize_t n = read(conn_fd, r_buf, READ_BUFFERS_SIZE - 1); n < 0) {
    perror("read() error");
    return;
  }
  printf("server says: %s\n", r_buf);
}

int main() {
  // Create socket communication
  // Exit if it fails
  const int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    perror("socket");
    exit(1);
  }

  // Create the server address
  sockaddr_in server_address = {};
  server_address.sin_family = AF_INET;
  server_address.sin_port = ntohs(PORT);
  server_address.sin_addr.s_addr = ntohl(IP_ADDRESS);

  // Try to connect to server
  // Exit if it fails
  if (const int conn = connect(socket_fd, reinterpret_cast<sockaddr *>(&server_address), sizeof(server_address));
    conn == -1) {
    perror("connect");
    exit(1);
  }

  app_logic(socket_fd);
  close(socket_fd);

  return 0;
}
