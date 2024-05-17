static void bta_av_rc_ctrl_cback(uint8_t handle, uint8_t event,
                                 UNUSED_ATTR uint16_t result,
 const RawAddress* peer_addr) {
 uint16_t msg_event = 0;

  APPL_TRACE_EVENT("%s: handle: %d event=0x%x", __func__, handle, event);
 if (event == AVRC_OPEN_IND_EVT) {
  

    msg_event = BTA_AV_AVRC_OPEN_EVT;
 } else if (event == AVRC_CLOSE_IND_EVT) {
    msg_event = BTA_AV_AVRC_CLOSE_EVT;
 } else if (event == AVRC_BROWSE_OPEN_IND_EVT) {
    msg_event = BTA_AV_AVRC_BROWSE_OPEN_EVT;
 } else if (event == AVRC_BROWSE_CLOSE_IND_EVT) {
    msg_event = BTA_AV_AVRC_BROWSE_CLOSE_EVT;
 }

 if (msg_event) {
    tBTA_AV_RC_CONN_CHG* p_msg =
 (tBTA_AV_RC_CONN_CHG*)osi_malloc(sizeof(tBTA_AV_RC_CONN_CHG));
    p_msg->hdr.event = msg_event;
    p_msg->handle = handle;
 if (peer_addr) p_msg->peer_addr = *peer_addr;
    bta_sys_sendmsg(p_msg);
 }
}
