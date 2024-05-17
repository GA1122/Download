static void bta_av_accept_signalling_timer_cback(void* data) {
 uint32_t inx = PTR_TO_UINT(data);
  tBTA_AV_CB* p_cb = &bta_av_cb;
  tBTA_AV_SCB* p_scb = NULL;
 if (inx < BTA_AV_NUM_STRS) {
    p_scb = p_cb->p_scb[inx];
 }
 if (p_scb) {
    APPL_TRACE_DEBUG("%s: coll_mask = 0x%02X", __func__, p_scb->coll_mask);

 if (p_scb->coll_mask & BTA_AV_COLL_INC_TMR) {
      p_scb->coll_mask &= ~BTA_AV_COLL_INC_TMR;

 if (bta_av_is_scb_opening(p_scb)) {
        APPL_TRACE_DEBUG("%s: stream state opening: SDP started = %d", __func__,
                         p_scb->sdp_discovery_started);
 if (p_scb->sdp_discovery_started) {
  
          p_scb->coll_mask |= BTA_AV_COLL_INC_TMR;

          alarm_set_on_mloop(p_cb->accept_signalling_timer,
                             BTA_AV_ACCEPT_SIGNALLING_TIMEOUT_MS,
                             bta_av_accept_signalling_timer_cback,
                             UINT_TO_PTR(inx));
 } else {
  
          bta_av_discover_req(p_scb, NULL);
 }
 } else if (bta_av_is_scb_incoming(p_scb)) {
  

        APPL_TRACE_DEBUG("%s: stream state incoming", __func__);
  
 if (p_scb->coll_mask & BTA_AV_COLL_API_CALLED) {
          p_scb->coll_mask &= ~BTA_AV_COLL_API_CALLED;

  
          tBTA_AV_API_OPEN* p_buf =
 (tBTA_AV_API_OPEN*)osi_malloc(sizeof(tBTA_AV_API_OPEN));
          memcpy(p_buf, &(p_scb->open_api), sizeof(tBTA_AV_API_OPEN));
          bta_sys_sendmsg(p_buf);
 }
 }
 }
 }
}
