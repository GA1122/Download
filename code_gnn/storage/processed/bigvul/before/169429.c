int ChunkedUploadDataStream::ReadChunk(IOBuffer* buf, int buf_len) {
  int bytes_read = 0;
  while (read_index_ < upload_data_.size() && bytes_read < buf_len) {
    std::vector<char>* data = upload_data_[read_index_].get();
    size_t bytes_to_read =
        std::min(static_cast<size_t>(buf_len - bytes_read),
                 data->size() - read_offset_);
    memcpy(buf->data() + bytes_read, data->data() + read_offset_,
           bytes_to_read);
    bytes_read += bytes_to_read;
    read_offset_ += bytes_to_read;
    if (read_offset_ == data->size()) {
      read_index_++;
      read_offset_ = 0;
    }
  }
  DCHECK_LE(bytes_read, buf_len);

  if (bytes_read == 0 && !all_data_appended_)
    return ERR_IO_PENDING;

  if (read_index_ == upload_data_.size() && all_data_appended_)
    SetIsFinalChunk();
  return bytes_read;
}
