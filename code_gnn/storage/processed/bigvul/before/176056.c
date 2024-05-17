static BT_HDR* avrc_proc_vendor_command(uint8_t handle, uint8_t label,
                                        BT_HDR* p_pkt,
                                        tAVRC_MSG_VENDOR* p_msg) {
  BT_HDR* p_rsp = NULL;
 uint8_t* p_data;
 uint8_t* p_begin;
 uint8_t pkt_type;
 bool abort_frag = false;
  tAVRC_STS status = AVRC_STS_NO_ERROR;
  tAVRC_FRAG_CB* p_fcb;

  p_begin = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  p_data = p_begin + AVRC_VENDOR_HDR_SIZE;
  pkt_type = *(p_data + 1) & AVRC_PKT_TYPE_MASK;

 if (pkt_type != AVRC_PKT_SINGLE) {
  
    AVRC_TRACE_ERROR("commands must be in single packet pdu:0x%x", *p_data);
  
    status = AVRC_STS_BAD_CMD;
 }
  
 else if (avrc_cb.fcb[handle].frag_enabled) {
    p_fcb = &avrc_cb.fcb[handle];
 if (p_msg->company_id == AVRC_CO_METADATA) {
 switch (*p_data) {
 case AVRC_PDU_ABORT_CONTINUATION_RSP:
  
          abort_frag = true;
          p_begin = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
 *p_begin = (AVRC_RSP_ACCEPT & AVRC_CTYPE_MASK);
 if (*(p_data + 4) != p_fcb->frag_pdu) {
 *p_begin = (AVRC_RSP_REJ & AVRC_CTYPE_MASK);
 *(p_data + 4) = AVRC_STS_BAD_PARAM;
 } else {
            p_data = (p_begin + AVRC_VENDOR_HDR_SIZE + 2);
            UINT16_TO_BE_STREAM(p_data, 0);
            p_pkt->len = (p_data - p_begin);
 }
          AVCT_MsgReq(handle, label, AVCT_RSP, p_pkt);
          p_msg->hdr.opcode =
              AVRC_OP_DROP;  
 break;

 case AVRC_PDU_REQUEST_CONTINUATION_RSP:
 if (*(p_data + 4) == p_fcb->frag_pdu) {
            avrc_send_continue_frag(handle, label);
            p_msg->hdr.opcode = AVRC_OP_DROP_N_FREE;
 } else {
  
            AVRC_TRACE_ERROR(
 "%s continue pdu: 0x%x does not match the current pdu: 0x%x",
                __func__, *(p_data + 4), p_fcb->frag_pdu);
            status = AVRC_STS_BAD_PARAM;
            abort_frag = true;
 }
 break;

 default:
  
          abort_frag = true;
 }
 } else {
      abort_frag = true;
  
 }

 if (abort_frag) {
      osi_free_and_reset((void**)&p_fcb->p_fmsg);
      p_fcb->frag_enabled = false;
 }
 }

 if (status != AVRC_STS_NO_ERROR) {
    p_rsp = (BT_HDR*)osi_malloc(BT_DEFAULT_BUFFER_SIZE);
    p_rsp->offset = p_pkt->offset;
    p_data = (uint8_t*)(p_rsp + 1) + p_pkt->offset;
 *p_data++ = AVRC_RSP_REJ;
    p_data += AVRC_VENDOR_HDR_SIZE;  
 *p_data++ = 0;  
    UINT16_TO_BE_STREAM(p_data, 1);  
 *p_data++ = status;  
    p_rsp->len = AVRC_VENDOR_HDR_SIZE + 5;
 }

 return p_rsp;
}
