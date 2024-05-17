void rfc_send_test(tRFC_MCB* p_mcb, bool is_command, BT_HDR* p_buf) {
  
 if (p_buf->offset < (L2CAP_MIN_OFFSET + RFCOMM_MIN_OFFSET + 2)) {
 uint8_t* p_src = (uint8_t*)(p_buf + 1) + p_buf->offset + p_buf->len - 1;
    BT_HDR* p_new_buf =
 (BT_HDR*)osi_malloc(p_buf->len + (L2CAP_MIN_OFFSET + RFCOMM_MIN_OFFSET +
 2 + sizeof(BT_HDR) + 1));

    p_new_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_MIN_OFFSET + 2;
    p_new_buf->len = p_buf->len;

 uint8_t* p_dest =
 (uint8_t*)(p_new_buf + 1) + p_new_buf->offset + p_new_buf->len - 1;

 for (uint16_t xx = 0; xx < p_buf->len; xx++) *p_dest-- = *p_src--;

    osi_free(p_buf);
    p_buf = p_new_buf;
 }

  
  p_buf->offset -= 2;
 uint8_t* p_data = (uint8_t*)(p_buf + 1) + p_buf->offset;

 *p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_TEST;
 *p_data++ = RFCOMM_EA | (p_buf->len << 1);

  p_buf->len += 2;

  rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}
