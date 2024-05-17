bool ChunkedUploadDataStream::Writer::AppendData(const char* data,
                                                 int data_len,
                                                 bool is_done) {
  if (!upload_data_stream_)
    return false;
  upload_data_stream_->AppendData(data, data_len, is_done);
  return true;
}
