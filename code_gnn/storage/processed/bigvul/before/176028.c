void bta_av_rc_disc(uint8_t disc) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
  tAVRC_SDP_DB_PARAMS db_params;
 uint16_t attr_list[] = {ATTR_ID_SERVICE_CLASS_ID_LIST,
                          ATTR_ID_BT_PROFILE_DESC_LIST,
                          ATTR_ID_SUPPORTED_FEATURES};
 uint8_t hdi;
  tBTA_AV_SCB* p_scb;
 RawAddress peer_addr = RawAddress::kEmpty;
 uint8_t rc_handle;

  APPL_TRACE_DEBUG("%s: disc: 0x%x, bta_av_cb.disc: 0x%x", __func__, disc,
                   bta_av_cb.disc);
 if ((bta_av_cb.disc != 0) || (disc == 0)) return;

 if ((disc & BTA_AV_CHNL_MSK) == BTA_AV_CHNL_MSK) {
  
    rc_handle = disc & (~BTA_AV_CHNL_MSK);
 if (p_cb->rcb[rc_handle].lidx) {
      peer_addr = p_cb->lcb[p_cb->rcb[rc_handle].lidx - 1].addr;
 }
 } else {
    hdi = (disc & BTA_AV_HNDL_MSK) - 1;
    p_scb = p_cb->p_scb[hdi];

 if (p_scb) {
      APPL_TRACE_DEBUG("%s: rc_handle %d", __func__, p_scb->rc_handle);
      peer_addr = p_scb->PeerAddress();
 }
 }

 if (!peer_addr.IsEmpty()) {
  
 if (p_cb->p_disc_db == NULL)
      p_cb->p_disc_db = (tSDP_DISCOVERY_DB*)osi_malloc(BTA_AV_DISC_BUF_SIZE);

  
    db_params.db_len = BTA_AV_DISC_BUF_SIZE;
    db_params.num_attr = 3;
    db_params.p_db = p_cb->p_disc_db;
    db_params.p_attrs = attr_list;

  
 if (AVRC_FindService(UUID_SERVCLASS_AV_REMOTE_CONTROL, peer_addr,
 &db_params,
                         base::Bind(bta_av_avrc_sdp_cback)) == AVRC_SUCCESS) {
      p_cb->disc = disc;
      APPL_TRACE_DEBUG("%s: disc 0x%x", __func__, p_cb->disc);
 }
 }
}
