void BackendIO::WriteData(EntryImpl* entry, int index, int offset,
                          net::IOBuffer* buf, int buf_len, bool truncate) {
  operation_ = OP_WRITE;
  entry_ = entry;
  index_ = index;
  offset_ = offset;
  buf_ = buf;
  buf_len_ = buf_len;
  truncate_ = truncate;
}
