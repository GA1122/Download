static bool hal_open() {
  LOG_INFO("%s", __func__);

 int fd_array[CH_MAX];
 int number_of_ports = vendor->send_command(VENDOR_OPEN_USERIAL, &fd_array);

 if (number_of_ports != 1) {
    LOG_ERROR("%s opened the wrong number of ports: got %d, expected 1.", __func__, number_of_ports);
 goto error;
 }

  uart_fd = fd_array[0];
 if (uart_fd == INVALID_FD) {
    LOG_ERROR("%s unable to open the uart serial port.", __func__);
 goto error;
 }

  uart_stream = eager_reader_new(uart_fd, &allocator_malloc, HCI_HAL_SERIAL_BUFFER_SIZE, SIZE_MAX, "hci_single_channel");
 if (!uart_stream) {
    LOG_ERROR("%s unable to create eager reader for the uart serial port.", __func__);
 goto error;
 }

  stream_has_interpretation = false;
  stream_corruption_detected = false;
  stream_corruption_bytes_to_ignore = 0;
  eager_reader_register(uart_stream, thread_get_reactor(thread), event_uart_has_bytes, NULL);

  thread_set_priority(thread, HCI_THREAD_PRIORITY);
  thread_set_priority(eager_reader_get_read_thread(uart_stream), HCI_THREAD_PRIORITY);

 return true;

error:
  interface.close();
 return false;
}
