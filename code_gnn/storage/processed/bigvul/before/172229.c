void uipc_close_locked(tUIPC_CH_ID ch_id)
{
 if (uipc_main.ch[ch_id].srvfd == UIPC_DISCONNECTED)
 {
        BTIF_TRACE_EVENT("CHANNEL %d ALREADY CLOSED", ch_id);
 return;
 }

  
    uipc_main.ch[ch_id].task_evt_flags |= UIPC_TASK_FLAG_DISCONNECT_CHAN;
    uipc_wakeup_locked();
}
