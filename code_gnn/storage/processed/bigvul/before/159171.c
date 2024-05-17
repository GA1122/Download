void DownloadItemImpl::OnAllDataSaved(
    int64_t total_bytes,
    std::unique_ptr<crypto::SecureHash> hash_state) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!AllDataSaved());
  destination_info_.all_data_saved = true;
  SetTotalBytes(total_bytes);
  UpdateProgress(total_bytes, 0);
  received_slices_.clear();
  SetHashState(std::move(hash_state));
  hash_state_.reset();   

  if (received_bytes_at_length_mismatch_ > 0) {
    if (total_bytes > received_bytes_at_length_mismatch_) {
      RecordDownloadCount(
          MORE_BYTES_RECEIVED_AFTER_CONTENT_LENGTH_MISMATCH_COUNT);
    } else if (total_bytes == received_bytes_at_length_mismatch_) {
      RecordDownloadCount(
          NO_BYTES_RECEIVED_AFTER_CONTENT_LENGTH_MISMATCH_COUNT);
    } else {
    }
  }
  DVLOG(20) << __func__ << "() download=" << DebugString(true);
  UpdateObservers();
}
