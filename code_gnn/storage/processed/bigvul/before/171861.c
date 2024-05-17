BOOLEAN btif_hl_find_mcl_idx_using_mdl_id( UINT8 mdl_id,UINT8 mcl_handle,
                                           UINT8 *p_app_idx, UINT8 *p_mcl_idx){
 btif_hl_app_cb_t *p_acb;
 btif_hl_mcl_cb_t *p_mcb;
    BOOLEAN         found=FALSE;
    UINT8 i,j,x;

 for (i=0; i<BTA_HL_NUM_APPS; i++)
 {
        p_acb =BTIF_HL_GET_APP_CB_PTR(i);
 for (j=0; j < BTA_HL_NUM_MCLS ; j++)
 {
 if (p_acb->mcb[j].in_use &&
 (p_acb->mcb[j].mcl_handle == mcl_handle))
 {
                    p_mcb = &p_acb->mcb[j];
                    BTIF_TRACE_DEBUG("btif_hl_find_mcl_idx_using_mdl_id: mcl handle found j =%d",j);
 for (x=0; x < BTA_HL_NUM_MDLS_PER_MCL ; x ++)
 {
 if (p_mcb->mdl[x].in_use && p_mcb->mdl[x].mdl_id == mdl_id)
 {
                            BTIF_TRACE_DEBUG("btif_hl_find_mcl_idx_using_mdl_id:found x =%d",x);
                            found = TRUE;
 *p_app_idx = i;
 *p_mcl_idx = j;
 break;
 }
 }
 }
 }
 }
    BTIF_TRACE_DEBUG("%s found=%d app_idx=%d mcl_idx=%d",__FUNCTION__,
                      found, i, j);
 return found;
}
