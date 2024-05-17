uint16_t AVRC_PassRsp(uint8_t handle, uint8_t label, tAVRC_MSG_PASS* p_msg) {
  BT_HDR* p_buf;
 if (!p_msg) return AVRC_BAD_PARAM;

  p_buf = avrc_pass_msg(p_msg);
 if (p_buf) return AVCT_MsgReq(handle, label, AVCT_RSP, p_buf);
 return AVRC_NO_RESOURCES;
}
