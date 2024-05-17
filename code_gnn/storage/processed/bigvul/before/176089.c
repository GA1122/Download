void rfc_send_pn(tRFC_MCB* p_mcb, uint8_t dlci, bool is_command, uint16_t mtu,
 uint8_t cl, uint8_t k) {
 uint8_t* p_data;
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(RFCOMM_CMD_BUF_SIZE);

  p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
  p_data = (uint8_t*)(p_buf + 1) + p_buf->offset;

 *p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_PN;
 *p_data++ = RFCOMM_EA | (RFCOMM_MX_PN_LEN << 1);

 *p_data++ = dlci;
 *p_data++ = RFCOMM_PN_FRAM_TYPE_UIH | cl;

  
 if (is_command)
 *p_data++ = RFCOMM_PN_PRIORITY_0;
 else
 *p_data++ = rfc_cb.rfc.rx_frame.u.pn.priority;

 *p_data++ = RFCOMM_T1_DSEC;
 *p_data++ = mtu & 0xFF;
 *p_data++ = mtu >> 8;
 *p_data++ = RFCOMM_N2;
 *p_data = k;

  
  p_buf->len = RFCOMM_MX_PN_LEN + 2;

  rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}
