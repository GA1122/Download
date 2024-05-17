static void btif_hl_clean_pcb(btif_hl_pending_chan_cb_t *p_pcb)
{
    BTIF_TRACE_DEBUG("%s", __FUNCTION__ );
    memset(p_pcb, 0 , sizeof(btif_hl_pending_chan_cb_t));
}
