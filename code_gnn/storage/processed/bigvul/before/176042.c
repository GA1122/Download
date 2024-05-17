uint16_t AVRC_Close(uint8_t handle) {
  AVRC_TRACE_DEBUG("%s handle:%d", __func__, handle);
  avrc_flush_cmd_q(handle);
 return AVCT_RemoveConn(handle);
}
