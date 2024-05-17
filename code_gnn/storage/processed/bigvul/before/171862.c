static  BOOLEAN btif_hl_find_mdep_cfg_idx(UINT8 app_idx,  tBTA_HL_MDEP_ID local_mdep_id,
                                          UINT8 *p_mdep_cfg_idx){
 btif_hl_app_cb_t *p_acb =BTIF_HL_GET_APP_CB_PTR(app_idx);
    tBTA_HL_SUP_FEATURE     *p_sup_feature= &p_acb->sup_feature;
    BOOLEAN found =FALSE;
    UINT8 i;

 for (i=0; i< p_sup_feature->num_of_mdeps; i++)
 {
        BTIF_TRACE_DEBUG("btif_hl_find_mdep_cfg_idx: mdep_id=%d app_idx = %d",
                    p_sup_feature->mdep[i].mdep_id,app_idx);
 if ( p_sup_feature->mdep[i].mdep_id == local_mdep_id)
 {
            found = TRUE;
 *p_mdep_cfg_idx = i;
 break;
 }
 }

    BTIF_TRACE_DEBUG("%s found=%d mdep_idx=%d local_mdep_id=%d app_idx=%d ",
                      __FUNCTION__, found,i, local_mdep_id,app_idx);
 return found;
}
