void BackendIO::ReadSparseData(EntryImpl* entry,
                               int64_t offset,
                               net::IOBuffer* buf,
                               int buf_len) {
  operation_ = OP_READ_SPARSE;
  entry_ = entry;
  offset64_ = offset;
  buf_ = buf;
  buf_len_ = buf_len;
}
