static void bta_av_avrc_sdp_cback(UNUSED_ATTR uint16_t status) {
  BT_HDR* p_msg = (BT_HDR*)osi_malloc(sizeof(BT_HDR));

  p_msg->event = BTA_AV_SDP_AVRC_DISC_EVT;

  bta_sys_sendmsg(p_msg);
}
