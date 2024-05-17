static bool btif_av_state_opened_handler(btif_sm_event_t event, void* p_data) {
  tBTA_AV* p_av = (tBTA_AV*)p_data;

  BTIF_TRACE_DEBUG("%s: event=%s flags=0x%x", __func__,
                   dump_av_sm_event_name((btif_av_sm_event_t)event),
                   btif_av_cb.flags);

 if ((event == BTA_AV_REMOTE_CMD_EVT) &&
 (btif_av_cb.flags & BTIF_AV_FLAG_REMOTE_SUSPEND) &&
 (p_av->remote_cmd.rc_id == BTA_AV_RC_PLAY)) {
    BTIF_TRACE_EVENT("%s: Resetting remote suspend flag on RC PLAY", __func__);
    btif_av_cb.flags &= ~BTIF_AV_FLAG_REMOTE_SUSPEND;
 }

 switch (event) {
 case BTIF_SM_ENTER_EVT:
      btif_av_cb.flags &= ~BTIF_AV_FLAG_PENDING_STOP;
      btif_av_cb.flags &= ~BTIF_AV_FLAG_PENDING_START;
 break;

 case BTIF_SM_EXIT_EVT:
      btif_av_cb.flags &= ~BTIF_AV_FLAG_PENDING_START;
 break;

 case BTIF_AV_START_STREAM_REQ_EVT:
 if (btif_av_cb.peer_sep != AVDT_TSEP_SRC) btif_a2dp_source_setup_codec();
      BTA_AvStart();
      btif_av_cb.flags |= BTIF_AV_FLAG_PENDING_START;
 break;

 case BTA_AV_START_EVT: {
      BTIF_TRACE_WARNING(
 "%s: BTA_AV_START_EVT status=%d suspending=%d initiator=%d "
 "flags=0x%x",
          __func__, p_av->start.status, p_av->start.suspending,
          p_av->start.initiator, btif_av_cb.flags);

 if ((p_av->start.status == BTA_SUCCESS) &&
 (p_av->start.suspending == true))
 return true;

  
 if (!(btif_av_cb.flags & BTIF_AV_FLAG_PENDING_START)) {
 if (btif_av_cb.peer_sep == AVDT_TSEP_SNK) {
          BTIF_TRACE_WARNING("%s: trigger suspend as remote initiated!!",
                             __func__);
          btif_dispatch_sm_event(BTIF_AV_SUSPEND_STREAM_REQ_EVT, NULL, 0);
 }
 }

  
 if (btif_av_cb.peer_sep == AVDT_TSEP_SNK) {
 if (btif_a2dp_on_started(
 &p_av->start,
 ((btif_av_cb.flags & BTIF_AV_FLAG_PENDING_START) != 0))) {
  
          btif_av_cb.flags &= ~BTIF_AV_FLAG_PENDING_START;
 }
 }

  
 if (p_av->start.status != BTA_AV_SUCCESS) return false;

 if (btif_av_cb.peer_sep == AVDT_TSEP_SRC) {
        btif_a2dp_sink_set_rx_flush(
 false);  
 }

  
 if (btif_av_cb.flags & BTIF_AV_FLAG_PENDING_START) {
 if (btif_av_cb.peer_sep == AVDT_TSEP_SNK)
          btif_a2dp_on_started(NULL, true);
  
 }
      btif_sm_change_state(btif_av_cb.sm_handle, BTIF_AV_STATE_STARTED);

 } break;

 case BTIF_AV_SOURCE_CONFIG_REQ_EVT:
      btif_update_source_codec(p_data);
 break;

 case BTIF_AV_SOURCE_CONFIG_UPDATED_EVT:
      btif_report_source_codec_state(p_data);
 break;

 case BTIF_AV_DISCONNECT_REQ_EVT:
      BTA_AvClose(btif_av_cb.bta_handle);
 if (btif_av_cb.peer_sep == AVDT_TSEP_SRC) {
        BTA_AvCloseRc(btif_av_cb.bta_handle);
 }

  
      btif_report_connection_state(BTAV_CONNECTION_STATE_DISCONNECTING,
 &(btif_av_cb.peer_bda));
 break;

 case BTA_AV_CLOSE_EVT:
  
      btif_a2dp_on_stopped(NULL);

  
      btif_report_connection_state(BTAV_CONNECTION_STATE_DISCONNECTED,
 &(btif_av_cb.peer_bda));

  
 if (btif_av_cb.flags & BTIF_AV_FLAG_PENDING_START) {
        btif_a2dp_command_ack(A2DP_CTRL_ACK_FAILURE);
  
 }
      btif_sm_change_state(btif_av_cb.sm_handle, BTIF_AV_STATE_IDLE);
 break;

 case BTA_AV_RECONFIG_EVT:
 if ((btif_av_cb.flags & BTIF_AV_FLAG_PENDING_START) &&
 (p_av->reconfig.status == BTA_AV_SUCCESS)) {
        APPL_TRACE_WARNING("reconfig done BTA_AVstart()");
        BTA_AvStart();
 } else if (btif_av_cb.flags & BTIF_AV_FLAG_PENDING_START) {
        btif_av_cb.flags &= ~BTIF_AV_FLAG_PENDING_START;
        btif_a2dp_command_ack(A2DP_CTRL_ACK_FAILURE);
 }
 break;

 case BTIF_AV_CONNECT_REQ_EVT: {
 btif_av_connect_req_t* connect_req_p = (btif_av_connect_req_t*)p_data;
 RawAddress& target_bda = *connect_req_p->target_bda;
 if (btif_av_cb.peer_bda == target_bda) {
        BTIF_TRACE_WARNING(
 "%s: Ignore BTIF_AV_CONNECT_REQ_EVT for same device: target_bda=%s",
            __func__, target_bda.ToString().c_str());
 } else {
        BTIF_TRACE_WARNING(
 "%s: Moved to opened by Other incoming Connect request: "
 "target_bda=%s",
            __func__, target_bda.ToString().c_str());
        btif_report_connection_state(BTAV_CONNECTION_STATE_DISCONNECTED,
 &target_bda);
 }
      btif_queue_advance();
 } break;

 case BTIF_AV_OFFLOAD_START_REQ_EVT:
      BTIF_TRACE_ERROR(
 "%s: BTIF_AV_OFFLOAD_START_REQ_EVT: Stream not Started Opened",
          __func__);
      btif_a2dp_on_offload_started(BTA_AV_FAIL);
 break;

      CHECK_RC_EVENT(event, (tBTA_AV*)p_data);

 default:
      BTIF_TRACE_WARNING("%s: unhandled event=%s", __func__,
                         dump_av_sm_event_name((btif_av_sm_event_t)event));
 return false;
 }
 return true;
}
