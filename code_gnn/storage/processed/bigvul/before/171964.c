void btpan_set_flow_control(BOOLEAN enable) {
 if (btpan_cb.tap_fd == -1)
 return;

    btpan_cb.flow = enable;
 if (enable) {
        btsock_thread_add_fd(pan_pth, btpan_cb.tap_fd, 0, SOCK_THREAD_FD_RD, 0);
        bta_dmexecutecallback(btu_exec_tap_fd_read, (void *)btpan_cb.tap_fd);
 }
}
