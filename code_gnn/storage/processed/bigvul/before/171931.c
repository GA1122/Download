void btif_hl_tmr_hdlr(TIMER_LIST_ENT *tle)
{
 btif_hl_mcl_cb_t *p_mcb;
    UINT8               i,j;
    BTIF_TRACE_DEBUG("%s timer_in_use=%d",  __FUNCTION__, tle->in_use );

 for (i=0; i < BTA_HL_NUM_APPS ; i ++)
 {
 for (j=0; j< BTA_HL_NUM_MCLS; j++)
 {
            p_mcb =BTIF_HL_GET_MCL_CB_PTR(i,j);

 if (p_mcb->cch_timer_active)
 {
                BTIF_TRACE_DEBUG("%app_idx=%d, mcl_idx=%d mcl-connected=%d",
                                  i, j,  p_mcb->is_connected);
                p_mcb->cch_timer_active = FALSE;
 if (p_mcb->is_connected)
 {
                    BTIF_TRACE_DEBUG("Idle timeout Close CCH app_idx=%d mcl_idx=%d mcl_handle=%d",
                                      i ,j, p_mcb->mcl_handle);
                    BTA_HlCchClose(p_mcb->mcl_handle);
 }
 else
 {
                    BTIF_TRACE_DEBUG("CCH idle timeout But CCH not connected app_idx=%d mcl_idx=%d ",i,j);
 }
 }
 }
 }
}
