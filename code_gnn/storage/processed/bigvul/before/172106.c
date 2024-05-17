static void hal_close() {
  LOG_INFO("%s", __func__);

  eager_reader_free(uart_stream);
  vendor->send_command(VENDOR_CLOSE_USERIAL, NULL);
  uart_fd = INVALID_FD;
}
