void btif_hl_start_cch_timer(UINT8 app_idx, UINT8 mcl_idx)
{
 btif_hl_mcl_cb_t *p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
    BTIF_TRACE_DEBUG("%s app_idx=%d, mcl_idx=%d  timer_active=%d timer_in_use=%d",
                      __FUNCTION__,app_idx, mcl_idx,
                      p_mcb->cch_timer_active, p_mcb->cch_timer.in_use);

    p_mcb->cch_timer_active = TRUE;
 if (!p_mcb->cch_timer.in_use)
 {
        BTIF_TRACE_DEBUG("Start CCH timer ");
        memset(&p_mcb->cch_timer, 0, sizeof(TIMER_LIST_ENT));
        p_mcb->cch_timer.param = (UINT32)btif_hl_tmr_hdlr;
        btu_start_timer(&p_mcb->cch_timer, BTU_TTYPE_USER_FUNC,
                        BTIF_TIMEOUT_CCH_NO_DCH_SECS);
 }
 else
 {
        BTIF_TRACE_DEBUG("Restart CCH timer ");
        btu_stop_timer(&p_mcb->cch_timer);
        btu_start_timer(&p_mcb->cch_timer, BTU_TTYPE_USER_FUNC,
                        BTIF_TIMEOUT_CCH_NO_DCH_SECS);
 }

}
