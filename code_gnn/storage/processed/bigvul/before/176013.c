void bta_av_dereg_comp(tBTA_AV_DATA* p_data) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
  tBTA_AV_SCB* p_scb;
  tBTA_UTL_COD cod;
 uint8_t mask;
  BT_HDR* p_buf;

  
  p_scb = bta_av_hndl_to_scb(p_data->hdr.layer_specific);

 if (p_scb) {
    APPL_TRACE_DEBUG("%s: deregistered %d(h%d)", __func__, p_scb->chnl,
                     p_scb->hndl);
    mask = BTA_AV_HNDL_TO_MSK(p_scb->hdi);
    p_cb->reg_audio &= ~mask;
 if ((p_cb->conn_audio & mask) && bta_av_cb.audio_open_cnt) {
  
      bta_av_cb.audio_open_cnt--;
 }
    p_cb->conn_audio &= ~mask;

 if (p_scb->q_tag == BTA_AV_Q_TAG_STREAM && p_scb->a2dp_list) {
  
 while (!list_is_empty(p_scb->a2dp_list)) {
        p_buf = (BT_HDR*)list_front(p_scb->a2dp_list);
        list_remove(p_scb->a2dp_list, p_buf);
        osi_free(p_buf);
 }
 }

  
 if (!p_cb->reg_audio) {
#if (BTA_AR_INCLUDED == TRUE)
      bta_ar_dereg_avrc(UUID_SERVCLASS_AV_REMOTE_CONTROL, BTA_ID_AV);
#endif
 if (p_cb->sdp_a2dp_handle) {
        bta_av_del_sdp_rec(&p_cb->sdp_a2dp_handle);
        p_cb->sdp_a2dp_handle = 0;
        bta_sys_remove_uuid(UUID_SERVCLASS_AUDIO_SOURCE);
 }

#if (BTA_AV_SINK_INCLUDED == TRUE)
 if (p_cb->sdp_a2dp_snk_handle) {
        bta_av_del_sdp_rec(&p_cb->sdp_a2dp_snk_handle);
        p_cb->sdp_a2dp_snk_handle = 0;
        bta_sys_remove_uuid(UUID_SERVCLASS_AUDIO_SINK);
 }
#endif
 }

    bta_av_free_scb(p_scb);
 }

  APPL_TRACE_DEBUG("%s: audio 0x%x, disable:%d", __func__, p_cb->reg_audio,
                   p_cb->disabling);
  
 if (p_cb->reg_audio == 0) {
#if (BTA_AR_INCLUDED == TRUE)
  
    bta_ar_dereg_avdt(BTA_ID_AV);

  
    bta_ar_dereg_avrc(UUID_SERVCLASS_AV_REM_CTRL_TARGET, BTA_ID_AV);
    bta_ar_dereg_avct(BTA_ID_AV);
#endif

 if (p_cb->disabling) {
      p_cb->disabling = false;
      bta_av_cb.features = 0;
 }

  
    cod.service = BTM_COD_SERVICE_CAPTURING;
    utl_set_device_class(&cod, BTA_UTL_CLR_COD_SERVICE_CLASS);
 }
}
