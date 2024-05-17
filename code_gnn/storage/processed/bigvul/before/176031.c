void bta_av_rc_free_rsp(UNUSED_ATTR tBTA_AV_CB* p_cb, tBTA_AV_DATA* p_data) {
  osi_free_and_reset((void**)&p_data->api_meta_rsp.p_pkt);
}
