BOOLEAN btif_hl_find_channel_id_using_mdl_id(UINT8 app_idx, tBTA_HL_MDL_ID mdl_id,
 int *p_channel_id){
 btif_hl_app_cb_t *p_acb;
 btif_hl_mdl_cfg_t *p_mdl;
    BOOLEAN found=FALSE;
    UINT8 j=0;
 int mdl_cfg_channel_id;
    p_acb =BTIF_HL_GET_APP_CB_PTR(app_idx);
 if (p_acb && p_acb->in_use)
 {
 for (j=0; j< BTA_HL_NUM_MDL_CFGS; j++)
 {
                    p_mdl =BTIF_HL_GET_MDL_CFG_PTR(app_idx,j);
                    mdl_cfg_channel_id = *(BTIF_HL_GET_MDL_CFG_CHANNEL_ID_PTR(app_idx,j));
 if ( p_mdl->base.active && (p_mdl->base.mdl_id == mdl_id))
 {
                            found = TRUE;
 *p_channel_id = mdl_cfg_channel_id;
 break;
 }
 }
 }
    BTIF_TRACE_EVENT("%s found=%d channel_id=0x%08x, mdl_id=0x%x app_idx=%d mdl_cfg_idx=%d  ",
                    __FUNCTION__,found,*p_channel_id,mdl_id, app_idx,j );
 return found;
}
