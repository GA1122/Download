static uint16_t avrc_send_continue_frag(uint8_t handle, uint8_t label) {
  tAVRC_FRAG_CB* p_fcb;
  BT_HDR *p_pkt_old, *p_pkt;
 uint8_t *p_old, *p_data;
 uint8_t cr = AVCT_RSP;

  p_fcb = &avrc_cb.fcb[handle];
  p_pkt = p_fcb->p_fmsg;

  AVRC_TRACE_DEBUG("%s handle = %u label = %u len = %d", __func__, handle,
                   label, p_pkt->len);
 if (p_pkt->len > AVRC_MAX_CTRL_DATA_LEN) {
 int offset_len = MAX(AVCT_MSG_OFFSET, p_pkt->offset);
    p_pkt_old = p_fcb->p_fmsg;
    p_pkt = (BT_HDR*)osi_malloc(AVRC_PACKET_LEN + offset_len + BT_HDR_SIZE);
    p_pkt->len = AVRC_MAX_CTRL_DATA_LEN;
    p_pkt->offset = AVCT_MSG_OFFSET;
    p_pkt->layer_specific = p_pkt_old->layer_specific;
    p_pkt->event = p_pkt_old->event;
    p_old = (uint8_t*)(p_pkt_old + 1) + p_pkt_old->offset;
    p_data = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
    memcpy(p_data, p_old, AVRC_MAX_CTRL_DATA_LEN);
  
    p_data += AVRC_VENDOR_HDR_SIZE;
    p_data++;  
 *p_data++ = AVRC_PKT_CONTINUE;
  
    UINT16_TO_BE_STREAM(p_data,
 (AVRC_MAX_CTRL_DATA_LEN - AVRC_VENDOR_HDR_SIZE - 4));

  
    avrc_prep_end_frag(handle);
 } else {
  
    p_fcb->frag_enabled = false;
    p_fcb->p_fmsg = NULL;
 }
 return AVCT_MsgReq(handle, label, cr, p_pkt);
}
