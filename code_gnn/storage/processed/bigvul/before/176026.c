uint8_t bta_av_rc_create(tBTA_AV_CB* p_cb, uint8_t role, uint8_t shdl,
 uint8_t lidx) {
 if (is_new_avrcp_enabled()) {
    APPL_TRACE_WARNING("%s: Skipping RC creation for the old AVRCP profile",
                       __func__);
 return BTA_AV_RC_HANDLE_NONE;
 }

  tAVRC_CONN_CB ccb;
 RawAddress bda = RawAddress::kAny;
 uint8_t status = BTA_AV_RC_ROLE_ACP;
  tBTA_AV_SCB* p_scb = p_cb->p_scb[shdl - 1];
 int i;
 uint8_t rc_handle;
  tBTA_AV_RCB* p_rcb;

 if (role == AVCT_INT) {
    bda = p_scb->PeerAddress();
    status = BTA_AV_RC_ROLE_INT;
 } else {
    p_rcb = bta_av_get_rcb_by_shdl(shdl);
 if (p_rcb != NULL) {
      APPL_TRACE_ERROR("%s: ACP handle exist for shdl:%d", __func__, shdl);
 return p_rcb->handle;
 }
 }

  ccb.ctrl_cback = base::Bind(bta_av_rc_ctrl_cback);
  ccb.msg_cback = base::Bind(bta_av_rc_msg_cback);
  ccb.company_id = p_bta_av_cfg->company_id;
  ccb.conn = role;
  
  ccb.control = p_cb->features & (BTA_AV_FEAT_RCTG | BTA_AV_FEAT_RCCT |
                                  BTA_AV_FEAT_METADATA | AVRC_CT_PASSIVE);

 if (AVRC_Open(&rc_handle, &ccb, bda) != AVRC_SUCCESS)
 return BTA_AV_RC_HANDLE_NONE;

  i = rc_handle;
  p_rcb = &p_cb->rcb[i];

 if (p_rcb->handle != BTA_AV_RC_HANDLE_NONE) {
    APPL_TRACE_ERROR("%s: found duplicated handle:%d", __func__, rc_handle);
 }

  p_rcb->handle = rc_handle;
  p_rcb->status = status;
  p_rcb->shdl = shdl;
  p_rcb->lidx = lidx;
  p_rcb->peer_features = 0;
 if (lidx == (BTA_AV_NUM_LINKS + 1)) {
  
    p_cb->rc_acp_handle = p_rcb->handle;
    p_cb->rc_acp_idx = (i + 1);
    APPL_TRACE_DEBUG("%s: rc_acp_handle:%d idx:%d", __func__,
                     p_cb->rc_acp_handle, p_cb->rc_acp_idx);
 }
  APPL_TRACE_DEBUG(
 "%s: create %d, role: %d, shdl:%d, rc_handle:%d, lidx:%d, status:0x%x",
      __func__, i, role, shdl, p_rcb->handle, lidx, p_rcb->status);

 return rc_handle;
}
