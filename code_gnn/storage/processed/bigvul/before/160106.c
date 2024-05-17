void BackendImpl::TooMuchStorageRequested(int32_t size) {
  stats_.ModifyStorageStats(0, size);
}
