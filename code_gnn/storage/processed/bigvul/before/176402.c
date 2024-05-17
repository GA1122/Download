static bool btif_av_state_opening_handler(btif_sm_event_t event, void* p_data) {
  BTIF_TRACE_DEBUG("%s: event=%s flags=0x%x", __func__,
                   dump_av_sm_event_name((btif_av_sm_event_t)event),
                   btif_av_cb.flags);

 switch (event) {
 case BTIF_SM_ENTER_EVT:
  
      btif_report_connection_state(BTAV_CONNECTION_STATE_CONNECTING,
 &(btif_av_cb.peer_bda));
 break;

 case BTIF_SM_EXIT_EVT:
 break;

 case BTA_AV_REJECT_EVT:
      BTIF_TRACE_WARNING("%s: Received BTA_AV_REJECT_EVT", __func__);
      btif_report_connection_state(BTAV_CONNECTION_STATE_DISCONNECTED,
 &(btif_av_cb.peer_bda));
      btif_sm_change_state(btif_av_cb.sm_handle, BTIF_AV_STATE_IDLE);
 if (btif_av_cb.self_initiated_connection) {
        btif_queue_advance();
 }
 break;

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
        BTIF_TRACE_WARNING("%s: BTA_AV_OPEN_EVT::FAILED status: %d", __func__,
                           p_bta_data->open.status);
 RawAddress peer_addr;
 uint8_t peer_handle = BTRC_HANDLE_NONE;
 if (btif_rc_get_connected_peer(&peer_addr) &&
            btif_av_cb.peer_bda == peer_addr) {
  
          BTIF_TRACE_WARNING("%s: Disconnecting AVRCP: peer_addr=%s", __func__,
                             peer_addr.ToString().c_str());
          peer_handle = btif_rc_get_connected_peer_handle(peer_addr);
 if (peer_handle != BTRC_HANDLE_NONE) {
            BTA_AvCloseRc(peer_handle);
 }
 }
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
 if (btif_av_cb.self_initiated_connection) {
        btif_queue_advance();
 }
 } break;

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
 if (btif_av_cb.peer_sep == AVDT_TSEP_SRC &&
          bt_av_sink_callbacks != NULL) {
        HAL_CBACK(bt_av_sink_callbacks, audio_config_cb, &(btif_av_cb.peer_bda),
                  req.sample_rate, req.channel_count);
 }
 } break;

 case BTIF_AV_CONNECT_REQ_EVT: {
 btif_av_connect_req_t* connect_req_p = (btif_av_connect_req_t*)p_data;
 RawAddress& target_bda = *connect_req_p->target_bda;
 if (btif_av_cb.peer_bda == target_bda) {
        BTIF_TRACE_WARNING(
 "%s: device %s is already connecting, ignore Connect request",
            __func__, btif_av_cb.peer_bda.ToString().c_str());
 } else {
        BTIF_TRACE_WARNING(
 "%s: device %s is already connecting, reject Connect request to %s",
            __func__, btif_av_cb.peer_bda.ToString().c_str(),
            target_bda.ToString().c_str());
        btif_report_connection_state(BTAV_CONNECTION_STATE_DISCONNECTED,
 &target_bda);
 }
      btif_queue_advance();
 } break;

 case BTA_AV_PENDING_EVT: {
 const RawAddress& bd_addr = ((tBTA_AV*)p_data)->pend.bd_addr;
 if (bd_addr == btif_av_cb.peer_bda) {
        BTIF_TRACE_WARNING(
 "%s: device %s is already connecting, ignore incoming request",
            __func__, btif_av_cb.peer_bda.ToString().c_str());
 } else {
        BTIF_TRACE_WARNING(
 "%s: device %s is already connecting, reject incoming request "
 "from %s",
            __func__, btif_av_cb.peer_bda.ToString().c_str(),
            bd_addr.ToString().c_str());
        BTA_AvDisconnect(bd_addr);
 }
 } break;

 case BTIF_AV_OFFLOAD_START_REQ_EVT:
      BTIF_TRACE_ERROR(
 "%s: BTIF_AV_OFFLOAD_START_REQ_EVT: Stream not Started OPENING",
          __func__);
      btif_a2dp_on_offload_started(BTA_AV_FAIL);
 break;

 case BTA_AV_CLOSE_EVT:
      btif_a2dp_on_stopped(NULL);
      btif_report_connection_state(BTAV_CONNECTION_STATE_DISCONNECTED,
 &(btif_av_cb.peer_bda));
      btif_sm_change_state(btif_av_cb.sm_handle, BTIF_AV_STATE_IDLE);
 if (btif_av_cb.self_initiated_connection) {
        btif_queue_advance();
 }
 break;

 case BTIF_AV_DISCONNECT_REQ_EVT:
      btif_report_connection_state(BTAV_CONNECTION_STATE_DISCONNECTED,
 &(btif_av_cb.peer_bda));
      BTA_AvClose(btif_av_cb.bta_handle);
      btif_sm_change_state(btif_av_cb.sm_handle, BTIF_AV_STATE_IDLE);
 if (btif_av_cb.self_initiated_connection) {
        btif_queue_advance();
 }
 break;

      CHECK_RC_EVENT(event, (tBTA_AV*)p_data);

 default:
      BTIF_TRACE_WARNING("%s: unhandled event=%s", __func__,
                         dump_av_sm_event_name((btif_av_sm_event_t)event));
 return false;
 }
 return true;
}
