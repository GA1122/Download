void rfc_send_rls(tRFC_MCB* p_mcb, uint8_t dlci, bool is_command,
 uint8_t status) {
 uint8_t* p_data;
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(RFCOMM_CMD_BUF_SIZE);

  p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
  p_data = (uint8_t*)(p_buf + 1) + p_buf->offset;

 *p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_RLS;
 *p_data++ = RFCOMM_EA | (RFCOMM_MX_RLS_LEN << 1);

 *p_data++ = RFCOMM_EA | RFCOMM_CR_MASK | (dlci << RFCOMM_SHIFT_DLCI);
 *p_data++ = RFCOMM_RLS_ERROR | status;

  
  p_buf->len = RFCOMM_MX_RLS_LEN + 2;

  rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}
