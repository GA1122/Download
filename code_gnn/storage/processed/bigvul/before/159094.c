int64_t DownloadItemImpl::CurrentSpeed() const {
  if (IsPaused())
    return 0;
  return bytes_per_sec_;
}
