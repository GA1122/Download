void BackendIO::OpenNextEntry(Rankings::Iterator* iterator,
                              Entry** next_entry) {
  operation_ = OP_OPEN_NEXT;
  iterator_ = iterator;
  entry_ptr_ = next_entry;
}
