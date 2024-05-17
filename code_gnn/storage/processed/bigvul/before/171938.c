static bt_status_t destroy_channel(int channel_id){
    UINT8 app_idx, mcl_idx, mdl_cfg_idx, mdep_cfg_idx = 0;
 bt_status_t status = BT_STATUS_SUCCESS;
 btif_hl_mdl_cfg_t *p_mdl;
 btif_hl_mcl_cb_t *p_mcb;
 btif_hl_app_cb_t *p_acb;

    CHECK_BTHL_INIT();
    BTIF_TRACE_EVENT("%s channel_id=0x%08x", __FUNCTION__, channel_id);
    btif_hl_display_calling_process_name();


 if (btif_hl_if_channel_setup_pending(channel_id, &app_idx, &mcl_idx))
 {
        btif_hl_dch_abort(app_idx, mcl_idx);
 }
 else
 {
 if (btif_hl_find_mdl_cfg_idx_using_channel_id(channel_id, &app_idx, &mdl_cfg_idx))
 {
            p_acb = BTIF_HL_GET_APP_CB_PTR(app_idx);
 if (!p_acb->delete_mdl.active)
 {
                p_mdl =BTIF_HL_GET_MDL_CFG_PTR(app_idx, mdl_cfg_idx);
                p_acb->delete_mdl.active = TRUE;
                p_acb->delete_mdl.mdl_id = p_mdl->base.mdl_id;
                p_acb->delete_mdl.channel_id = channel_id;
                p_acb->delete_mdl.mdep_cfg_idx = p_mdl->extra.mdep_cfg_idx;
                memcpy(p_acb->delete_mdl.bd_addr, p_mdl->base.peer_bd_addr,sizeof(BD_ADDR));

 if (btif_hl_find_mcl_idx(app_idx, p_mdl->base.peer_bd_addr, &mcl_idx))
 {
                    p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
 if (p_mcb->is_connected)
 {
                        BTIF_TRACE_DEBUG("calling BTA_HlDeleteMdl mdl_id=%d",p_acb->delete_mdl.mdl_id );
                        BTA_HlDeleteMdl(p_mcb->mcl_handle, p_acb->delete_mdl.mdl_id);
 }
 else
 {
                        status = BT_STATUS_FAIL;
 }
 }
 else
 {
                    BTIF_TRACE_DEBUG("btif_hl_delete_mdl calling btif_hl_cch_open" );
                    mdep_cfg_idx = p_mdl->extra.mdep_cfg_idx;
                    p_acb->filter.num_elems =1;
                    p_acb->filter.elem[0].data_type = p_acb->sup_feature.mdep[mdep_cfg_idx].mdep_cfg.data_cfg[mdep_cfg_idx].data_type;
 if (p_acb->sup_feature.mdep[mdep_cfg_idx].mdep_cfg.mdep_role == BTA_HL_MDEP_ROLE_SINK)
                        p_acb->filter.elem[0].peer_mdep_role = BTA_HL_MDEP_ROLE_SOURCE;
 else
                        p_acb->filter.elem[0].peer_mdep_role = BTA_HL_MDEP_ROLE_SINK;
 if (btif_hl_cch_open(p_acb->app_id, p_acb->delete_mdl.bd_addr, 0,
                                         mdep_cfg_idx,
                                         BTIF_HL_PEND_DCH_OP_DELETE_MDL, NULL))
 {
                        status = BT_STATUS_FAIL;
 }
 }

 if (  status == BT_STATUS_FAIL)
 {
  
                    btif_hl_clean_delete_mdl(&p_acb->delete_mdl);
 }
 }
 else
 {
                status = BT_STATUS_BUSY;
 }
 }
 else
 {
            status = BT_STATUS_FAIL;
 }

 }
 return status;
}
