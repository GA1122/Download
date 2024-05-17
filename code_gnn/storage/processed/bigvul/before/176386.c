bt_status_t btif_av_execute_service(bool b_enable) {
 if (b_enable) {
 

 
 
#if (AVRC_METADATA_INCLUDED == TRUE)
    BTA_AvEnable(BTA_SEC_AUTHENTICATE,
                 BTA_AV_FEAT_RCTG | BTA_AV_FEAT_METADATA | BTA_AV_FEAT_VENDOR |
                     BTA_AV_FEAT_NO_SCO_SSPD
#if (AVRC_ADV_CTRL_INCLUDED == TRUE)
 | BTA_AV_FEAT_RCCT | BTA_AV_FEAT_ADV_CTRL |
                     BTA_AV_FEAT_BROWSE
#endif
 ,
                 bte_av_callback);
#else
    BTA_AvEnable(BTA_SEC_AUTHENTICATE,
 (BTA_AV_FEAT_RCTG | BTA_AV_FEAT_NO_SCO_SSPD), bte_av_callback);
#endif
    BTA_AvRegister(BTA_AV_CHNL_AUDIO, BTIF_AV_SERVICE_NAME, 0, NULL,
                   UUID_SERVCLASS_AUDIO_SOURCE);
 } else {
    BTA_AvDeregister(btif_av_cb.bta_handle);
    BTA_AvDisable();
 }
 return BT_STATUS_SUCCESS;
}
