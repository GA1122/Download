bt_status_t btif_av_sink_execute_service(bool b_enable) {
 if (b_enable) {
  
    BTA_AvEnable(BTA_SEC_AUTHENTICATE,
                 BTA_AV_FEAT_NO_SCO_SSPD | BTA_AV_FEAT_RCCT |
                     BTA_AV_FEAT_METADATA | BTA_AV_FEAT_VENDOR |
                     BTA_AV_FEAT_ADV_CTRL | BTA_AV_FEAT_RCTG |
                     BTA_AV_FEAT_BROWSE,
                 bte_av_callback);
    BTA_AvRegister(BTA_AV_CHNL_AUDIO, BTIF_AVK_SERVICE_NAME, 0,
                   bte_av_sink_media_callback, UUID_SERVCLASS_AUDIO_SINK);
 } else {
    BTA_AvDeregister(btif_av_cb.bta_handle);
    BTA_AvDisable();
 }
 return BT_STATUS_SUCCESS;
}
