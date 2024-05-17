static bool btif_av_state_started_handler(btif_sm_event_t event, void* p_data) {
  tBTA_AV* p_av = (tBTA_AV*)p_data;

  BTIF_TRACE_DEBUG("%s: event=%s flags=0x%x", __func__,
                   dump_av_sm_event_name((btif_av_sm_event_t)event),
                   btif_av_cb.flags);

 switch (event) {
 case BTIF_SM_ENTER_EVT:

  
      btif_av_cb.flags &= ~BTIF_AV_FLAG_REMOTE_SUSPEND;

  
      btif_report_audio_state(BTAV_AUDIO_STATE_STARTED, &(btif_av_cb.peer_bda));
 break;

 case BTIF_SM_EXIT_EVT:
 break;

 case BTIF_AV_START_STREAM_REQ_EVT:
  
 if (btif_av_cb.peer_sep == AVDT_TSEP_SNK)
        btif_a2dp_on_started(NULL, true);
 break;

 case BTIF_AV_SOURCE_CONFIG_REQ_EVT:
      btif_update_source_codec(p_data);
 break;

 case BTIF_AV_SOURCE_CONFIG_UPDATED_EVT:
      btif_report_source_codec_state(p_data);
 break;

  
 case BTIF_AV_STOP_STREAM_REQ_EVT:
 case BTIF_AV_SUSPEND_STREAM_REQ_EVT:
      BTIF_TRACE_WARNING("%s: event=%s flags=0x%x", __func__,
                         dump_av_sm_event_name((btif_av_sm_event_t)event),
                         btif_av_cb.flags);
  
      btif_av_cb.flags |= BTIF_AV_FLAG_LOCAL_SUSPEND_PENDING;

  
      btif_av_cb.flags &= ~BTIF_AV_FLAG_REMOTE_SUSPEND;

 if (btif_av_cb.peer_sep == AVDT_TSEP_SNK) {
  
        btif_a2dp_source_set_tx_flush(true);
 }

 if (btif_av_cb.peer_sep == AVDT_TSEP_SRC) {
        btif_a2dp_on_stopped(NULL);
 }

      BTA_AvStop(true);
 break;

 case BTIF_AV_DISCONNECT_REQ_EVT:
      BTIF_TRACE_WARNING("%s: event=%s flags=0x%x", __func__,
                         dump_av_sm_event_name((btif_av_sm_event_t)event),
                         btif_av_cb.flags);

  
      BTA_AvClose(btif_av_cb.bta_handle);
 if (btif_av_cb.peer_sep == AVDT_TSEP_SRC) {
        BTA_AvCloseRc(btif_av_cb.bta_handle);
 }

  
      btif_report_connection_state(BTAV_CONNECTION_STATE_DISCONNECTING,
 &(btif_av_cb.peer_bda));

  
      btif_sm_change_state(btif_av_cb.sm_handle, BTIF_AV_STATE_CLOSING);
 break;

 case BTA_AV_SUSPEND_EVT:
      BTIF_TRACE_WARNING(
 "%s: BTA_AV_SUSPEND_EVT status=%d initiator=%d flags=0x%x", __func__,
          p_av->suspend.status, p_av->suspend.initiator, btif_av_cb.flags);

  
      btif_a2dp_on_suspended(&p_av->suspend);

  
 if (p_av->suspend.status != BTA_AV_SUCCESS) {
        btif_av_cb.flags &= ~BTIF_AV_FLAG_LOCAL_SUSPEND_PENDING;

 if (btif_av_cb.peer_sep == AVDT_TSEP_SNK) {
  
          btif_a2dp_source_set_tx_flush(false);
 }
 return false;
 }

 if (p_av->suspend.initiator != true) {
  

  
 if ((btif_av_cb.flags & BTIF_AV_FLAG_LOCAL_SUSPEND_PENDING) == 0)
          btif_av_cb.flags |= BTIF_AV_FLAG_REMOTE_SUSPEND;

        btif_report_audio_state(BTAV_AUDIO_STATE_REMOTE_SUSPEND,
 &(btif_av_cb.peer_bda));
 } else {
        btif_report_audio_state(BTAV_AUDIO_STATE_STOPPED,
 &(btif_av_cb.peer_bda));
 }

      btif_sm_change_state(btif_av_cb.sm_handle, BTIF_AV_STATE_OPENED);

  
      btif_av_cb.flags &= ~BTIF_AV_FLAG_LOCAL_SUSPEND_PENDING;
 break;

 case BTA_AV_STOP_EVT:
      BTIF_TRACE_WARNING("%s: event=%s flags=0x%x", __func__,
                         dump_av_sm_event_name((btif_av_sm_event_t)event),
                         btif_av_cb.flags);

      btif_av_cb.flags |= BTIF_AV_FLAG_PENDING_STOP;
      btif_a2dp_on_stopped(&p_av->suspend);

      btif_report_audio_state(BTAV_AUDIO_STATE_STOPPED, &(btif_av_cb.peer_bda));

  
 if (p_av->suspend.status == BTA_AV_SUCCESS)
        btif_sm_change_state(btif_av_cb.sm_handle, BTIF_AV_STATE_OPENED);

 break;

 case BTA_AV_CLOSE_EVT:
      BTIF_TRACE_WARNING("%s: event=%s flags=0x%x", __func__,
                         dump_av_sm_event_name((btif_av_sm_event_t)event),
                         btif_av_cb.flags);

      btif_av_cb.flags |= BTIF_AV_FLAG_PENDING_STOP;

  
      btif_a2dp_on_stopped(NULL);

  
      btif_report_connection_state(BTAV_CONNECTION_STATE_DISCONNECTED,
 &(btif_av_cb.peer_bda));

      btif_sm_change_state(btif_av_cb.sm_handle, BTIF_AV_STATE_IDLE);
 break;

 case BTIF_AV_OFFLOAD_START_REQ_EVT:
      BTA_AvOffloadStart(btif_av_cb.bta_handle);
 break;

 case BTA_AV_OFFLOAD_START_RSP_EVT:
      btif_a2dp_on_offload_started(p_av->status);
 break;

      CHECK_RC_EVENT(event, (tBTA_AV*)p_data);

 default:
      BTIF_TRACE_WARNING("%s: unhandled event=%s", __func__,
                         dump_av_sm_event_name((btif_av_sm_event_t)event));
 return false;
 }

 return true;
}
