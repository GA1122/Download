static int activity_manager_connect() {
 int amfd = socket(PF_UNIX, SOCK_STREAM, 0);
 if (amfd >= 0) {
 struct sockaddr_un address;
 int err;

    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, NCRASH_SOCKET_PATH, sizeof(address.sun_path));
    err = TEMP_FAILURE_RETRY(connect(
        amfd, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)));
 if (!err) {
 struct timeval tv;
      memset(&tv, 0, sizeof(tv));
      tv.tv_sec = 1;  
      err = setsockopt(amfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 if (!err) {
        tv.tv_sec = 3;  
        err = setsockopt(amfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
 }
 }
 if (err) {
      close(amfd);
      amfd = -1;
 }
 }

 return amfd;
}
