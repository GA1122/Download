void bta_av_rc_opened(tBTA_AV_CB* p_cb, tBTA_AV_DATA* p_data) {
  tBTA_AV_RC_OPEN rc_open;
  tBTA_AV_SCB* p_scb;
 int i;
 uint8_t shdl = 0;
  tBTA_AV_LCB* p_lcb;
  tBTA_AV_RCB* p_rcb;
 uint8_t tmp;
 uint8_t disc = 0;

  
 for (i = 0; i < BTA_AV_NUM_STRS; i++) {
    p_scb = p_cb->p_scb[i];
 if (p_scb && p_scb->PeerAddress() == p_data->rc_conn_chg.peer_addr) {
      p_scb->rc_handle = p_data->rc_conn_chg.handle;
      APPL_TRACE_DEBUG("%s: shdl:%d, srch %d", __func__, i + 1,
                       p_scb->rc_handle);
      shdl = i + 1;
      LOG_INFO(LOG_TAG, "%s: allow incoming AVRCP connections:%d", __func__,
               p_scb->use_rc);
      alarm_cancel(p_scb->avrc_ct_timer);
      disc = p_scb->hndl;
 break;
 }
 }

  i = p_data->rc_conn_chg.handle;
 if (p_cb->rcb[i].handle == BTA_AV_RC_HANDLE_NONE) {
    APPL_TRACE_ERROR("%s: not a valid handle:%d any more", __func__, i);
 return;
 }

  APPL_TRACE_DEBUG("%s: local features %d peer features %d", __func__,
                   p_cb->features, p_cb->rcb[i].peer_features);

  
  AVRC_OpenBrowse(p_data->rc_conn_chg.handle, AVCT_ACP);

 if (p_cb->rcb[i].lidx == (BTA_AV_NUM_LINKS + 1) && shdl != 0) {
  
    p_rcb = bta_av_get_rcb_by_shdl(shdl);
 if (p_rcb) {
      p_rcb->shdl = p_cb->rcb[i].shdl;
      tmp = p_rcb->lidx;
      p_rcb->lidx = p_cb->rcb[i].lidx;
      p_cb->rcb[i].lidx = tmp;
      p_cb->rc_acp_handle = p_rcb->handle;
      p_cb->rc_acp_idx = (p_rcb - p_cb->rcb) + 1;
      APPL_TRACE_DEBUG("%s: switching RCB rc_acp_handle:%d idx:%d", __func__,
                       p_cb->rc_acp_handle, p_cb->rc_acp_idx);
 }
 }

  p_cb->rcb[i].shdl = shdl;
  rc_open.rc_handle = i;
  APPL_TRACE_ERROR("%s: rcb[%d] shdl:%d lidx:%d/%d", __func__, i, shdl,
                   p_cb->rcb[i].lidx, p_cb->lcb[BTA_AV_NUM_LINKS].lidx);
  p_cb->rcb[i].status |= BTA_AV_RC_CONN_MASK;

 if (!shdl && 0 == p_cb->lcb[BTA_AV_NUM_LINKS].lidx) {
  
    p_lcb = &p_cb->lcb[BTA_AV_NUM_LINKS];
    p_lcb->addr = p_data->rc_conn_chg.peer_addr;
    p_lcb->lidx = BTA_AV_NUM_LINKS + 1;
    p_cb->rcb[i].lidx = p_lcb->lidx;
    p_lcb->conn_msk = 1;
    APPL_TRACE_ERROR("%s: bd_addr: %s rcb[%d].lidx=%d, lcb.conn_msk=x%x",
                     __func__, p_lcb->addr.ToString().c_str(), i,
                     p_cb->rcb[i].lidx, p_lcb->conn_msk);
    disc = p_data->rc_conn_chg.handle | BTA_AV_CHNL_MSK;
 }

  rc_open.peer_addr = p_data->rc_conn_chg.peer_addr;
  rc_open.peer_features = p_cb->rcb[i].peer_features;
  rc_open.status = BTA_AV_SUCCESS;
  APPL_TRACE_DEBUG("%s: local features:x%x peer_features:x%x", __func__,
                   p_cb->features, rc_open.peer_features);
 if (rc_open.peer_features == 0) {
  
 if (p_cb->features & BTA_AV_FEAT_RCCT)
      rc_open.peer_features |= BTA_AV_FEAT_RCTG;
 if (p_cb->features & BTA_AV_FEAT_RCTG)
      rc_open.peer_features |= BTA_AV_FEAT_RCCT;

    bta_av_rc_disc(disc);
 }
  tBTA_AV bta_av_data;
  bta_av_data.rc_open = rc_open;
 (*p_cb->p_cback)(BTA_AV_RC_OPEN_EVT, &bta_av_data);

  
 if ((p_cb->features & BTA_AV_FEAT_BROWSE) &&
 (rc_open.peer_features & BTA_AV_FEAT_BROWSE) &&
 ((p_cb->rcb[i].status & BTA_AV_RC_ROLE_MASK) == BTA_AV_RC_ROLE_INT)) {
    APPL_TRACE_DEBUG("%s: opening AVRC Browse channel", __func__);
    AVRC_OpenBrowse(p_data->rc_conn_chg.handle, AVCT_INT);
 }
}
