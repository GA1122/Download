void FileReaderLoader::OnReceivedData(const char* data, unsigned data_length) {
  DCHECK(data);

  if (error_code_ != FileErrorCode::kOK)
    return;

  if (read_type_ == kReadByClient) {
    bytes_loaded_ += data_length;

    if (client_)
      client_->DidReceiveDataForClient(data, data_length);
    return;
  }

  unsigned bytes_appended = raw_data_->Append(data, data_length);
  if (!bytes_appended) {
    raw_data_.reset();
    bytes_loaded_ = 0;
    Failed(FileErrorCode::kNotReadableErr,
           FailureType::kArrayBufferBuilderAppend);
    return;
  }
  bytes_loaded_ += bytes_appended;
  is_raw_data_converted_ = false;
  AdjustReportedMemoryUsageToV8(bytes_appended);

  if (client_)
    client_->DidReceiveData();
}
