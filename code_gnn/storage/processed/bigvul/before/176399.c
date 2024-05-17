static bool btif_av_state_closing_handler(btif_sm_event_t event, void* p_data) {
  BTIF_TRACE_DEBUG("%s: event=%s flags=0x%x", __func__,
                   dump_av_sm_event_name((btif_av_sm_event_t)event),
                   btif_av_cb.flags);

 switch (event) {
 case BTIF_SM_ENTER_EVT:
 if (btif_av_cb.peer_sep == AVDT_TSEP_SNK) {
  
        btif_a2dp_source_set_tx_flush(true);
  
 }
 if (btif_av_cb.peer_sep == AVDT_TSEP_SRC) {
        btif_a2dp_sink_set_rx_flush(true);
 }
 break;

 case BTA_AV_STOP_EVT:
 case BTIF_AV_STOP_STREAM_REQ_EVT:
      btif_a2dp_on_stopped(NULL);
 break;

 case BTIF_SM_EXIT_EVT:
 break;

 case BTIF_AV_SOURCE_CONFIG_REQ_EVT:
      btif_update_source_codec(p_data);
 break;

 case BTIF_AV_SOURCE_CONFIG_UPDATED_EVT:
      btif_report_source_codec_state(p_data);
 break;

 case BTA_AV_CLOSE_EVT:

  
      btif_report_connection_state(BTAV_CONNECTION_STATE_DISCONNECTED,
 &(btif_av_cb.peer_bda));

      btif_sm_change_state(btif_av_cb.sm_handle, BTIF_AV_STATE_IDLE);
 break;

  
 case BTA_AV_RC_CLOSE_EVT:
      btif_rc_handler(event, (tBTA_AV*)p_data);
 break;

  
 case BTA_AV_RC_BROWSE_CLOSE_EVT:
      btif_rc_handler(event, (tBTA_AV*)p_data);
 break;

 case BTIF_AV_OFFLOAD_START_REQ_EVT:
      BTIF_TRACE_ERROR(
 "%s: BTIF_AV_OFFLOAD_START_REQ_EVT: Stream not Started Closing",
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
