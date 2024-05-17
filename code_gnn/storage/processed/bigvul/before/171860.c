BOOLEAN btif_hl_find_mcl_idx_using_handle( tBTA_HL_MCL_HANDLE mcl_handle,
                                           UINT8 *p_app_idx, UINT8 *p_mcl_idx){
 btif_hl_app_cb_t *p_acb;
    BOOLEAN         found=FALSE;
    UINT8 i,j;

 for (i=0; i<BTA_HL_NUM_APPS; i++)
 {
        p_acb =BTIF_HL_GET_APP_CB_PTR(i);
 for (j=0; j < BTA_HL_NUM_MCLS ; j++)
 {
 if (p_acb->mcb[j].in_use)
                BTIF_TRACE_DEBUG("btif_hl_find_mcl_idx_using_handle:app_idx=%d,"
 "mcl_idx =%d mcl_handle=%d",i,j,p_acb->mcb[j].mcl_handle);
 if (p_acb->mcb[j].in_use &&
 (p_acb->mcb[j].mcl_handle == mcl_handle))
 {
                found = TRUE;
 *p_app_idx = i;
 *p_mcl_idx = j;
 break;
 }
 }
 }
    BTIF_TRACE_DEBUG("%s found=%d app_idx=%d mcl_idx=%d",__FUNCTION__,
                      found, i, j);
 return found;
}
