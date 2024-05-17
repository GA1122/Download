void destroy_tap_read_thread(void)
{
 if (pan_pth >= 0)
 {
        btsock_thread_exit(pan_pth);
        pan_pth = -1;
 }
}
