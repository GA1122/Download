BOOLEAN btif_hl_cch_open(UINT8 app_id, BD_ADDR bd_addr, UINT16 ctrl_psm,
 int mdep_cfg_idx,
 btif_hl_pend_dch_op_t op, int *channel_id){

 btif_hl_app_cb_t *p_acb;
 btif_hl_mcl_cb_t *p_mcb;
 btif_hl_pending_chan_cb_t *p_pcb;
    UINT8                       app_idx, mcl_idx;
    BOOLEAN                     status = TRUE;

    BTIF_TRACE_DEBUG("%s app_id=%d ctrl_psm=%d mdep_cfg_idx=%d op=%d",
                      __FUNCTION__, app_id, ctrl_psm, mdep_cfg_idx, op);
    BTIF_TRACE_DEBUG("DB [%02x:%02x:%02x:%02x:%02x:%02x]",
                      bd_addr[0],  bd_addr[1],bd_addr[2],  bd_addr[3], bd_addr[4],  bd_addr[5]);

 if (btif_hl_find_app_idx(app_id, &app_idx))
 {
        p_acb = BTIF_HL_GET_APP_CB_PTR(app_idx);

 if (!btif_hl_find_mcl_idx(app_idx, bd_addr, &mcl_idx))
 {
 if (btif_hl_find_avail_mcl_idx(app_idx, &mcl_idx))
 {
                p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
                memset(p_mcb,0, sizeof(btif_hl_mcl_cb_t));
                p_mcb->in_use = TRUE;
                bdcpy(p_mcb->bd_addr, bd_addr);

 if (!ctrl_psm)
 {
                    p_mcb->cch_oper = BTIF_HL_CCH_OP_MDEP_FILTERING;
 }
 else
 {
                    p_mcb->cch_oper        = BTIF_HL_CCH_OP_MATCHED_CTRL_PSM;
                    p_mcb->req_ctrl_psm    = ctrl_psm;
 }

                p_pcb = BTIF_HL_GET_PCB_PTR(app_idx, mcl_idx);
                p_pcb->in_use = TRUE;
                p_pcb->mdep_cfg_idx = mdep_cfg_idx;
                memcpy(p_pcb->bd_addr, bd_addr, sizeof(BD_ADDR));
                p_pcb->op = op;

 switch (op)
 {
 case BTIF_HL_PEND_DCH_OP_OPEN:
 *channel_id       =
                        p_pcb->channel_id = (int) btif_hl_get_next_channel_id(app_id);
                        p_pcb->cb_state = BTIF_HL_CHAN_CB_STATE_CONNECTING_PENDING;
 break;
 case BTIF_HL_PEND_DCH_OP_DELETE_MDL:
                        p_pcb->channel_id =  p_acb->delete_mdl.channel_id;
                        p_pcb->cb_state = BTIF_HL_CHAN_CB_STATE_DESTROYED_PENDING;
 break;
 default:
 break;
 }
                BTA_HlSdpQuery(app_id,p_acb->app_handle, bd_addr);
 }
 else
 {
                status = FALSE;
                BTIF_TRACE_ERROR("Open CCH request discarded- No mcl cb");
 }
 }
 else
 {
            status = FALSE;
            BTIF_TRACE_ERROR("Open CCH request discarded- already in USE");
 }
 }
 else
 {
        status = FALSE;
        BTIF_TRACE_ERROR("Invalid app_id=%d", app_id);
 }

 if (channel_id)
 {
        BTIF_TRACE_DEBUG("status=%d channel_id=0x%08x", status, *channel_id);
 }
 else
 {
        BTIF_TRACE_DEBUG("status=%d ", status);
 }
 return status;
}
