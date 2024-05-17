bt_status_t btif_av_init(int service_id) {
 if (btif_av_cb.sm_handle == NULL) {
    alarm_free(av_open_on_rc_timer);
    av_open_on_rc_timer = alarm_new("btif_av.av_open_on_rc_timer");

 switch (service_id) {
 case BTA_A2DP_SOURCE_SERVICE_ID:
 if (!btif_a2dp_source_startup())
 return BT_STATUS_FAIL;  
 break;
 case BTA_A2DP_SINK_SERVICE_ID:
 if (!btif_a2dp_sink_startup())
 return BT_STATUS_FAIL;  
 break;
 default:
 break;
 }

    btif_enable_service(service_id);

  
    btif_av_cb.sm_handle = btif_sm_init(
 (const btif_sm_handler_t*)btif_av_state_handlers, BTIF_AV_STATE_IDLE);
 }

 return BT_STATUS_SUCCESS;
}
