static uint8_t avrc_proc_far_msg(uint8_t handle, uint8_t label, uint8_t cr,
                                 BT_HDR** pp_pkt, tAVRC_MSG_VENDOR* p_msg) {
  BT_HDR* p_pkt = *pp_pkt;
 uint8_t* p_data;
 uint8_t drop_code = 0;
 bool buf_overflow = false;
  BT_HDR* p_rsp = NULL;
  BT_HDR* p_cmd = NULL;
 bool req_continue = false;
  BT_HDR* p_pkt_new = NULL;
 uint8_t pkt_type;
  tAVRC_RASM_CB* p_rcb;
  tAVRC_NEXT_CMD avrc_cmd;
  tAVRC_STS status;

  p_data = (uint8_t*)(p_pkt + 1) + p_pkt->offset;

  
  p_data += AVRC_VENDOR_HDR_SIZE;

  pkt_type = *(p_data + 1) & AVRC_PKT_TYPE_MASK;
  AVRC_TRACE_DEBUG("pkt_type %d", pkt_type);
  p_rcb = &avrc_cb.rcb[handle];

  
 if (pkt_type == AVRC_PKT_SINGLE || pkt_type == AVRC_PKT_START) {
  
    p_rcb->rasm_offset = 0;
    osi_free_and_reset((void**)&p_rcb->p_rmsg);
 }

 if (pkt_type != AVRC_PKT_SINGLE && cr == AVCT_RSP) {
  
 if (pkt_type == AVRC_PKT_START) {
  
      p_rcb->rasm_pdu = *p_data;
      p_rcb->p_rmsg = (BT_HDR*)osi_malloc(BT_DEFAULT_BUFFER_SIZE);
  
      memcpy(p_rcb->p_rmsg, p_pkt, sizeof(BT_HDR));  

  
      memcpy((uint8_t*)(p_rcb->p_rmsg + 1),
 (uint8_t*)(p_pkt + 1) + p_pkt->offset, p_pkt->len);

  
      p_rcb->p_rmsg->offset = p_rcb->rasm_offset = 0;

  
      osi_free(p_pkt);
 *pp_pkt = p_rcb->p_rmsg;

  
      p_rcb->p_rmsg->offset += p_rcb->p_rmsg->len;
      req_continue = true;
 } else if (p_rcb->p_rmsg == NULL) {
  
      AVRC_TRACE_DEBUG(
 "Received a CONTINUE/END without no corresponding START \
 (or previous fragmented response was dropped)");
      drop_code = 5;
      osi_free(p_pkt);
 *pp_pkt = NULL;
 } else {
  
  
 uint16_t buf_len = BT_DEFAULT_BUFFER_SIZE - sizeof(BT_HDR);
  
      p_pkt->offset += (AVRC_VENDOR_HDR_SIZE + AVRC_MIN_META_HDR_SIZE);
      p_pkt->len -= (AVRC_VENDOR_HDR_SIZE + AVRC_MIN_META_HDR_SIZE);
  
 if ((p_rcb->p_rmsg->offset + p_pkt->len) > buf_len) {
        AVRC_TRACE_WARNING(
 "Fragmented message too big! - report the partial message");
        p_pkt->len = buf_len - p_rcb->p_rmsg->offset;
        pkt_type = AVRC_PKT_END;
        buf_overflow = true;
 }

  
      memcpy((uint8_t*)(p_rcb->p_rmsg + 1) + p_rcb->p_rmsg->offset,
 (uint8_t*)(p_pkt + 1) + p_pkt->offset, p_pkt->len);

 if (pkt_type == AVRC_PKT_END) {
        p_rcb->p_rmsg->offset = p_rcb->rasm_offset;
        p_rcb->p_rmsg->len += p_pkt->len;
        p_pkt_new = p_rcb->p_rmsg;
        p_rcb->rasm_offset = 0;
        p_rcb->p_rmsg = NULL;
        p_msg->p_vendor_data = (uint8_t*)(p_pkt_new + 1) + p_pkt_new->offset;
        p_msg->hdr.ctype = p_msg->p_vendor_data[0] & AVRC_CTYPE_MASK;
  
        p_msg->p_vendor_data += AVRC_VENDOR_HDR_SIZE;
        p_msg->vendor_len = p_pkt_new->len - AVRC_VENDOR_HDR_SIZE;
        p_data = p_msg->p_vendor_data + 1;  
 *p_data++ = AVRC_PKT_SINGLE;
        UINT16_TO_BE_STREAM(p_data,
 (p_msg->vendor_len - AVRC_MIN_META_HDR_SIZE));
        AVRC_TRACE_DEBUG("end frag:%d, total len:%d, offset:%d", p_pkt->len,
                         p_pkt_new->len, p_pkt_new->offset);
 } else {
        p_rcb->p_rmsg->offset += p_pkt->len;
        p_rcb->p_rmsg->len += p_pkt->len;
        p_pkt_new = NULL;
        req_continue = true;
 }
      osi_free(p_pkt);
 *pp_pkt = p_pkt_new;
 }
 }

 if (cr == AVCT_CMD) {
    p_rsp = avrc_proc_vendor_command(handle, label, *pp_pkt, p_msg);
 if (p_rsp) {
      AVCT_MsgReq(handle, label, AVCT_RSP, p_rsp);
      osi_free_and_reset((void**)pp_pkt);
      drop_code = 3;
 } else if (p_msg->hdr.opcode == AVRC_OP_DROP) {
      drop_code = 1;
 } else if (p_msg->hdr.opcode == AVRC_OP_DROP_N_FREE)
      drop_code = 4;

 } else if (cr == AVCT_RSP) {
 if (req_continue) {
      avrc_cmd.pdu = AVRC_PDU_REQUEST_CONTINUATION_RSP;
      drop_code = 2;
 } else if (buf_overflow) {
  
      avrc_cmd.pdu = AVRC_PDU_ABORT_CONTINUATION_RSP;
      drop_code = 4;
 } else {
 return drop_code;
 }
    avrc_cmd.status = AVRC_STS_NO_ERROR;
    avrc_cmd.target_pdu = p_rcb->rasm_pdu;

    tAVRC_COMMAND avrc_command;
    avrc_command.continu = avrc_cmd;
    status = AVRC_BldCommand(&avrc_command, &p_cmd);
 if (status == AVRC_STS_NO_ERROR) {
      AVRC_MsgReq(handle, (uint8_t)(label), AVRC_CMD_CTRL, p_cmd);
 }
 }

 return drop_code;
}
