BOOLEAN btif_hl_find_mdl_idx_using_handle(tBTA_HL_MDL_HANDLE mdl_handle,
                                          UINT8 *p_app_idx,UINT8 *p_mcl_idx,
                                          UINT8 *p_mdl_idx){
 btif_hl_app_cb_t *p_acb;
 btif_hl_mcl_cb_t *p_mcb;
 btif_hl_mdl_cb_t *p_dcb;
    BOOLEAN found=FALSE;
    UINT8 i,j,k;

 *p_app_idx = 0;
 *p_mcl_idx =0;
 *p_mdl_idx = 0;
 for (i=0; i < BTA_HL_NUM_APPS ; i ++)
 {
        p_acb =BTIF_HL_GET_APP_CB_PTR(i);
 for (j=0; j< BTA_HL_NUM_MCLS; j++)
 {
            p_mcb =BTIF_HL_GET_MCL_CB_PTR(i,j);
 for (k=0; k< BTA_HL_NUM_MDLS_PER_MCL; k++)
 {
                p_dcb =BTIF_HL_GET_MDL_CB_PTR(i,j,k);
 if (p_acb->in_use &&
                    p_mcb->in_use &&
                    p_dcb->in_use &&
 (p_dcb->mdl_handle == mdl_handle))
 {
                    found = TRUE;
 *p_app_idx = i;
 *p_mcl_idx =j;
 *p_mdl_idx = k;
 break;
 }
 }
 }
 }


    BTIF_TRACE_EVENT("%s found=%d app_idx=%d mcl_idx=%d mdl_idx=%d  ",
                      __FUNCTION__,found,i,j,k );
 return found;
}
