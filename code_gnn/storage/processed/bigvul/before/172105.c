static void event_uart_has_bytes(eager_reader_t *reader, UNUSED_ATTR void *context) {
 if (stream_has_interpretation) {
    callbacks->data_ready(current_data_type);
 } else {
 uint8_t type_byte;
 if (eager_reader_read(reader, &type_byte, 1, true) == 0) {
      LOG_ERROR("%s could not read HCI message type", __func__);
 return;
 }

 if (stream_corrupted_during_le_scan_workaround(type_byte))
 return;

 if (type_byte < DATA_TYPE_ACL || type_byte > DATA_TYPE_EVENT) {
      LOG_ERROR("%s Unknown HCI message type. Dropping this byte 0x%x, min %x, max %x", __func__, type_byte, DATA_TYPE_ACL, DATA_TYPE_EVENT);
 return;
 }

    stream_has_interpretation = true;
    current_data_type = type_byte;
 }
}
