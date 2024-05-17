void eager_reader_unregister(eager_reader_t *reader) {
  assert(reader != NULL);

 if (reader->outbound_registration) {
    reactor_unregister(reader->outbound_registration);
    reader->outbound_registration = NULL;
 }
}
