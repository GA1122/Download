void bta_av_rc_browse_closed(tBTA_AV_DATA* p_data) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
  tBTA_AV_RC_CONN_CHG* p_msg = (tBTA_AV_RC_CONN_CHG*)p_data;
  tBTA_AV_RC_BROWSE_CLOSE rc_browse_close;

  LOG_INFO(LOG_TAG, "%s: peer_addr: %s rc_handle:%d", __func__,
           p_msg->peer_addr.ToString().c_str(), p_msg->handle);

  rc_browse_close.rc_handle = p_msg->handle;
  rc_browse_close.peer_addr = p_msg->peer_addr;

  tBTA_AV bta_av_data;
  bta_av_data.rc_browse_close = rc_browse_close;
 (*p_cb->p_cback)(BTA_AV_RC_BROWSE_CLOSE_EVT, &bta_av_data);
}
