static void event_event_stream_has_bytes(UNUSED_ATTR eager_reader_t *reader, UNUSED_ATTR void *context) {
  callbacks->data_ready(DATA_TYPE_EVENT);
}
