size_t eager_reader_read(eager_reader_t *reader, uint8_t *buffer, size_t max_size, bool block) {
  assert(reader != NULL);
  assert(buffer != NULL);

 if (!block && !has_byte(reader))
 return 0;

 eventfd_t bytes_available;
 if (eventfd_read(reader->bytes_available_fd, &bytes_available) == -1) {
    LOG_ERROR("%s unable to read semaphore for output data.", __func__);
 return 0;
 }

 if (max_size > bytes_available)
    max_size = bytes_available;

 size_t bytes_consumed = 0;
 while (bytes_consumed < max_size) {
 if (!reader->current_buffer)
      reader->current_buffer = fixed_queue_dequeue(reader->buffers);

 size_t bytes_to_copy = reader->current_buffer->length - reader->current_buffer->offset;
 if (bytes_to_copy > (max_size - bytes_consumed))
      bytes_to_copy = max_size - bytes_consumed;

    memcpy(&buffer[bytes_consumed], &reader->current_buffer->data[reader->current_buffer->offset], bytes_to_copy);
    bytes_consumed += bytes_to_copy;
    reader->current_buffer->offset += bytes_to_copy;

 if (reader->current_buffer->offset >= reader->current_buffer->length) {
      reader->allocator->free(reader->current_buffer);
      reader->current_buffer = NULL;
 }
 }

  bytes_available -= bytes_consumed;
 if (eventfd_write(reader->bytes_available_fd, bytes_available) == -1) {
    LOG_ERROR("%s unable to write back bytes available for output data.", __func__);
 }

 return bytes_consumed;
}
