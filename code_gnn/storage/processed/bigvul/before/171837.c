static void btif_hl_clean_mdl_cb(btif_hl_mdl_cb_t *p_dcb)
{
    BTIF_TRACE_DEBUG("%s", __FUNCTION__ );
    btif_hl_free_buf((void **) &p_dcb->p_rx_pkt);
    btif_hl_free_buf((void **) &p_dcb->p_tx_pkt);
    memset(p_dcb, 0 , sizeof(btif_hl_mdl_cb_t));
}
