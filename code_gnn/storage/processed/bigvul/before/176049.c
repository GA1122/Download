static BT_HDR* avrc_copy_packet(BT_HDR* p_pkt, int rsp_pkt_len) {
 const int offset = MAX(AVCT_MSG_OFFSET, p_pkt->offset);
 const int pkt_len = MAX(rsp_pkt_len, p_pkt->len);
  BT_HDR* p_pkt_copy = (BT_HDR*)osi_malloc(BT_HDR_SIZE + offset + pkt_len);

  
  memcpy(p_pkt_copy, p_pkt, BT_HDR_SIZE);
  p_pkt_copy->offset = offset;
 uint8_t* p_data = avrc_get_data_ptr(p_pkt);
 uint8_t* p_data_copy = avrc_get_data_ptr(p_pkt_copy);
  memcpy(p_data_copy, p_data, p_pkt->len);

 return p_pkt_copy;
}
