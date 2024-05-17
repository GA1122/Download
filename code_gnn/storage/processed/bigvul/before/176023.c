void bta_av_rc_browse_opened(tBTA_AV_DATA* p_data) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
  tBTA_AV_RC_CONN_CHG* p_msg = (tBTA_AV_RC_CONN_CHG*)p_data;
  tBTA_AV_RC_BROWSE_OPEN rc_browse_open;

  LOG_INFO(LOG_TAG, "%s: peer_addr: %s rc_handle:%d", __func__,
           p_msg->peer_addr.ToString().c_str(), p_msg->handle);

  rc_browse_open.status = BTA_AV_SUCCESS;
  rc_browse_open.rc_handle = p_msg->handle;
  rc_browse_open.peer_addr = p_msg->peer_addr;

  tBTA_AV bta_av_data;
  bta_av_data.rc_browse_open = rc_browse_open;
 (*p_cb->p_cback)(BTA_AV_RC_BROWSE_OPEN_EVT, &bta_av_data);
}
