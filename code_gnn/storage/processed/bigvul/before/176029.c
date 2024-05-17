void bta_av_rc_disc_done(UNUSED_ATTR tBTA_AV_DATA* p_data) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
  tBTA_AV_SCB* p_scb = NULL;
  tBTA_AV_LCB* p_lcb;
 uint8_t rc_handle;
  tBTA_AV_FEAT peer_features = 0;  

  APPL_TRACE_DEBUG("%s: bta_av_rc_disc_done disc:x%x", __func__, p_cb->disc);
 if (!p_cb->disc) {
 return;
 }

 if ((p_cb->disc & BTA_AV_CHNL_MSK) == BTA_AV_CHNL_MSK) {
  
    rc_handle = p_cb->disc & (~BTA_AV_CHNL_MSK);
 } else {
  
 if (((p_cb->disc & BTA_AV_HNDL_MSK) - 1) < BTA_AV_NUM_STRS) {
      p_scb = p_cb->p_scb[(p_cb->disc & BTA_AV_HNDL_MSK) - 1];
 }
 if (p_scb) {
      rc_handle = p_scb->rc_handle;
 } else {
      p_cb->disc = 0;
 return;
 }
 }

  APPL_TRACE_DEBUG("%s: rc_handle %d", __func__, rc_handle);
#if (BTA_AV_SINK_INCLUDED == TRUE)
 if (p_cb->sdp_a2dp_snk_handle) {
  
    peer_features =
        bta_avk_check_peer_features(UUID_SERVCLASS_AV_REM_CTRL_TARGET);
    APPL_TRACE_DEBUG("%s: populating rem ctrl target features %d", __func__,
                     peer_features);
 if (BTA_AV_FEAT_ADV_CTRL &
        bta_avk_check_peer_features(UUID_SERVCLASS_AV_REMOTE_CONTROL))
      peer_features |= (BTA_AV_FEAT_ADV_CTRL | BTA_AV_FEAT_RCCT);
 } else
#endif
 if (p_cb->sdp_a2dp_handle) {
  
    peer_features =
        bta_av_check_peer_features(UUID_SERVCLASS_AV_REMOTE_CONTROL);
 if ((p_cb->features & BTA_AV_FEAT_ADV_CTRL) &&
 ((peer_features & BTA_AV_FEAT_ADV_CTRL) == 0)) {
  
      peer_features |=
          bta_av_check_peer_features(UUID_SERVCLASS_AV_REM_CTRL_TARGET);
 }

  
    tSDP_DISC_REC* p_rec = nullptr;
    p_rec = SDP_FindServiceInDb(p_cb->p_disc_db,
                                UUID_SERVCLASS_AV_REMOTE_CONTROL, p_rec);
 if (p_rec != NULL &&
        SDP_FindAttributeInRec(p_rec, ATTR_ID_BT_PROFILE_DESC_LIST) != NULL) {
  
 uint16_t peer_rc_version = 0xFFFF;  
      SDP_FindProfileVersionInRec(p_rec, UUID_SERVCLASS_AV_REMOTE_CONTROL,
 &peer_rc_version);
 if (peer_rc_version <= AVRC_REV_1_3) {
        APPL_TRACE_DEBUG("%s: Using AVRCP 1.3 Capabilities with remote device",
                         __func__);
        p_bta_av_cfg = &bta_av_cfg_compatibility;
 }
 }
 }

  p_cb->disc = 0;
  osi_free_and_reset((void**)&p_cb->p_disc_db);

  APPL_TRACE_DEBUG("%s: peer_features 0x%x, features 0x%x", __func__,
                   peer_features, p_cb->features);

  
 if (rc_handle == BTA_AV_RC_HANDLE_NONE) {
 if (p_scb) {
  
 if ((((p_cb->features & BTA_AV_FEAT_RCCT) &&
 (peer_features & BTA_AV_FEAT_RCTG)) ||
 ((p_cb->features & BTA_AV_FEAT_RCTG) &&
 (peer_features & BTA_AV_FEAT_RCCT)))) {
        p_lcb = bta_av_find_lcb(p_scb->PeerAddress(), BTA_AV_LCB_FIND);
 if (p_lcb) {
          rc_handle = bta_av_rc_create(p_cb, AVCT_INT,
 (uint8_t)(p_scb->hdi + 1), p_lcb->lidx);
          p_cb->rcb[rc_handle].peer_features = peer_features;
 } else {
          APPL_TRACE_ERROR("%s: can not find LCB!!", __func__);
 }
 } else if (p_scb->use_rc) {
  
        p_scb->use_rc = false;
        tBTA_AV_RC_OPEN rc_open;
        rc_open.peer_addr = p_scb->PeerAddress();
        rc_open.peer_features = 0;
        rc_open.status = BTA_AV_FAIL_SDP;
        tBTA_AV bta_av_data;
        bta_av_data.rc_open = rc_open;
 (*p_cb->p_cback)(BTA_AV_RC_OPEN_EVT, &bta_av_data);
 }
 }
 } else {
    tBTA_AV_RC_FEAT rc_feat;
    p_cb->rcb[rc_handle].peer_features = peer_features;
    rc_feat.rc_handle = rc_handle;
    rc_feat.peer_features = peer_features;
 if (p_scb == NULL) {
  
      rc_feat.peer_addr = p_cb->lcb[p_cb->rcb[rc_handle].lidx].addr;
 } else {
      rc_feat.peer_addr = p_scb->PeerAddress();
 }
    tBTA_AV bta_av_data;
    bta_av_data.rc_feat = rc_feat;
 (*p_cb->p_cback)(BTA_AV_RC_FEAT_EVT, &bta_av_data);
 }
}
