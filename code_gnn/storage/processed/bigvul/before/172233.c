 static int uipc_setup_server_locked(tUIPC_CH_ID ch_id, char *name, tUIPC_RCV_CBACK *cback)
{
 int fd;

    BTIF_TRACE_EVENT("SETUP CHANNEL SERVER %d", ch_id);

 if (ch_id >= UIPC_CH_NUM)
 return -1;

    UIPC_LOCK();

    fd = create_server_socket(name);

 if (fd < 0)
 {
        BTIF_TRACE_ERROR("failed to setup %s", name, strerror(errno));
        UIPC_UNLOCK();
 return -1;
 }

    BTIF_TRACE_EVENT("ADD SERVER FD TO ACTIVE SET %d", fd);
    FD_SET(fd, &uipc_main.active_set);
    uipc_main.max_fd = MAX(uipc_main.max_fd, fd);

    uipc_main.ch[ch_id].srvfd = fd;
    uipc_main.ch[ch_id].cback = cback;
    uipc_main.ch[ch_id].read_poll_tmo_ms = DEFAULT_READ_POLL_TMO_MS;

  
    uipc_wakeup_locked();

    UIPC_UNLOCK();

 return 0;
}
