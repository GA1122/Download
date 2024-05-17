static BOOLEAN btif_hl_proc_sdp_query_cfm(tBTA_HL *p_data){
 btif_hl_app_cb_t *p_acb;
 btif_hl_mcl_cb_t *p_mcb;
    tBTA_HL_SDP                     *p_sdp;
    tBTA_HL_CCH_OPEN_PARAM          open_param;
    UINT8                           app_idx, mcl_idx, sdp_idx = 0;
    UINT8                           num_recs, i, num_mdeps, j;
 btif_hl_cch_op_t                old_cch_oper;
    BOOLEAN                         status =FALSE;
 btif_hl_pending_chan_cb_t *p_pcb;

    BTIF_TRACE_DEBUG("%s", __FUNCTION__);

    p_sdp = p_data->sdp_query_cfm.p_sdp;
    num_recs = p_sdp->num_recs;

    BTIF_TRACE_DEBUG("num of SDP records=%d",num_recs);
 for (i=0; i<num_recs; i++)
 {
        BTIF_TRACE_DEBUG("rec_idx=%d ctrl_psm=0x%x data_psm=0x%x",
 (i+1),p_sdp->sdp_rec[i].ctrl_psm, p_sdp->sdp_rec[i].data_psm);
        BTIF_TRACE_DEBUG("MCAP supported procedures=0x%x",p_sdp->sdp_rec[i].mcap_sup_proc);
        num_mdeps = p_sdp->sdp_rec[i].num_mdeps;
        BTIF_TRACE_DEBUG("num of mdeps =%d",num_mdeps);
 for (j=0; j< num_mdeps; j++)
 {
            BTIF_TRACE_DEBUG("mdep_idx=%d mdep_id=0x%x data_type=0x%x mdep_role=0x%x",
 (j+1),
                              p_sdp->sdp_rec[i].mdep_cfg[j].mdep_id,
                              p_sdp->sdp_rec[i].mdep_cfg[j].data_type,
                              p_sdp->sdp_rec[i].mdep_cfg[j].mdep_role );
 }
 }

 if (btif_hl_find_app_idx_using_app_id(p_data->sdp_query_cfm.app_id, &app_idx))
 {
            p_acb = BTIF_HL_GET_APP_CB_PTR(app_idx);

 if (btif_hl_find_mcl_idx(app_idx, p_data->sdp_query_cfm.bd_addr, &mcl_idx))
 {
                p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
 if (p_mcb->cch_oper != BTIF_HL_CCH_OP_NONE)
 {
                    memcpy(&p_mcb->sdp, p_sdp, sizeof(tBTA_HL_SDP));
                    old_cch_oper = p_mcb->cch_oper;
                    p_mcb->cch_oper = BTIF_HL_CCH_OP_NONE;

 switch (old_cch_oper)
 {
 case BTIF_HL_CCH_OP_MDEP_FILTERING:
                            status = btif_hl_find_sdp_idx_using_mdep_filter(app_idx,
                                                                    mcl_idx, &sdp_idx);
 break;
 default:
 break;
 }

 if (status)
 {
                        p_mcb->sdp_idx       = sdp_idx;
                        p_mcb->valid_sdp_idx = TRUE;
                        p_mcb->ctrl_psm      = p_mcb->sdp.sdp_rec[sdp_idx].ctrl_psm;

 switch (old_cch_oper)
 {
 case BTIF_HL_CCH_OP_MDEP_FILTERING:
                                p_pcb = BTIF_HL_GET_PCB_PTR(app_idx, mcl_idx);
 if (p_pcb->in_use)
 {
 if (!p_pcb->abort_pending)
 {
 switch (p_pcb->op)
 {
 case BTIF_HL_PEND_DCH_OP_OPEN:
                                                btif_hl_send_setup_connecting_cb(app_idx, mcl_idx);
 break;
 case BTIF_HL_PEND_DCH_OP_DELETE_MDL:
 default:
 break;
 }
                                        open_param.ctrl_psm = p_mcb->ctrl_psm;
                                        bdcpy(open_param.bd_addr, p_mcb->bd_addr);
                                        open_param.sec_mask =
 (BTA_SEC_AUTHENTICATE | BTA_SEC_ENCRYPT);
                                        BTA_HlCchOpen(p_acb->app_id,p_acb->app_handle, &open_param);
 }
 else
 {
                                        BTIF_TRACE_DEBUG("channel abort pending");
 }
 }
 break;

 case BTIF_HL_CCH_OP_DCH_OPEN:
                                status = btif_hl_proc_pending_op(app_idx,mcl_idx);
 break;

 default:
                                BTIF_TRACE_ERROR("Invalid CCH oper %d", old_cch_oper);
 break;
 }
 }
 else
 {
                        BTIF_TRACE_ERROR("Can not find SDP idx discard CCH Open request");
 }
 }
 }
 }
 return status;
}
