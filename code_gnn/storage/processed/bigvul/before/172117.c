static bool hal_open() {
  LOG_INFO("%s", __func__);

 int number_of_ports = vendor->send_command(VENDOR_OPEN_USERIAL, &uart_fds);

 if (number_of_ports != 2 && number_of_ports != 4) {
    LOG_ERROR("%s opened the wrong number of ports: got %d, expected 2 or 4.", __func__, number_of_ports);
 goto error;
 }

  LOG_INFO("%s got uart fds: CMD=%d, EVT=%d, ACL_OUT=%d, ACL_IN=%d",
      __func__, uart_fds[CH_CMD], uart_fds[CH_EVT], uart_fds[CH_ACL_OUT], uart_fds[CH_ACL_IN]);

 if (uart_fds[CH_CMD] == INVALID_FD) {
    LOG_ERROR("%s unable to open the command uart serial port.", __func__);
 goto error;
 }

 if (uart_fds[CH_EVT] == INVALID_FD) {
    LOG_ERROR("%s unable to open the event uart serial port.", __func__);
 goto error;
 }

 if (uart_fds[CH_ACL_OUT] == INVALID_FD) {
    LOG_ERROR("%s unable to open the acl-out uart serial port.", __func__);
 goto error;
 }

 if (uart_fds[CH_ACL_IN] == INVALID_FD) {
    LOG_ERROR("%s unable to open the acl-in uart serial port.", __func__);
 goto error;
 }

  event_stream = eager_reader_new(uart_fds[CH_EVT], &allocator_malloc, HCI_HAL_SERIAL_BUFFER_SIZE, SIZE_MAX, "hci_mct");
 if (!event_stream) {
    LOG_ERROR("%s unable to create eager reader for the event uart serial port.", __func__);
 goto error;
 }

  acl_stream = eager_reader_new(uart_fds[CH_ACL_IN], &allocator_malloc, HCI_HAL_SERIAL_BUFFER_SIZE, SIZE_MAX, "hci_mct");
 if (!event_stream) {
    LOG_ERROR("%s unable to create eager reader for the acl-in uart serial port.", __func__);
 goto error;
 }

  eager_reader_register(event_stream, thread_get_reactor(thread), event_event_stream_has_bytes, NULL);
  eager_reader_register(acl_stream, thread_get_reactor(thread), event_acl_stream_has_bytes, NULL);

 return true;

error:;
  interface.close();
 return false;
}
