void UIPC_Close(tUIPC_CH_ID ch_id)
{
    BTIF_TRACE_DEBUG("UIPC_Close : ch_id %d", ch_id);

  
 if (ch_id != UIPC_CH_ID_ALL)
 {
        UIPC_LOCK();
        uipc_close_locked(ch_id);
        UIPC_UNLOCK();
 }
 else
 {
        BTIF_TRACE_DEBUG("UIPC_Close : waiting for shutdown to complete");
        uipc_stop_main_server_thread();
        BTIF_TRACE_DEBUG("UIPC_Close : shutdown complete");
 }
}
