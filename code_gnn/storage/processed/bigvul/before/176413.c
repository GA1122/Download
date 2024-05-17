static void cleanup_sink(void) {
  BTIF_TRACE_EVENT("%s", __func__);

  btif_queue_cleanup(UUID_SERVCLASS_AUDIO_SINK);
 if (bt_av_sink_callbacks) {
    bt_av_sink_callbacks = NULL;
 if (bt_av_src_callbacks == NULL) cleanup(BTA_A2DP_SINK_SERVICE_ID);
 }
}
