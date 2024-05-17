uint16_t AVRC_Open(uint8_t* p_handle, tAVRC_CONN_CB* p_ccb,
 const RawAddress& peer_addr) {
 uint16_t status;
  tAVCT_CC cc;

  cc.p_ctrl_cback = avrc_ctrl_cback;  
  cc.p_msg_cback = avrc_msg_cback;  
  cc.pid = UUID_SERVCLASS_AV_REMOTE_CONTROL;  
  cc.role = p_ccb->conn;  
  cc.control = p_ccb->control;  

  status = AVCT_CreateConn(p_handle, &cc, peer_addr);
 if (status == AVCT_SUCCESS) {
    avrc_cb.ccb[*p_handle] = *p_ccb;
    memset(&avrc_cb.ccb_int[*p_handle], 0, sizeof(tAVRC_CONN_INT_CB));
    memset(&avrc_cb.fcb[*p_handle], 0, sizeof(tAVRC_FRAG_CB));
    memset(&avrc_cb.rcb[*p_handle], 0, sizeof(tAVRC_RASM_CB));
    avrc_cb.ccb_int[*p_handle].tle = alarm_new("avrcp.commandTimer");
    avrc_cb.ccb_int[*p_handle].cmd_q = fixed_queue_new(SIZE_MAX);
 }
  AVRC_TRACE_DEBUG("%s role: %d, control:%d status:%d, handle:%d", __func__,
                   cc.role, cc.control, status, *p_handle);

 return status;
}
