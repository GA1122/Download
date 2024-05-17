static void cleanup_src(void) {
  BTIF_TRACE_EVENT("%s", __func__);

  btif_queue_cleanup(UUID_SERVCLASS_AUDIO_SOURCE);
 if (bt_av_src_callbacks) {
    bt_av_src_callbacks = NULL;
 if (bt_av_sink_callbacks == NULL) cleanup(BTA_A2DP_SOURCE_SERVICE_ID);
 }
}
