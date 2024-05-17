void FileReaderLoader::OnStartLoading(uint64_t total_bytes) {
  total_bytes_ = total_bytes;

  DCHECK(!raw_data_);

  if (read_type_ != kReadByClient) {
    if (total_bytes > std::numeric_limits<unsigned>::max()) {
      Failed(FileErrorCode::kNotReadableErr, FailureType::kTotalBytesTooLarge);
      return;
    }

    raw_data_ = std::make_unique<ArrayBufferBuilder>(total_bytes);
    if (!raw_data_->IsValid()) {
      Failed(FileErrorCode::kNotReadableErr,
             FailureType::kArrayBufferBuilderCreation);
      return;
    }
    raw_data_->SetVariableCapacity(false);
  }

  if (client_)
    client_->DidStartLoading();
}
