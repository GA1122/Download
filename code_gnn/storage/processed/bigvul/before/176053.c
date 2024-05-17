static BT_HDR* avrc_pass_msg(tAVRC_MSG_PASS* p_msg) {
  CHECK(p_msg != NULL);
  CHECK(AVRC_CMD_BUF_SIZE > (AVRC_MIN_CMD_LEN + p_msg->pass_len));

  BT_HDR* p_cmd = (BT_HDR*)osi_malloc(AVRC_CMD_BUF_SIZE);
  p_cmd->offset = AVCT_MSG_OFFSET;
  p_cmd->layer_specific = AVCT_DATA_CTRL;

 uint8_t* p_data = (uint8_t*)(p_cmd + 1) + p_cmd->offset;
 *p_data++ = (p_msg->hdr.ctype & AVRC_CTYPE_MASK);
 *p_data++ = (AVRC_SUB_PANEL << AVRC_SUBTYPE_SHIFT);  
 *p_data++ = AVRC_OP_PASS_THRU;
 *p_data = (AVRC_PASS_OP_ID_MASK & p_msg->op_id);
 if (p_msg->state) *p_data |= AVRC_PASS_STATE_MASK;
  p_data++;

 if (p_msg->op_id == AVRC_ID_VENDOR) {
 *p_data++ = p_msg->pass_len;
 if (p_msg->pass_len && p_msg->p_pass_data) {
      memcpy(p_data, p_msg->p_pass_data, p_msg->pass_len);
      p_data += p_msg->pass_len;
 }
 } else {
  
 *p_data++ = 0;
 }
  p_cmd->len = (uint16_t)(p_data - (uint8_t*)(p_cmd + 1) - p_cmd->offset);

 return p_cmd;
}
