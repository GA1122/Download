static size_t read_data(serial_data_type_t type, uint8_t *buffer, size_t max_size, bool block) {
 if (type < DATA_TYPE_ACL || type > DATA_TYPE_EVENT) {
    LOG_ERROR("%s invalid data type: %d", __func__, type);
 return 0;
 } else if (!stream_has_interpretation) {
    LOG_ERROR("%s with no valid stream intepretation.", __func__);
 return 0;
 } else if (current_data_type != type) {
    LOG_ERROR("%s with different type than existing interpretation.", __func__);
 return 0;
 }

 return eager_reader_read(uart_stream, buffer, max_size, block);
}
