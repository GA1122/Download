void bta_av_rc_closed(tBTA_AV_DATA* p_data) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
  tBTA_AV_RC_CLOSE rc_close;
  tBTA_AV_RC_CONN_CHG* p_msg = (tBTA_AV_RC_CONN_CHG*)p_data;
  tBTA_AV_RCB* p_rcb;
  tBTA_AV_SCB* p_scb;
 int i;
 bool conn = false;
  tBTA_AV_LCB* p_lcb;

  rc_close.rc_handle = BTA_AV_RC_HANDLE_NONE;
  p_scb = NULL;
  APPL_TRACE_DEBUG("%s: rc_handle:%d", __func__, p_msg->handle);
 for (i = 0; i < BTA_AV_NUM_RCB; i++) {
    p_rcb = &p_cb->rcb[i];
    APPL_TRACE_DEBUG("%s: rcb[%d] rc_handle:%d, status=0x%x", __func__, i,
                     p_rcb->handle, p_rcb->status);
 if (p_rcb->handle == p_msg->handle) {
      rc_close.rc_handle = i;
      p_rcb->status &= ~BTA_AV_RC_CONN_MASK;
      p_rcb->peer_features = 0;
      APPL_TRACE_DEBUG("%s: shdl:%d, lidx:%d", __func__, p_rcb->shdl,
                       p_rcb->lidx);
 if (p_rcb->shdl) {
 if ((p_rcb->shdl - 1) < BTA_AV_NUM_STRS) {
          p_scb = bta_av_cb.p_scb[p_rcb->shdl - 1];
 }
 if (p_scb) {
          rc_close.peer_addr = p_scb->PeerAddress();
 if (p_scb->rc_handle == p_rcb->handle)
            p_scb->rc_handle = BTA_AV_RC_HANDLE_NONE;
          APPL_TRACE_DEBUG("%s: shdl:%d, srch:%d", __func__, p_rcb->shdl,
                           p_scb->rc_handle);
 }
        p_rcb->shdl = 0;
 } else if (p_rcb->lidx == (BTA_AV_NUM_LINKS + 1)) {
  
        p_lcb = &p_cb->lcb[BTA_AV_NUM_LINKS];
        rc_close.peer_addr = p_msg->peer_addr;
        LOG_INFO(LOG_TAG, "%s: rc_only closed bd_addr: %s", __func__,
                 p_msg->peer_addr.ToString().c_str());
        p_lcb->conn_msk = 0;
        p_lcb->lidx = 0;
 }
      p_rcb->lidx = 0;

 if ((p_rcb->status & BTA_AV_RC_ROLE_MASK) == BTA_AV_RC_ROLE_INT) {
  
        p_rcb->handle = BTA_AV_RC_HANDLE_NONE;
        p_rcb->status = 0;
 } else {
  
        bta_av_del_rc(p_rcb);

  
 if (bta_av_cb.rc_acp_handle == BTA_AV_RC_HANDLE_NONE &&
            bta_av_cb.features & BTA_AV_FEAT_RCTG)
          bta_av_rc_create(&bta_av_cb, AVCT_ACP, 0, BTA_AV_NUM_LINKS + 1);
 }
 } else if ((p_rcb->handle != BTA_AV_RC_HANDLE_NONE) &&
 (p_rcb->status & BTA_AV_RC_CONN_MASK)) {
  
      conn = true;
 }
 }

 if (!conn) {
  
    bta_av_sm_execute(p_cb, BTA_AV_AVRC_NONE_EVT, NULL);
 }

 if (rc_close.rc_handle == BTA_AV_RC_HANDLE_NONE) {
    rc_close.rc_handle = p_msg->handle;
    rc_close.peer_addr = p_msg->peer_addr;
 }
  tBTA_AV bta_av_data;
  bta_av_data.rc_close = rc_close;
 (*p_cb->p_cback)(BTA_AV_RC_CLOSE_EVT, &bta_av_data);
}
