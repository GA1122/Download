static void event_postload(UNUSED_ATTR void *context) {
  LOG_INFO("%s", __func__);
 if(vendor->send_async_command(VENDOR_CONFIGURE_SCO, NULL) == -1) {
    sco_config_callback(false);

 }
}
