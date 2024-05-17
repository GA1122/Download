static bt_status_t connect_channel(int app_id, bt_bdaddr_t *bd_addr, int mdep_cfg_index, int *channel_id){
    UINT8                   app_idx, mcl_idx;
 btif_hl_app_cb_t *p_acb = NULL;
 btif_hl_pending_chan_cb_t *p_pcb = NULL;
 btif_hl_mcl_cb_t *p_mcb=NULL;
 bt_status_t             status = BT_STATUS_SUCCESS;
    tBTA_HL_DCH_OPEN_PARAM  dch_open;
    BD_ADDR                 bda;
    UINT8 i;

    CHECK_BTHL_INIT();
    BTIF_TRACE_EVENT("%s", __FUNCTION__);
    btif_hl_display_calling_process_name();


 for (i=0; i<6; i++)
 {
        bda[i] = (UINT8) bd_addr->address[i];
 }
 if (btif_hl_find_app_idx(((UINT8)app_id), &app_idx))
 {
        p_acb = BTIF_HL_GET_APP_CB_PTR(app_idx);
 if (btif_hl_find_mcl_idx(app_idx, bda , &mcl_idx))
 {
            p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
 if (p_mcb->is_connected)
 {
                dch_open.ctrl_psm = p_mcb->ctrl_psm;
                dch_open.local_mdep_id = p_acb->sup_feature.mdep[mdep_cfg_index].mdep_id;
                BTIF_TRACE_DEBUG("connect_channel: app_idx =%d, mdep_cfg_indx =%d, mdep_id =%d app_id= %d", app_idx,
                                                mdep_cfg_index, dch_open.local_mdep_id, app_id);
 if (btif_hl_find_peer_mdep_id(p_acb->app_id, p_mcb->bd_addr,
                                              p_acb->sup_feature.mdep[mdep_cfg_index].mdep_cfg.mdep_role,
                                              p_acb->sup_feature.mdep[mdep_cfg_index].mdep_cfg.data_cfg[0].data_type, &dch_open.peer_mdep_id ))
 {
                    dch_open.local_cfg = p_acb->channel_type[mdep_cfg_index];
 if ((p_acb->sup_feature.mdep[mdep_cfg_index].mdep_cfg.mdep_role == BTA_HL_MDEP_ROLE_SOURCE)
 && !btif_hl_is_the_first_reliable_existed(app_idx,mcl_idx))
 {
                        dch_open.local_cfg = BTA_HL_DCH_CFG_RELIABLE;
 }
                    dch_open.sec_mask = (BTA_SEC_AUTHENTICATE | BTA_SEC_ENCRYPT);

 if( !btif_hl_dch_open(p_acb->app_id, bda, &dch_open,
                                              mdep_cfg_index, BTIF_HL_PEND_DCH_OP_OPEN, channel_id ))
 {
                        status = BT_STATUS_FAIL;
                        BTIF_TRACE_EVENT("%s loc0 status = BT_STATUS_FAIL", __FUNCTION__);
 }
 }
 else
 {
                    p_mcb->cch_oper = BTIF_HL_CCH_OP_MDEP_FILTERING;

                    p_pcb = BTIF_HL_GET_PCB_PTR(app_idx, mcl_idx);
                    p_pcb->in_use = TRUE;
                    p_pcb->mdep_cfg_idx = mdep_cfg_index;
                    memcpy(p_pcb->bd_addr, bda, sizeof(BD_ADDR));
                    p_pcb->op = BTIF_HL_PEND_DCH_OP_OPEN;
                    BTA_HlSdpQuery(app_id,p_acb->app_handle, bda);
 }
 }
 else
 {
                status = BT_STATUS_FAIL;
 }
 }
 else
 {
            p_acb->filter.num_elems =1;
            p_acb->filter.elem[0].data_type = p_acb->sup_feature.mdep[mdep_cfg_index].mdep_cfg.data_cfg[mdep_cfg_index].data_type;
 if (p_acb->sup_feature.mdep[mdep_cfg_index].mdep_cfg.mdep_role == BTA_HL_MDEP_ROLE_SINK)
                p_acb->filter.elem[0].peer_mdep_role = BTA_HL_MDEP_ROLE_SOURCE;
 else
                p_acb->filter.elem[0].peer_mdep_role = BTA_HL_MDEP_ROLE_SINK;

 if ( !btif_hl_cch_open(p_acb->app_id, bda, 0, mdep_cfg_index,
                                   BTIF_HL_PEND_DCH_OP_OPEN,
                                   channel_id))
 {
                status = BT_STATUS_FAIL;
 }
 }
 }
 else
 {
        status = BT_STATUS_FAIL;
 }

    BTIF_TRACE_DEBUG("%s status=%d channel_id=0x%08x", __FUNCTION__, status, *channel_id);

 return status;
}
