void rfc_send_nsc(tRFC_MCB* p_mcb) {
 uint8_t* p_data;
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(RFCOMM_CMD_BUF_SIZE);

  p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
  p_data = (uint8_t*)(p_buf + 1) + p_buf->offset;

 *p_data++ = RFCOMM_EA | RFCOMM_I_CR(false) | RFCOMM_MX_NSC;
 *p_data++ = RFCOMM_EA | (RFCOMM_MX_NSC_LEN << 1);

 *p_data++ = rfc_cb.rfc.rx_frame.ea |
 (rfc_cb.rfc.rx_frame.cr << RFCOMM_SHIFT_CR) |
              rfc_cb.rfc.rx_frame.type;

  
  p_buf->len = RFCOMM_MX_NSC_LEN + 2;

  rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}
