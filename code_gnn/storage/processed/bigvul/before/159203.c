void DownloadItemImpl::UpdateProgress(
    int64_t bytes_so_far,
    int64_t bytes_per_sec) {
  destination_info_.received_bytes = bytes_so_far;
  bytes_per_sec_ = bytes_per_sec;

  if (bytes_so_far > total_bytes_)
    total_bytes_ = 0;
}
