void eager_reader_free(eager_reader_t *reader) {
 if (!reader)
 return;

  eager_reader_unregister(reader);

 if (reader->inbound_read_object)
    reactor_unregister(reader->inbound_read_object);

 if (reader->bytes_available_fd != INVALID_FD)
    close(reader->bytes_available_fd);

 if (reader->current_buffer)
    reader->allocator->free(reader->current_buffer);

  fixed_queue_free(reader->buffers, reader->allocator->free);
  thread_free(reader->inbound_read_thread);
  osi_free(reader);
}
