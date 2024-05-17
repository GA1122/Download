void UIPC_ReadBuf(tUIPC_CH_ID ch_id, BT_HDR *p_msg)
{
    UNUSED(p_msg);

    BTIF_TRACE_DEBUG("UIPC_ReadBuf : ch_id:%d NOT IMPLEMENTED", ch_id);

    UIPC_LOCK();
    UIPC_UNLOCK();
}
