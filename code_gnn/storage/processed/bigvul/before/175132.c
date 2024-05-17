void wifi_event_loop(wifi_handle handle)
{
    hal_info *info = getHalInfo(handle);
 if (info->in_event_loop) {
 return;
 } else {
        info->in_event_loop = true;
 }

    pollfd pfd[2];
    memset(&pfd[0], 0, sizeof(pollfd) * 2);

    pfd[0].fd = nl_socket_get_fd(info->event_sock);
    pfd[0].events = POLLIN;
    pfd[1].fd = info->cleanup_socks[1];
    pfd[1].events = POLLIN;

 char buf[2048];
  

 do {
 int timeout = -1;  
        pfd[0].revents = 0;
        pfd[1].revents = 0;
 int result = poll(pfd, 2, timeout);
 if (result < 0) {
 } else if (pfd[0].revents & POLLERR) {
            ALOGE("POLL Error; error no = %d", errno);
 int result2 = read(pfd[0].fd, buf, sizeof(buf));
            ALOGE("Read after POLL returned %d, error no = %d", result2, errno);
 } else if (pfd[0].revents & POLLHUP) {
            ALOGE("Remote side hung up");
 break;
 } else if (pfd[0].revents & POLLIN) {
            internal_pollin_handler(handle);
 } else if (pfd[1].revents & POLLIN) {
            memset(buf, 0, sizeof(buf));
 int result2 = read(pfd[1].fd, buf, sizeof(buf));
            ALOGE("%s: Read after POLL returned %d, error no = %d", __FUNCTION__, result2, errno);
 if (strncmp(buf, "Exit", 4) == 0) {
                ALOGD("Got a signal to exit!!!");
 if (write(pfd[1].fd, "Done", 4) < 1) {
                    ALOGE("could not write to the cleanup socket");
 }
 break;
 } else {
                ALOGD("Rx'ed %s on the cleanup socket\n", buf);
 }
 } else {
            ALOGE("Unknown event - %0x, %0x", pfd[0].revents, pfd[1].revents);
 }
 } while (!info->clean_up);
    ALOGI("Exit %s", __FUNCTION__);
}
