void mca_ccb_hdl_rsp(tMCA_CCB* p_ccb, tMCA_CCB_EVT* p_data) {
  BT_HDR* p_pkt = &p_data->hdr;
 uint8_t* p;
  tMCA_CTRL evt_data;
 bool chk_mdl = false;
  tMCA_DCB* p_dcb;
  tMCA_RESULT result = MCA_BAD_HANDLE;
  tMCA_TC_TBL* p_tbl;

 if (p_ccb->p_tx_req) {
  
    p = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
    evt_data.hdr.op_code = *p++;
 if ((evt_data.hdr.op_code == 0) ||
 ((p_ccb->p_tx_req->op_code + 1) == evt_data.hdr.op_code)) {
      evt_data.rsp.rsp_code = *p++;
      mca_stop_timer(p_ccb);
      BE_STREAM_TO_UINT16(evt_data.hdr.mdl_id, p);
 if (evt_data.hdr.op_code == MCA_OP_MDL_CREATE_RSP) {
        evt_data.create_cfm.cfg = *p++;
        chk_mdl = true;
 } else if (evt_data.hdr.op_code == MCA_OP_MDL_RECONNECT_RSP)
        chk_mdl = true;

 if (chk_mdl) {
        p_dcb = mca_dcb_by_hdl(p_ccb->p_tx_req->dcb_idx);
 if (evt_data.rsp.rsp_code == MCA_RSP_SUCCESS) {
 if (evt_data.hdr.mdl_id != p_dcb->mdl_id) {
            MCA_TRACE_ERROR("peer's mdl_id=%d != our mdl_id=%d",
                            evt_data.hdr.mdl_id, p_dcb->mdl_id);
  
 if (evt_data.rsp.rsp_code == MCA_RSP_SUCCESS) {
              evt_data.rsp.rsp_code = MCA_RSP_BAD_MDL;
  
              p_ccb->status = MCA_CCB_STAT_PENDING;
              MCA_Abort(mca_ccb_to_hdl(p_ccb));
 }
 } else if (p_dcb->p_chnl_cfg) {
  
            BTM_SetSecurityLevel(true, "", BTM_SEC_SERVICE_MCAP_DATA,
                                 p_ccb->sec_mask, p_ccb->data_vpsm,
                                 BTM_SEC_PROTO_MCA, p_ccb->p_tx_req->dcb_idx);
            p_dcb->lcid = mca_l2c_open_req(p_ccb->peer_addr, p_ccb->data_vpsm,
                                           p_dcb->p_chnl_cfg);
 if (p_dcb->lcid) {
              p_tbl = mca_tc_tbl_dalloc(p_dcb);
 if (p_tbl) {
                p_tbl->state = MCA_TC_ST_CONN;
                p_ccb->status = MCA_CCB_STAT_PENDING;
                result = MCA_SUCCESS;
 }
 }
 } else {
  
            p_ccb->status = MCA_CCB_STAT_PENDING;
            result = MCA_SUCCESS;
 }
 }

 if (result != MCA_SUCCESS && p_dcb) {
          mca_dcb_dealloc(p_dcb, NULL);
 }
 }  

 if (p_ccb->status != MCA_CCB_STAT_PENDING)
        osi_free_and_reset((void**)&p_ccb->p_tx_req);
      mca_ccb_report_event(p_ccb, evt_data.hdr.op_code, &evt_data);
 }
  
 } else {
  
    MCA_TRACE_WARNING("dropping received rsp (not expecting a response)");
 }
  osi_free(p_data);
}
