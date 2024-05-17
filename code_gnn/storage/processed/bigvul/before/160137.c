void BackendIO::GetAvailableRange(EntryImpl* entry,
                                  int64_t offset,
                                  int len,
                                  int64_t* start) {
  operation_ = OP_GET_RANGE;
  entry_ = entry;
  offset64_ = offset;
  buf_len_ = len;
  start_ = start;
}
