BOOLEAN UIPC_SendBuf(tUIPC_CH_ID ch_id, BT_HDR *p_msg)
{
    UNUSED(p_msg);

    BTIF_TRACE_DEBUG("UIPC_SendBuf : ch_id %d NOT IMPLEMENTED", ch_id);

    UIPC_LOCK();

  

    UIPC_UNLOCK();

 return FALSE;
}
