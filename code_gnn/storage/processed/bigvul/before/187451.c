 static int skt_write(int fd, const void *p, size_t len)
 {
  int sent;
  struct pollfd pfd;
 
     FNLOG();
 
     pfd.fd = fd;
     pfd.events = POLLOUT;
 
 
       
  
       
    if (poll(&pfd, 1, 500) == 0)
//     if (TEMP_FAILURE_RETRY(poll(&pfd, 1, 500)) == 0)
          return 0;
  
      ts_log("skt_write", len, NULL);
  
    if ((sent = send(fd, p, len, MSG_NOSIGNAL)) == -1)
//     if ((sent = TEMP_FAILURE_RETRY(send(fd, p, len, MSG_NOSIGNAL))) == -1)
      {
          ERROR("write failed with errno=%d\n", errno);
          return -1;
  }
 
  return sent;
 }