static int uipc_close_ch_locked(tUIPC_CH_ID ch_id)
{
 int wakeup = 0;

    BTIF_TRACE_EVENT("CLOSE CHANNEL %d", ch_id);

 if (ch_id >= UIPC_CH_NUM)
 return -1;

 if (uipc_main.ch[ch_id].srvfd != UIPC_DISCONNECTED)
 {
        BTIF_TRACE_EVENT("CLOSE SERVER (FD %d)", uipc_main.ch[ch_id].srvfd);
        close(uipc_main.ch[ch_id].srvfd);
        FD_CLR(uipc_main.ch[ch_id].srvfd, &uipc_main.active_set);
        uipc_main.ch[ch_id].srvfd = UIPC_DISCONNECTED;
        wakeup = 1;
 }

 if (uipc_main.ch[ch_id].fd != UIPC_DISCONNECTED)
 {
        BTIF_TRACE_EVENT("CLOSE CONNECTION (FD %d)", uipc_main.ch[ch_id].fd);
        close(uipc_main.ch[ch_id].fd);
        FD_CLR(uipc_main.ch[ch_id].fd, &uipc_main.active_set);
        uipc_main.ch[ch_id].fd = UIPC_DISCONNECTED;
        wakeup = 1;
 }

  
 if (uipc_main.ch[ch_id].cback)
        uipc_main.ch[ch_id].cback(ch_id, UIPC_CLOSE_EVT);

  
 if (wakeup)
        uipc_wakeup_locked();

 return 0;
}
