static void redirect_to_32(int fd, debugger_request_t* request) {
 debugger_msg_t msg;
  memset(&msg, 0, sizeof(msg));
  msg.tid = request->tid;
  msg.action = request->action;

 int sock_fd = socket_local_client(DEBUGGER32_SOCKET_NAME, ANDROID_SOCKET_NAMESPACE_ABSTRACT,
                                    SOCK_STREAM | SOCK_CLOEXEC);
 if (sock_fd < 0) {
    ALOGE("Failed to connect to debuggerd32: %s", strerror(errno));
 return;
 }

 if (TEMP_FAILURE_RETRY(write(sock_fd, &msg, sizeof(msg))) != (ssize_t) sizeof(msg)) {
    ALOGE("Failed to write request to debuggerd32 socket: %s", strerror(errno));
    close(sock_fd);
 return;
 }

 char ack;
 if (TEMP_FAILURE_RETRY(read(sock_fd, &ack, 1)) == -1) {
    ALOGE("Failed to read ack from debuggerd32 socket: %s", strerror(errno));
    close(sock_fd);
 return;
 }

 char buffer[1024];
 ssize_t bytes_read;
 while ((bytes_read = TEMP_FAILURE_RETRY(read(sock_fd, buffer, sizeof(buffer)))) > 0) {
 ssize_t bytes_to_send = bytes_read;
 ssize_t bytes_written;
 do {
      bytes_written = TEMP_FAILURE_RETRY(write(fd, buffer + bytes_read - bytes_to_send,
                                               bytes_to_send));
 if (bytes_written == -1) {
 if (errno == EAGAIN) {
 continue;
 }
        ALOGE("Error while writing data to fd: %s", strerror(errno));
 break;
 }
      bytes_to_send -= bytes_written;
 } while (bytes_written != 0 && bytes_to_send > 0);
 if (bytes_to_send != 0) {
        ALOGE("Failed to write all data to fd: read %zd, sent %zd", bytes_read, bytes_to_send);
 break;
 }
 }
  close(sock_fd);

 }
