void uipc_main_cleanup(void)
{
 int i;

    BTIF_TRACE_EVENT("uipc_main_cleanup");

    close(uipc_main.signal_fds[0]);
    close(uipc_main.signal_fds[1]);

  
 for (i=0; i<UIPC_CH_NUM; i++)
        uipc_close_ch_locked(i);
}
