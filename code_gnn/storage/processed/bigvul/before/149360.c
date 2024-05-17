void BinaryUploadService::RunAuthorizationCallbacks() {
  DCHECK(can_upload_data_.has_value());
  for (auto& callback : authorization_callbacks_) {
    std::move(callback).Run(can_upload_data_.value());
  }
  authorization_callbacks_.clear();
}
