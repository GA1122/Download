void bta_av_sig_chg(tBTA_AV_DATA* p_data) {
 uint16_t event = p_data->str_msg.hdr.layer_specific;
  tBTA_AV_CB* p_cb = &bta_av_cb;
 uint32_t xx;
 uint8_t mask;
  tBTA_AV_LCB* p_lcb = NULL;

  APPL_TRACE_DEBUG("%s: event: %d", __func__, event);
 if (event == AVDT_CONNECT_IND_EVT) {
    APPL_TRACE_DEBUG("%s: AVDT_CONNECT_IND_EVT: peer %s", __func__,
                     p_data->str_msg.bd_addr.ToString().c_str());

    p_lcb = bta_av_find_lcb(p_data->str_msg.bd_addr, BTA_AV_LCB_FIND);
 if (!p_lcb) {
  
      xx = bta_av_find_lcb_index_by_scb_and_address(p_data->str_msg.bd_addr);

  
 if (xx >= BTA_AV_NUM_LINKS) {
  
  
        APPL_TRACE_ERROR("%s: av scb not available for avdt connection for %s",
                         __func__, p_data->str_msg.bd_addr.ToString().c_str());
        AVDT_DisconnectReq(p_data->str_msg.bd_addr, NULL);
 return;
 }
      LOG_INFO(LOG_TAG,
 "%s: AVDT_CONNECT_IND_EVT: peer %s selected lcb_index %d",
               __func__, p_data->str_msg.bd_addr.ToString().c_str(), xx);

      tBTA_AV_SCB* p_scb = p_cb->p_scb[xx];
      mask = 1 << xx;
      p_lcb = &p_cb->lcb[xx];
      p_lcb->lidx = xx + 1;
      p_lcb->addr = p_data->str_msg.bd_addr;
      p_lcb->conn_msk = 0;  
  
 if (p_cb->features & BTA_AV_FEAT_RCTG) {
        bta_av_rc_create(p_cb, AVCT_ACP, 0, p_lcb->lidx);
 }
  
      p_cb->conn_lcb |= mask;  
      APPL_TRACE_DEBUG("%s: start sig timer %d", __func__, p_data->hdr.offset);
 if (p_data->hdr.offset == AVDT_ACP) {
        APPL_TRACE_DEBUG("%s: Incoming L2CAP acquired, set state as incoming",
                         __func__);
        p_scb->OnConnected(p_data->str_msg.bd_addr);
        p_scb->use_rc = true;  
        bta_av_ssm_execute(p_scb, BTA_AV_ACP_CONNECT_EVT, p_data);

  
        bta_av_signalling_timer(NULL);

        APPL_TRACE_DEBUG("%s: Re-start timer for AVDTP service", __func__);
        bta_sys_conn_open(BTA_ID_AV, p_scb->app_id, p_scb->PeerAddress());
  
        p_scb->coll_mask = BTA_AV_COLL_INC_TMR;
        alarm_set_on_mloop(
            p_cb->accept_signalling_timer, BTA_AV_ACCEPT_SIGNALLING_TIMEOUT_MS,
            bta_av_accept_signalling_timer_cback, UINT_TO_PTR(xx));
 }
 }
 }
#if (BTA_AR_INCLUDED == TRUE)
 else if (event == BTA_AR_AVDT_CONN_EVT) {
    alarm_cancel(bta_av_cb.link_signalling_timer);
 }
#endif
 else {
  
    APPL_TRACE_DEBUG("%s: bta_av_cb.conn_lcb is %d", __func__,
                     bta_av_cb.conn_lcb);

    p_lcb = bta_av_find_lcb(p_data->str_msg.bd_addr, BTA_AV_LCB_FREE);
 if (p_lcb && (p_lcb->conn_msk || bta_av_cb.conn_lcb)) {
      APPL_TRACE_DEBUG("%s: conn_msk: 0x%x", __func__, p_lcb->conn_msk);
  
 for (xx = 0; xx < BTA_AV_NUM_STRS; xx++) {
 if (p_cb->p_scb[xx] &&
            p_cb->p_scb[xx]->PeerAddress() == p_data->str_msg.bd_addr) {
          APPL_TRACE_DEBUG("%s: Closing timer for AVDTP service", __func__);
          bta_sys_conn_close(BTA_ID_AV, p_cb->p_scb[xx]->app_id,
                             p_cb->p_scb[xx]->PeerAddress());
 }
        mask = 1 << (xx + 1);
 if (((mask & p_lcb->conn_msk) || bta_av_cb.conn_lcb) &&
            p_cb->p_scb[xx] &&
            p_cb->p_scb[xx]->PeerAddress() == p_data->str_msg.bd_addr) {
          APPL_TRACE_WARNING("%s: Sending AVDT_DISCONNECT_EVT peer_addr=%s",
                             __func__,
                             p_cb->p_scb[xx]->PeerAddress().ToString().c_str());
          bta_av_ssm_execute(p_cb->p_scb[xx], BTA_AV_AVDT_DISCONNECT_EVT, NULL);
 }
 }
 }
 }
  APPL_TRACE_DEBUG("%s: sig_chg conn_lcb: 0x%x", __func__, p_cb->conn_lcb);
}
