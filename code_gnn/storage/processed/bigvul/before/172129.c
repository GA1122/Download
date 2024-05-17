static void event_finish_startup(UNUSED_ATTR void *context) {
  LOG_INFO("%s", __func__);
  hal->open();
  vendor->send_async_command(VENDOR_CONFIGURE_FIRMWARE, NULL);
}
