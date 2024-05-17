static void btif_av_handle_event(uint16_t event, char* p_param) {
  BTIF_TRACE_EVENT("%s: event=%s", __func__,
                   dump_av_sm_event_name((btif_av_sm_event_t)event));
 switch (event) {
 case BTIF_AV_CLEANUP_REQ_EVT:
      btif_a2dp_source_shutdown();
      btif_a2dp_sink_shutdown();
 break;

 case BTA_AV_REGISTER_EVT:
 if (btif_av_cb.sm_handle == NULL) {
        btif_av_cb.bta_handle = ((tBTA_AV*)p_param)->registr.hndl;
        BTIF_TRACE_DEBUG("%s: BTA AV Handle updated", __func__);
 }
  
 default:
      btif_sm_dispatch(btif_av_cb.sm_handle, event, (void*)p_param);
      btif_av_event_free_data(event, p_param);
 }
}
