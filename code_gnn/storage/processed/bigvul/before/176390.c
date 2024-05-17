const btav_source_interface_t* btif_av_get_src_interface(void) {
  BTIF_TRACE_EVENT("%s", __func__);
 return &bt_av_src_interface;
}
