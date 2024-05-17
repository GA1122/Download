int ChunkedUploadDataStream::InitInternal(const NetLogWithSource& net_log) {
  DCHECK(!read_buffer_.get());
  DCHECK_EQ(0u, read_index_);
  DCHECK_EQ(0u, read_offset_);
  return OK;
}
