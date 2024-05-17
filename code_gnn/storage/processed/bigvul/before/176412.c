static void cleanup(int service_uuid) {
  BTIF_TRACE_EVENT("%s", __func__);

  btif_transfer_context(btif_av_handle_event, BTIF_AV_CLEANUP_REQ_EVT, NULL, 0,
                        NULL);

  btif_disable_service(service_uuid);

  alarm_free(av_open_on_rc_timer);
  av_open_on_rc_timer = NULL;

  
  btif_sm_shutdown(btif_av_cb.sm_handle);
  btif_av_cb.sm_handle = NULL;
}
