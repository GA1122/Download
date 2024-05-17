void AppCacheUpdateJob::OnManifestDataReadComplete(int result) {
  if (result > 0) {
    loaded_manifest_data_.append(read_manifest_buffer_->data(), result);
    manifest_response_reader_->ReadData(
        read_manifest_buffer_.get(), kAppCacheFetchBufferSize,
        base::BindOnce(&AppCacheUpdateJob::OnManifestDataReadComplete,
                       base::Unretained(this)));   
  } else {
    read_manifest_buffer_ = nullptr;
    manifest_response_reader_.reset();
    ContinueHandleManifestFetchCompleted(
        result < 0 || manifest_data_ != loaded_manifest_data_);
  }
}
