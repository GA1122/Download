static void internal_outbound_read_ready(void *context) {
  assert(context != NULL);

 eager_reader_t *reader = (eager_reader_t *)context;
  reader->outbound_read_ready(reader, reader->outbound_context);
}
