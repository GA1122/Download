const btav_sink_interface_t* btif_av_get_sink_interface(void) {
  BTIF_TRACE_EVENT("%s", __func__);
 return &bt_av_sink_interface;
}
