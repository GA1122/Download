static BOOLEAN btif_hl_proc_dch_open_cfm(tBTA_HL *p_data)

{
 btif_hl_mdl_cb_t *p_dcb;
 btif_hl_pending_chan_cb_t *p_pcb;
    UINT8                    app_idx, mcl_idx, mdl_idx, mdep_cfg_idx;
    BOOLEAN                  status = FALSE;
    BOOLEAN                  close_dch = FALSE;

    BTIF_TRACE_DEBUG("%s", __FUNCTION__);

    btif_hl_find_app_idx_using_mdepId(p_data->dch_open_cfm.local_mdep_id,&app_idx);

 if (btif_hl_find_mcl_idx_using_app_idx(p_data->dch_open_cfm.mcl_handle, app_idx, &mcl_idx ))
 {
        BTIF_HL_GET_APP_CB_PTR(app_idx);
        BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
        p_pcb = BTIF_HL_GET_PCB_PTR(app_idx, mcl_idx);

 if (btif_hl_find_avail_mdl_idx(app_idx, mcl_idx, &mdl_idx))
 {
            p_dcb = BTIF_HL_GET_MDL_CB_PTR(app_idx, mcl_idx, mdl_idx);

 if (btif_hl_find_mdep_cfg_idx(app_idx, p_data->dch_open_cfm.local_mdep_id, &mdep_cfg_idx))
 {
                p_dcb->in_use               = TRUE;
                p_dcb->mdl_handle           = p_data->dch_open_cfm.mdl_handle;
                p_dcb->local_mdep_cfg_idx   = mdep_cfg_idx;
                p_dcb->local_mdep_id        = p_data->dch_open_cfm.local_mdep_id;
                p_dcb->mdl_id               = p_data->dch_open_cfm.mdl_id;
                p_dcb->dch_mode             = p_data->dch_open_cfm.dch_mode;
                p_dcb->is_the_first_reliable= p_data->dch_open_cfm.first_reliable;
                p_dcb->mtu                  = p_data->dch_open_cfm.mtu;
                p_dcb->channel_id           = p_pcb->channel_id;

                BTIF_TRACE_DEBUG("app_idx=%d mcl_idx=%d mdl_idx=%d",  app_idx, mcl_idx, mdl_idx  );
                btif_hl_send_setup_connecting_cb(app_idx, mcl_idx);
 if (btif_hl_create_socket(app_idx, mcl_idx, mdl_idx))
 {
                    status = TRUE;
                    BTIF_TRACE_DEBUG("app_idx=%d mcl_idx=%d mdl_idx=%d p_dcb->channel_id=0x%08x",
                                      app_idx, mcl_idx, mdl_idx, p_dcb->channel_id);
                    btif_hl_clean_pcb(p_pcb);
 }
 else
 {
                    BTIF_TRACE_ERROR("Unable to create socket");
                    close_dch = TRUE;
 }
 }
 else
 {
                BTIF_TRACE_ERROR("INVALID_LOCAL_MDEP_ID mdep_id=%d",p_data->dch_open_cfm.local_mdep_id);
                close_dch = TRUE;
 }

 if (close_dch)
 {
                btif_hl_clean_mdl_cb(p_dcb);
                BTA_HlDchClose(p_data->dch_open_cfm.mdl_handle);
 }
 }
 }

 return status;
}
