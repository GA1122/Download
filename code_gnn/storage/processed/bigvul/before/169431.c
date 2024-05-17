void ChunkedUploadDataStream::ResetInternal() {
  read_buffer_ = NULL;
  read_buffer_len_ = 0;
  read_index_ = 0;
  read_offset_ = 0;
}
