static void avrc_prep_end_frag(uint8_t handle) {
  tAVRC_FRAG_CB* p_fcb;
  BT_HDR* p_pkt_new;
 uint8_t *p_data, *p_orig_data;
 uint8_t rsp_type;

  AVRC_TRACE_DEBUG("%s", __func__);
  p_fcb = &avrc_cb.fcb[handle];

  
  p_orig_data = ((uint8_t*)(p_fcb->p_fmsg + 1) + p_fcb->p_fmsg->offset);
  rsp_type = ((*p_orig_data) & AVRC_CTYPE_MASK);

  p_pkt_new = p_fcb->p_fmsg;
  p_pkt_new->len -=
 (AVRC_MAX_CTRL_DATA_LEN - AVRC_VENDOR_HDR_SIZE - AVRC_MIN_META_HDR_SIZE);
  p_pkt_new->offset +=
 (AVRC_MAX_CTRL_DATA_LEN - AVRC_VENDOR_HDR_SIZE - AVRC_MIN_META_HDR_SIZE);
  p_data = (uint8_t*)(p_pkt_new + 1) + p_pkt_new->offset;
 *p_data++ = rsp_type;
 *p_data++ = (AVRC_SUB_PANEL << AVRC_SUBTYPE_SHIFT);
 *p_data++ = AVRC_OP_VENDOR;
  AVRC_CO_ID_TO_BE_STREAM(p_data, AVRC_CO_METADATA);
 *p_data++ = p_fcb->frag_pdu;
 *p_data++ = AVRC_PKT_END;

  
  UINT16_TO_BE_STREAM(
      p_data, (p_pkt_new->len - AVRC_VENDOR_HDR_SIZE - AVRC_MIN_META_HDR_SIZE));
}
