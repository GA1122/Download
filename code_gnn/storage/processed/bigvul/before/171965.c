int btpan_tap_close(int fd)
{
 if (tap_if_down(TAP_IF_NAME) == 0)
        close(fd);
 if (pan_pth >= 0)
        btsock_thread_wakeup(pan_pth);
 return 0;
}
