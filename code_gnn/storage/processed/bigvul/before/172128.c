static void event_epilog(UNUSED_ATTR void *context) {
  vendor->send_async_command(VENDOR_DO_EPILOG, NULL);
}
