void create_tap_read_thread(int tap_fd)
{
 if (pan_pth < 0)
        pan_pth = btsock_thread_create(btpan_tap_fd_signaled, NULL);
 if (pan_pth >= 0)
        btsock_thread_add_fd(pan_pth, tap_fd, 0, SOCK_THREAD_FD_RD, 0);
}
