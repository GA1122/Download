static void hal_close() {
  LOG_INFO("%s", __func__);

  eager_reader_free(event_stream);
  eager_reader_free(acl_stream);
  vendor->send_command(VENDOR_CLOSE_USERIAL, NULL);

 for (int i = 0; i < CH_MAX; i++)
    uart_fds[i] = INVALID_FD;
}
