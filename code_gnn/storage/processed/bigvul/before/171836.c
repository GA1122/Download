static void btif_hl_clean_mcl_cb(UINT8 app_idx, UINT8 mcl_idx)
{
 btif_hl_mcl_cb_t *p_mcb;
    BTIF_TRACE_DEBUG("%s app_idx=%d, mcl_idx=%d", __FUNCTION__,app_idx, mcl_idx);
    p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
    memset(p_mcb, 0, sizeof(btif_hl_mcl_cb_t));
}
