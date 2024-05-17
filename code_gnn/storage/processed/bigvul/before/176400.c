static bool btif_av_state_idle_handler(btif_sm_event_t event, void* p_data) {
  BTIF_TRACE_DEBUG("%s: event=%s flags=0x%x", __func__,
                   dump_av_sm_event_name((btif_av_sm_event_t)event),
                   btif_av_cb.flags);

 switch (event) {
 case BTIF_SM_ENTER_EVT:
  
      btif_av_cb.peer_bda = RawAddress::kEmpty;
      btif_av_cb.flags = 0;
      btif_av_cb.edr = 0;
      bta_av_co_init(btif_av_cb.codec_priorities);
      btif_a2dp_on_idle();
 break;

 case BTIF_SM_EXIT_EVT:
 break;

 case BTA_AV_ENABLE_EVT:
 break;

 case BTA_AV_REGISTER_EVT:
      btif_av_cb.bta_handle = ((tBTA_AV*)p_data)->registr.hndl;
 break;

 case BTA_AV_PENDING_EVT:
 case BTIF_AV_CONNECT_REQ_EVT: {
 if (event == BTIF_AV_CONNECT_REQ_EVT) {
 btif_av_connect_req_t* connect_req_p = (btif_av_connect_req_t*)p_data;
        btif_av_cb.peer_bda = *connect_req_p->target_bda;
        btif_av_cb.self_initiated_connection = true;
        BTA_AvOpen(btif_av_cb.peer_bda, btif_av_cb.bta_handle, true,
                   BTA_SEC_AUTHENTICATE, connect_req_p->uuid);
 } else if (event == BTA_AV_PENDING_EVT) {
        btif_av_cb.peer_bda = ((tBTA_AV*)p_data)->pend.bd_addr;
        btif_av_cb.self_initiated_connection = false;
 if (bt_av_src_callbacks != NULL) {
          BTA_AvOpen(btif_av_cb.peer_bda, btif_av_cb.bta_handle, true,
                     BTA_SEC_AUTHENTICATE, UUID_SERVCLASS_AUDIO_SOURCE);
 }
 if (bt_av_sink_callbacks != NULL) {
          BTA_AvOpen(btif_av_cb.peer_bda, btif_av_cb.bta_handle, true,
                     BTA_SEC_AUTHENTICATE, UUID_SERVCLASS_AUDIO_SINK);
 }
 }
      btif_sm_change_state(btif_av_cb.sm_handle, BTIF_AV_STATE_OPENING);
 } break;

 case BTA_AV_RC_OPEN_EVT:
  

      BTIF_TRACE_WARNING("%s: BTA_AV_RC_OPEN_EVT received w/o AV", __func__);
      alarm_set_on_mloop(av_open_on_rc_timer, BTIF_TIMEOUT_AV_OPEN_ON_RC_MS,
                         btif_initiate_av_open_timer_timeout, NULL);
      btif_rc_handler(event, (tBTA_AV*)p_data);
 break;

 case BTA_AV_RC_BROWSE_OPEN_EVT:
      BTIF_TRACE_DEBUG("%s: BTA_AV_RC_BROWSE_OPEN_EVT received", __func__);
      btif_rc_handler(event, (tBTA_AV*)p_data);
 break;

 case BTIF_AV_SOURCE_CONFIG_REQ_EVT:
      btif_update_source_codec(p_data);
 break;

 case BTIF_AV_SOURCE_CONFIG_UPDATED_EVT:
      btif_report_source_codec_state(p_data);
 break;

  
 case BTIF_AV_SINK_CONFIG_REQ_EVT: {
 btif_av_sink_config_req_t req;
      memcpy(&req, p_data, sizeof(req));

      BTIF_TRACE_WARNING(
 "%s: BTIF_AV_SINK_CONFIG_REQ_EVT sample_rate=%d "
 "channel_count=%d",
          __func__, req.sample_rate, req.channel_count);
 if (bt_av_sink_callbacks != NULL) {
        HAL_CBACK(bt_av_sink_callbacks, audio_config_cb, &(req.peer_bd),
                  req.sample_rate, req.channel_count);
 }
 } break;

 case BTA_AV_OPEN_EVT: {
      tBTA_AV* p_bta_data = (tBTA_AV*)p_data;
 btav_connection_state_t state;
 btif_sm_state_t av_state;
      BTIF_TRACE_WARNING("%s: BTA_AV_OPEN_EVT status=%d, edr=0x%x", __func__,
                         p_bta_data->open.status, p_bta_data->open.edr);

 if (p_bta_data->open.status == BTA_AV_SUCCESS) {
        state = BTAV_CONNECTION_STATE_CONNECTED;
        av_state = BTIF_AV_STATE_OPENED;
        btif_av_cb.edr = p_bta_data->open.edr;

        btif_av_cb.peer_sep = p_bta_data->open.sep;
 } else {
        BTIF_TRACE_WARNING("%s: BTA_AV_OPEN_EVT::FAILED status=%d", __func__,
                           p_bta_data->open.status);
        state = BTAV_CONNECTION_STATE_DISCONNECTED;
        av_state = BTIF_AV_STATE_IDLE;
 }

  
      btif_report_connection_state(state, &(btif_av_cb.peer_bda));
  
      btif_sm_change_state(btif_av_cb.sm_handle, av_state);
 if (btif_av_cb.peer_sep == AVDT_TSEP_SNK) {
  
        btif_rc_check_handle_pending_play(
            p_bta_data->open.bd_addr,
 (p_bta_data->open.status == BTA_AV_SUCCESS));
 } else if ((btif_av_cb.peer_sep == AVDT_TSEP_SRC) &&
 (p_bta_data->open.status == BTA_AV_SUCCESS)) {
  
        BTA_AvOpenRc(btif_av_cb.bta_handle);
 }
      btif_queue_advance();
 } break;

 case BTA_AV_REMOTE_CMD_EVT:
 case BTA_AV_VENDOR_CMD_EVT:
 case BTA_AV_META_MSG_EVT:
 case BTA_AV_RC_FEAT_EVT:
 case BTA_AV_REMOTE_RSP_EVT:
      btif_rc_handler(event, (tBTA_AV*)p_data);
 break;

 case BTA_AV_RC_CLOSE_EVT:
      BTIF_TRACE_DEBUG("%s: BTA_AV_RC_CLOSE_EVT: Stopping AV timer.", __func__);
      alarm_cancel(av_open_on_rc_timer);
      btif_rc_handler(event, (tBTA_AV*)p_data);
 break;

 case BTIF_AV_OFFLOAD_START_REQ_EVT:
      BTIF_TRACE_ERROR(
 "%s: BTIF_AV_OFFLOAD_START_REQ_EVT: Stream not Started IDLE",
          __func__);
      btif_a2dp_on_offload_started(BTA_AV_FAIL);
 break;

 default:
      BTIF_TRACE_WARNING("%s: unhandled event=%s", __func__,
                         dump_av_sm_event_name((btif_av_sm_event_t)event));
 return false;
 }

 return true;
}
