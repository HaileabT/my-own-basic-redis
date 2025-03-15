/**
 * Simple socket Server
 * System calls used socket(), setsockopts, bind(), accept(), write(), read(), close()
 * Writes word to client socket connection
 */
#include <cstdio>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
// #include <cstring>


constexpr int ON = 1;
constexpr int OFF = 0;
constexpr int PORT = 4000;
constexpr int IP_ADDRESS = 0;
constexpr int READ_WRITE_BUFFERS_SIZE = 64;

static void app_logic(const int conn_fd) {
  // Read what client has sent
  char r_buf[READ_WRITE_BUFFERS_SIZE] = {};
  if (const ssize_t read_bytes = read(conn_fd, r_buf, READ_WRITE_BUFFERS_SIZE - 1); read_bytes <= 0) {
    return;
  }
  printf("Client says %s\n", r_buf);

  // Respond to client
  constexpr char w_buf[] = "hey, whatsup!";
  if (const ssize_t written_bytes = write(conn_fd, w_buf, sizeof(w_buf)); written_bytes <0) {
    perror("write");
    exit(1);
  }

}

int main() {
  // Create a socket communication
  const int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  // If the system fails to do so, exit
  if (socket_fd == -1) {
    perror("socket");
    exit(1);
  }


  // Configure the socket communication
  // If the system fails to set the options for the socket we can ignore (because we are not building anything serious)
  if (const int opts_set = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &ON, sizeof(ON)); opts_set == -1) {
    perror("setsockopt");
  }

  // Bind this communication medium to an address on the server
  // Create the address
  sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htons(IP_ADDRESS);
  constexpr int addr_len = sizeof(addr);
  // Bind the address with the socket
  // If this fails exit
  if (const int addr_bound = bind(socket_fd, reinterpret_cast<sockaddr*>(&addr), addr_len); addr_bound == -1) {
    perror("bind");
    exit(1);
  }

  // Initiate server to listen
  // If it fails exit
  if (const int server_init = listen(socket_fd, SOMAXCONN); server_init < 0) { perror("listen"); exit(1); }


  while (true) { // Keep the server alive
    // Define client connections
    sockaddr_in client_addr = {};
    socklen_t client_addr_len = sizeof(client_addr);
    // Accept connection
    const int conn_fd = accept(socket_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_len);
    // If it fails exit
    if (conn_fd < 0) {
      perror("accept");
      exit(1);
    }

    app_logic(conn_fd);
    close(conn_fd);
  }
  return 0;
  }
