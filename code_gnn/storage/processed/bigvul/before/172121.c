static size_t read_data(serial_data_type_t type, uint8_t *buffer, size_t max_size, bool block) {
 if (type == DATA_TYPE_ACL) {
 return eager_reader_read(acl_stream, buffer, max_size, block);
 } else if (type == DATA_TYPE_EVENT) {
 return eager_reader_read(event_stream, buffer, max_size, block);
 }

  LOG_ERROR("%s invalid data type: %d", __func__, type);
 return 0;
}
