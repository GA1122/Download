BOOLEAN  btif_hl_save_mdl_cfg(UINT8 mdep_id, UINT8 item_idx,
                              tBTA_HL_MDL_CFG *p_mdl_cfg){
 btif_hl_mdl_cfg_t *p_mdl=NULL;
    BOOLEAN             success = FALSE;
 btif_hl_app_cb_t *p_acb;
 btif_hl_mcl_cb_t *p_mcb;
    UINT8               app_idx, mcl_idx, len;
 bt_status_t         bt_status;
 btif_hl_evt_cb_t    evt_param;
 int *p_channel_id;

    BTIF_TRACE_DEBUG("%s mdep_id=%d item_idx=%d, local_mdep_id=%d mdl_id=0x%x dch_mode=%d",
                      __FUNCTION__, mdep_id, item_idx, p_mdl_cfg->local_mdep_id,
                      p_mdl_cfg->mdl_id, p_mdl_cfg->dch_mode );

 if(btif_hl_find_app_idx_using_mdepId(mdep_id,&app_idx))
 {
        p_acb = BTIF_HL_GET_APP_CB_PTR(app_idx);
        p_mdl = BTIF_HL_GET_MDL_CFG_PTR(app_idx, item_idx);
        p_channel_id = BTIF_HL_GET_MDL_CFG_CHANNEL_ID_PTR(app_idx, item_idx);
 if (p_mdl)
 {
            memcpy(&p_mdl->base, p_mdl_cfg, sizeof(tBTA_HL_MDL_CFG));
 if (btif_hl_find_mcl_idx(app_idx, p_mdl->base.peer_bd_addr , &mcl_idx))
 {
                p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
 if (p_mcb->pcb.in_use)
 *p_channel_id = p_mcb->pcb.channel_id;
 else
 *p_channel_id = btif_hl_get_next_channel_id(p_acb->app_id);
                p_mdl->extra.mdep_cfg_idx = p_mcb->pcb.mdep_cfg_idx;
                p_mdl->extra.data_type = p_acb->sup_feature.mdep[p_mcb->pcb.mdep_cfg_idx].mdep_cfg.data_cfg[0].data_type;

 if (!btif_hl_find_peer_mdep_id(p_acb->app_id, p_mcb->bd_addr,
                                               p_acb->sup_feature.mdep[p_mcb->pcb.mdep_cfg_idx].mdep_cfg.mdep_role,
                                               p_acb->sup_feature.mdep[p_mcb->pcb.mdep_cfg_idx].mdep_cfg.data_cfg[0].data_type,
 &p_mdl->extra.peer_mdep_id))
 {
                    p_mdl->extra.peer_mdep_id = BTA_HL_INVALID_MDEP_ID;
 }
                BTIF_TRACE_DEBUG("%s app_idx=%d item_idx=%d mld_id=0x%x",
                                  __FUNCTION__, app_idx, item_idx, p_mdl->base.mdl_id);
                evt_param.update_mdl.app_idx = app_idx;
                len = sizeof(btif_hl_update_mdl_t);
                BTIF_TRACE_DEBUG("send BTIF_HL_UPDATE_MDL event app_idx=%d  ",app_idx);
 if ((bt_status = btif_transfer_context (btif_hl_proc_cb_evt, BTIF_HL_UPDATE_MDL,
 (char*) &evt_param, len, NULL)) == BT_STATUS_SUCCESS)
 {
                    success = TRUE;
 }
                ASSERTC(bt_status == BT_STATUS_SUCCESS, "context transfer failed", bt_status);
 }
 }
 }
    BTIF_TRACE_DEBUG("%s success=%d  ",__FUNCTION__, success );

 return success;
}
