void eager_reader_register(eager_reader_t *reader, reactor_t *reactor, eager_reader_cb read_cb, void *context) {
  assert(reader != NULL);
  assert(reactor != NULL);
  assert(read_cb != NULL);

  eager_reader_unregister(reader);

  reader->outbound_read_ready = read_cb;
  reader->outbound_context = context;
  reader->outbound_registration = reactor_register(reactor, reader->bytes_available_fd, reader, internal_outbound_read_ready, NULL);
}
