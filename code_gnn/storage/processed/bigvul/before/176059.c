void avrc_send_next_vendor_cmd(uint8_t handle) {
  BT_HDR* p_next_cmd;
 uint8_t next_label;

 while ((p_next_cmd = (BT_HDR*)fixed_queue_try_dequeue(
              avrc_cb.ccb_int[handle].cmd_q)) != NULL) {
    p_next_cmd->event &= 0xFF;  
    next_label = (p_next_cmd->layer_specific) >> 8;  
    p_next_cmd->layer_specific &= 0xFF;  

    AVRC_TRACE_DEBUG(
 "AVRC: Dequeuing command 0x%08x (handle=0x%02x, label=0x%02x)",
        p_next_cmd, handle, next_label);

  
 if ((AVCT_MsgReq(handle, next_label, AVCT_CMD, p_next_cmd)) ==
        AVCT_SUCCESS) {
  
      avrc_start_cmd_timer(handle, next_label, AVRC_MSG_MASK_IS_VENDOR_CMD);
 return;
 }
 }

 if (p_next_cmd == NULL) {
  
    avrc_cb.ccb_int[handle].flags &= ~AVRC_CB_FLAGS_RSP_PENDING;
 }
}
