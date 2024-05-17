static bt_status_t init_sink(btav_sink_callbacks_t* callbacks) {
  BTIF_TRACE_EVENT("%s", __func__);

 bt_status_t status = btif_av_init(BTA_A2DP_SINK_SERVICE_ID);
 if (status == BT_STATUS_SUCCESS) bt_av_sink_callbacks = callbacks;

 return status;
}
