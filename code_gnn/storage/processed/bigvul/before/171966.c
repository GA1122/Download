static void btpan_tap_fd_signaled(int fd, int type, int flags, uint32_t user_id) {
    assert(btpan_cb.tap_fd == INVALID_FD || btpan_cb.tap_fd == fd);

 if (btpan_cb.tap_fd != fd) {
        BTIF_TRACE_WARNING("%s Signaled on mismatched fds exp:%d act:%d\n",
                __func__, btpan_cb.tap_fd, fd);
 return;
 }

 if (flags & SOCK_THREAD_FD_EXCEPTION) {
        btpan_cb.tap_fd = INVALID_FD;
        btpan_tap_close(fd);
        btif_pan_close_all_conns();
 } else if (flags & SOCK_THREAD_FD_RD)
        bta_dmexecutecallback(btu_exec_tap_fd_read, (void *)fd);
}
