void avrc_flush_cmd_q(uint8_t handle) {
  AVRC_TRACE_DEBUG("AVRC: Flushing command queue for handle=0x%02x", handle);
  avrc_cb.ccb_int[handle].flags &= ~AVRC_CB_FLAGS_RSP_PENDING;

  alarm_cancel(avrc_cb.ccb_int[handle].tle);
  fixed_queue_free(avrc_cb.ccb_int[handle].cmd_q, osi_free);
  avrc_cb.ccb_int[handle].cmd_q = NULL;
}
