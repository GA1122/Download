void btif_hl_stop_cch_timer(UINT8 app_idx, UINT8 mcl_idx)
{
 btif_hl_mcl_cb_t *p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
    BTIF_TRACE_DEBUG("%s app_idx=%d, mcl_idx=%d timer_in_use=%d",
                      __FUNCTION__,app_idx, mcl_idx, p_mcb->cch_timer.in_use);

    p_mcb->cch_timer_active = FALSE;
 if (p_mcb->cch_timer.in_use)
 {
        BTIF_TRACE_DEBUG("stop CCH timer ");
        btu_stop_timer(&p_mcb->cch_timer);
 }
}
