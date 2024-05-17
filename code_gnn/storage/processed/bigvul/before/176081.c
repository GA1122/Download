void rfc_send_buf_uih(tRFC_MCB* p_mcb, uint8_t dlci, BT_HDR* p_buf) {
 uint8_t* p_data;
 uint8_t cr = RFCOMM_CR(p_mcb->is_initiator, true);
 uint8_t credits;

  p_buf->offset -= RFCOMM_CTRL_FRAME_LEN;
 if (p_buf->len > 127) p_buf->offset--;

 if (dlci)
    credits = (uint8_t)p_buf->layer_specific;
 else
    credits = 0;

 if (credits) p_buf->offset--;

  p_data = (uint8_t*)(p_buf + 1) + p_buf->offset;

  
 *p_data++ = RFCOMM_EA | cr | (dlci << RFCOMM_SHIFT_DLCI);
 *p_data++ = RFCOMM_UIH | ((credits) ? RFCOMM_PF : 0);
 if (p_buf->len <= 127) {
 *p_data++ = RFCOMM_EA | (p_buf->len << 1);
    p_buf->len += 3;
 } else {
 *p_data++ = (p_buf->len & 0x7f) << 1;
 *p_data++ = p_buf->len >> RFCOMM_SHIFT_LENGTH2;
    p_buf->len += 4;
 }

 if (credits) {
 *p_data++ = credits;
    p_buf->len++;
 }

  p_data = (uint8_t*)(p_buf + 1) + p_buf->offset + p_buf->len++;

 *p_data = RFCOMM_UIH_FCS((uint8_t*)(p_buf + 1) + p_buf->offset, dlci);

 if (dlci == RFCOMM_MX_DLCI) {
    rfc_check_send_cmd(p_mcb, p_buf);
 } else {
    L2CA_DataWrite(p_mcb->lcid, p_buf);
 }
}
