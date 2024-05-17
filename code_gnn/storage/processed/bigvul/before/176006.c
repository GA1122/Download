void bta_av_api_disconnect(tBTA_AV_DATA* p_data) {
  AVDT_DisconnectReq(p_data->api_discnt.bd_addr, bta_av_conn_cback);
  alarm_cancel(bta_av_cb.link_signalling_timer);
}
