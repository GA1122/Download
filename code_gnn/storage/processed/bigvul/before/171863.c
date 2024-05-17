BOOLEAN btif_hl_find_mdl_cfg_idx_using_channel_id(int channel_id,
                                                  UINT8 *p_app_idx,
                                                  UINT8 *p_mdl_cfg_idx){
 btif_hl_app_cb_t *p_acb;
 btif_hl_mdl_cfg_t *p_mdl;
    BOOLEAN found=FALSE;
    UINT8 i,j;
 int mdl_cfg_channel_id;

 *p_app_idx = 0;
 *p_mdl_cfg_idx =0;
 for (i=0; i < BTA_HL_NUM_APPS ; i ++)
 {
        p_acb =BTIF_HL_GET_APP_CB_PTR(i);
 for (j=0; j< BTA_HL_NUM_MDL_CFGS; j++)
 {
            p_mdl =BTIF_HL_GET_MDL_CFG_PTR(i,j);
            mdl_cfg_channel_id = *(BTIF_HL_GET_MDL_CFG_CHANNEL_ID_PTR(i,j));
 if (p_acb->in_use &&
                p_mdl->base.active &&
 (mdl_cfg_channel_id == channel_id))
 {
                found = TRUE;
 *p_app_idx = i;
 *p_mdl_cfg_idx =j;
 break;
 }
 }
 }

    BTIF_TRACE_EVENT("%s found=%d channel_id=0x%08x, app_idx=%d mdl_cfg_idx=%d  ",
                      __FUNCTION__,found,channel_id, i,j );
 return found;
}
