static inline int btif_hl_select_wakeup_init(fd_set* set){
    BTIF_TRACE_DEBUG("%s", __func__);
 if (signal_fds[0] == -1 && socketpair(AF_UNIX, SOCK_STREAM, 0, signal_fds) < 0)
 {
        BTIF_TRACE_ERROR("socketpair failed: %s", strerror(errno));
 return -1;
 }

    BTIF_TRACE_DEBUG("btif_hl_select_wakeup_init signal_fds[0]=%d signal_fds[1]=%d",signal_fds[0], signal_fds[1] );
    FD_SET(signal_fds[0], set);

 return signal_fds[0];
}
