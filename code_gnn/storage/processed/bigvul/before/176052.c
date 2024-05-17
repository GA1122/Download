static uint8_t* avrc_get_data_ptr(BT_HDR* p_pkt) {
 return (uint8_t*)(p_pkt + 1) + p_pkt->offset;
}
