void bta_av_disable(tBTA_AV_CB* p_cb, UNUSED_ATTR tBTA_AV_DATA* p_data) {
  BT_HDR hdr;
 uint16_t xx;

  p_cb->disabling = true;

  bta_av_close_all_rc(p_cb);

  osi_free_and_reset((void**)&p_cb->p_disc_db);

  
 for (xx = 0; xx < BTA_AV_NUM_STRS; xx++) {
 if (p_cb->p_scb[xx] != NULL) {
      hdr.layer_specific = xx + 1;
      bta_av_api_deregister((tBTA_AV_DATA*)&hdr);
 }
 }

  alarm_free(p_cb->link_signalling_timer);
  p_cb->link_signalling_timer = NULL;
  alarm_free(p_cb->accept_signalling_timer);
  p_cb->accept_signalling_timer = NULL;
}
