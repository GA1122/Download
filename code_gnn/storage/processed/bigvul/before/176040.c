void bta_av_signalling_timer(UNUSED_ATTR tBTA_AV_DATA* p_data) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
 int xx;
 uint8_t mask;
  tBTA_AV_LCB* p_lcb = NULL;

  APPL_TRACE_DEBUG("%s: conn_lcb=0x%x", __func__, p_cb->conn_lcb);
 for (xx = 0; xx < BTA_AV_NUM_LINKS; xx++) {
    p_lcb = &p_cb->lcb[xx];
    mask = 1 << xx;
    APPL_TRACE_DEBUG(
 "%s: index=%d conn_lcb=0x%x peer=%s conn_mask=0x%x lidx=%d", __func__,
        xx, p_cb->conn_lcb, p_lcb->addr.ToString().c_str(), p_lcb->conn_msk,
        p_lcb->lidx);
 if (mask & p_cb->conn_lcb) {
  
 if (!p_lcb->conn_msk) {
        bta_sys_start_timer(p_cb->link_signalling_timer,
                            BTA_AV_SIGNALLING_TIMEOUT_MS,
                            BTA_AV_SIGNALLING_TIMER_EVT, 0);
        tBTA_AV_PEND pend;
        pend.bd_addr = p_lcb->addr;
        tBTA_AV bta_av_data;
        bta_av_data.pend = pend;
        APPL_TRACE_DEBUG(
 "%s: BTA_AV_PENDING_EVT for %s index=%d conn_mask=0x%x lidx=%d",
            __func__, pend.bd_addr.ToString().c_str(), xx, p_lcb->conn_msk,
            p_lcb->lidx);
 (*p_cb->p_cback)(BTA_AV_PENDING_EVT, &bta_av_data);
 }
 }
 }
}
