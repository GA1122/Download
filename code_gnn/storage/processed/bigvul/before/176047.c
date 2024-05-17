uint16_t AVRC_PassCmd(uint8_t handle, uint8_t label, tAVRC_MSG_PASS* p_msg) {
  BT_HDR* p_buf;
 uint16_t status = AVRC_NO_RESOURCES;
 if (!p_msg) return AVRC_BAD_PARAM;

  p_msg->hdr.ctype = AVRC_CMD_CTRL;
  p_buf = avrc_pass_msg(p_msg);
 if (p_buf) {
    status = AVCT_MsgReq(handle, label, AVCT_CMD, p_buf);
 if (status == AVCT_SUCCESS) {
  
      avrc_start_cmd_timer(handle, label, 0);
 }
 }
 return (status);
}
