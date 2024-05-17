BOOLEAN UIPC_Open(tUIPC_CH_ID ch_id, tUIPC_RCV_CBACK *p_cback)
{
    BTIF_TRACE_DEBUG("UIPC_Open : ch_id %d, p_cback %x", ch_id, p_cback);

    UIPC_LOCK();

 if (ch_id >= UIPC_CH_NUM)
 {
        UIPC_UNLOCK();
 return FALSE;
 }

 if (uipc_main.ch[ch_id].srvfd != UIPC_DISCONNECTED)
 {
        BTIF_TRACE_EVENT("CHANNEL %d ALREADY OPEN", ch_id);
        UIPC_UNLOCK();
 return 0;
 }

 switch(ch_id)
 {
 case UIPC_CH_ID_AV_CTRL:
            uipc_setup_server_locked(ch_id, A2DP_CTRL_PATH, p_cback);
 break;

 case UIPC_CH_ID_AV_AUDIO:
            uipc_setup_server_locked(ch_id, A2DP_DATA_PATH, p_cback);
 break;
 }

    UIPC_UNLOCK();

 return TRUE;
}
