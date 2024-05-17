tBTA_AV_FEAT bta_avk_check_peer_features(uint16_t service_uuid) {
  tBTA_AV_FEAT peer_features = 0;
  tBTA_AV_CB* p_cb = &bta_av_cb;

  APPL_TRACE_DEBUG("%s: service_uuid:x%x", __func__, service_uuid);

  
  tSDP_DISC_REC* p_rec =
      SDP_FindServiceInDb(p_cb->p_disc_db, service_uuid, NULL);
 while (p_rec) {
    APPL_TRACE_DEBUG("%s: found Service record for x%x", __func__,
                     service_uuid);

 if ((SDP_FindAttributeInRec(p_rec, ATTR_ID_SERVICE_CLASS_ID_LIST)) !=
        NULL) {
  
 if (SDP_FindServiceInDb(p_cb->p_disc_db, UUID_SERVCLASS_AV_REMOTE_CONTROL,
                              NULL)) {
        peer_features |= BTA_AV_FEAT_RCCT;
 }
 if (SDP_FindServiceInDb(p_cb->p_disc_db,
                              UUID_SERVCLASS_AV_REM_CTRL_TARGET, NULL)) {
        peer_features |= BTA_AV_FEAT_RCTG;
 }
 }

 if ((SDP_FindAttributeInRec(p_rec, ATTR_ID_BT_PROFILE_DESC_LIST)) != NULL) {
  
 uint16_t peer_rc_version = 0;
 bool val = SDP_FindProfileVersionInRec(
          p_rec, UUID_SERVCLASS_AV_REMOTE_CONTROL, &peer_rc_version);
      APPL_TRACE_DEBUG("%s: peer_rc_version for TG 0x%x, profile_found %d",
                       __func__, peer_rc_version, val);

 if (peer_rc_version >= AVRC_REV_1_3)
        peer_features |= (BTA_AV_FEAT_VENDOR | BTA_AV_FEAT_METADATA);

  
 if (peer_rc_version >= AVRC_REV_1_3) {
  
        tSDP_DISC_ATTR* p_attr =
            SDP_FindAttributeInRec(p_rec, ATTR_ID_SUPPORTED_FEATURES);
 if (p_attr != NULL) {
 uint16_t categories = p_attr->attr_value.v.u16;
 if (categories & AVRC_SUPF_CT_CAT2)
            peer_features |= (BTA_AV_FEAT_ADV_CTRL);
 if (categories & AVRC_SUPF_CT_APP_SETTINGS)
            peer_features |= (BTA_AV_FEAT_APP_SETTING);
 if (categories & AVRC_SUPF_CT_BROWSE)
            peer_features |= (BTA_AV_FEAT_BROWSE);
 }
 }
 }
  
    p_rec = SDP_FindServiceInDb(p_cb->p_disc_db, service_uuid, p_rec);
 }
  APPL_TRACE_DEBUG("%s: peer_features:x%x", __func__, peer_features);
 return peer_features;
}
