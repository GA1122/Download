static inline int btif_hl_select_wake_signaled(fd_set* set){
    BTIF_TRACE_DEBUG("btif_hl_select_wake_signaled");
 return FD_ISSET(signal_fds[0], set);
}
