bool PPB_URLLoader_Impl::RecordUploadProgress() const {
  return request_info_ && request_info_->record_upload_progress();
}
