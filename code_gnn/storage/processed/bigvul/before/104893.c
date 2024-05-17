bool PPB_URLLoader_Impl::RecordDownloadProgress() const {
  return request_info_ && request_info_->record_download_progress();
}
