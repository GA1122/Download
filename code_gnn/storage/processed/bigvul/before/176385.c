static void bte_av_sink_media_callback(tBTA_AV_EVT event,
                                       tBTA_AV_MEDIA* p_data) {
 switch (event) {
 case BTA_AV_SINK_MEDIA_DATA_EVT: {
 btif_sm_state_t state = btif_sm_get_state(btif_av_cb.sm_handle);
 if ((state == BTIF_AV_STATE_STARTED) || (state == BTIF_AV_STATE_OPENED)) {
 uint8_t queue_len = btif_a2dp_sink_enqueue_buf((BT_HDR*)p_data);
        BTIF_TRACE_DEBUG("%s: packets in sink queue %d", __func__, queue_len);
 }
 break;
 }
 case BTA_AV_SINK_MEDIA_CFG_EVT: {
 btif_av_sink_config_req_t config_req;

  
      btif_a2dp_sink_update_decoder((uint8_t*)(p_data->avk_config.codec_info));
  
      config_req.sample_rate =
          A2DP_GetTrackSampleRate(p_data->avk_config.codec_info);
 if (config_req.sample_rate == -1) {
        APPL_TRACE_ERROR("%s: cannot get the track frequency", __func__);
 break;
 }
      config_req.channel_count =
          A2DP_GetTrackChannelCount(p_data->avk_config.codec_info);
 if (config_req.channel_count == -1) {
        APPL_TRACE_ERROR("%s: cannot get the channel count", __func__);
 break;
 }

      config_req.peer_bd = p_data->avk_config.bd_addr;
      btif_transfer_context(btif_av_handle_event, BTIF_AV_SINK_CONFIG_REQ_EVT,
 (char*)&config_req, sizeof(config_req), NULL);
 break;
 }
 default:
 break;
 }
}
