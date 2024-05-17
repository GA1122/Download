BOOLEAN btif_hl_dch_open(UINT8 app_id, BD_ADDR bd_addr,
                         tBTA_HL_DCH_OPEN_PARAM *p_dch_open_api,
 int mdep_cfg_idx,
 btif_hl_pend_dch_op_t op, int *channel_id){
 btif_hl_app_cb_t *p_acb;
 btif_hl_mcl_cb_t *p_mcb;
 btif_hl_pending_chan_cb_t *p_pcb;
    UINT8                       app_idx, mcl_idx;
    BOOLEAN                     status = FALSE;
    tBTA_HL_MDL_ID              mdl_id;
    tBTA_HL_DCH_RECONNECT_PARAM reconnect_param;

    BTIF_TRACE_DEBUG("%s app_id=%d ",
                      __FUNCTION__, app_id );
    BTIF_TRACE_DEBUG("DB [%02x:%02x:%02x:%02x:%02x:%02x]",
                      bd_addr[0],  bd_addr[1],bd_addr[2],  bd_addr[3], bd_addr[4],  bd_addr[5]);

 if (btif_hl_find_app_idx(app_id, &app_idx))
 {
 if (btif_hl_find_mcl_idx(app_idx, bd_addr , &mcl_idx))
 {
            p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);

            p_pcb = BTIF_HL_GET_PCB_PTR(app_idx, mcl_idx);
 if (!p_pcb->in_use)
 {
                p_mcb->req_ctrl_psm = p_dch_open_api->ctrl_psm;

                p_pcb->in_use = TRUE;
 *channel_id       =
                p_pcb->channel_id = (int) btif_hl_get_next_channel_id(app_id);
                p_pcb->cb_state = BTIF_HL_CHAN_CB_STATE_CONNECTING_PENDING;
                p_pcb->mdep_cfg_idx = mdep_cfg_idx;
                memcpy(p_pcb->bd_addr, bd_addr, sizeof(BD_ADDR));
                p_pcb->op = op;

 if (p_mcb->sdp.num_recs)
 {
 if (p_mcb->valid_sdp_idx)
 {
                        p_dch_open_api->ctrl_psm  = p_mcb->ctrl_psm;
 }

 if (!btif_hl_is_reconnect_possible(app_idx, mcl_idx, mdep_cfg_idx, p_dch_open_api, &mdl_id ))
 {

                        BTIF_TRACE_DEBUG("Issue DCH open" );
                        BTA_HlDchOpen(p_mcb->mcl_handle, p_dch_open_api);
 }
 else
 {
                        reconnect_param.ctrl_psm = p_mcb->ctrl_psm;
                        reconnect_param.mdl_id = mdl_id;;
                        BTIF_TRACE_DEBUG("Issue Reconnect ctrl_psm=0x%x mdl_id=0x%x",reconnect_param.ctrl_psm, reconnect_param.mdl_id   );
                        BTA_HlDchReconnect(p_mcb->mcl_handle, &reconnect_param);
 }

                    status = TRUE;
 }
 else
 {
                    p_acb = BTIF_HL_GET_APP_CB_PTR(app_idx);
                    p_mcb->cch_oper = BTIF_HL_CCH_OP_DCH_OPEN;
                    BTA_HlSdpQuery(app_id,p_acb->app_handle, bd_addr);
                    status = TRUE;
 }
 }
 }
 }

    BTIF_TRACE_DEBUG("status=%d ", status);
 return status;
}
