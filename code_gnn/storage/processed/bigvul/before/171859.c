BOOLEAN btif_hl_find_mcl_idx_using_app_idx( tBTA_HL_MCL_HANDLE mcl_handle,
                                           UINT8 p_app_idx, UINT8 *p_mcl_idx){
 btif_hl_app_cb_t *p_acb;
    BOOLEAN         found=FALSE;
    UINT8 j;

    p_acb =BTIF_HL_GET_APP_CB_PTR(p_app_idx);
 for (j=0; j < BTA_HL_NUM_MCLS ; j++)
 {
 if (p_acb->mcb[j].in_use &&
 (p_acb->mcb[j].mcl_handle == mcl_handle))
 {
            found = TRUE;
 *p_mcl_idx = j;
 break;
 }
 }
    BTIF_TRACE_DEBUG("%s found=%dmcl_idx=%d",__FUNCTION__,
                      found, j);
 return found;
}
