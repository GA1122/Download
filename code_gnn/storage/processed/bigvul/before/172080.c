static inline void init_cmd_fd(int h)
{
    asrt(ts[h].cmd_fdr == -1 && ts[h].cmd_fdw == -1);
 if(socketpair(AF_UNIX, SOCK_STREAM, 0, &ts[h].cmd_fdr) < 0)
 {
        APPL_TRACE_ERROR("socketpair failed: %s", strerror(errno));
 return;
 }
    APPL_TRACE_DEBUG("h:%d, cmd_fdr:%d, cmd_fdw:%d", h, ts[h].cmd_fdr, ts[h].cmd_fdw);
    add_poll(h, ts[h].cmd_fdr, 0, SOCK_THREAD_FD_RD, 0);
}
