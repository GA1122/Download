BOOLEAN btif_hl_if_channel_setup_pending(int channel_id, UINT8 *p_app_idx, UINT8 *p_mcl_idx)
{
 btif_hl_app_cb_t *p_acb;
 btif_hl_mcl_cb_t *p_mcb;
    UINT8 i, j;
    BOOLEAN found=FALSE;

 *p_app_idx = 0;
 *p_mcl_idx = 0;
 for (i=0; i < BTA_HL_NUM_APPS ; i ++)
 {
        p_acb  =BTIF_HL_GET_APP_CB_PTR(i);
 if (p_acb->in_use)
 {
 for (j=0; j< BTA_HL_NUM_MCLS; j++)
 {
                p_mcb = BTIF_HL_GET_MCL_CB_PTR(i, j);
 if (p_mcb->in_use &&
                    p_mcb->is_connected && p_mcb->pcb.channel_id == channel_id )
 {
                    found = TRUE;
 *p_app_idx = i;
 *p_mcl_idx = j;
 break;
 }
 }
 }
 if (found)
 break;
 }
    BTIF_TRACE_DEBUG("%s found=%d channel_id=0x%08x",
                      __FUNCTION__, found, channel_id, *p_app_idx, *p_mcl_idx);
 return found;

}
