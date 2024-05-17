void DownloadItemImpl::DestinationUpdate(
    int64_t bytes_so_far,
    int64_t bytes_per_sec,
    const std::vector<DownloadItem::ReceivedSlice>& received_slices) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(state_ == TARGET_PENDING_INTERNAL || state_ == IN_PROGRESS_INTERNAL);

  DCHECK_EQ(deferred_interrupt_reason_, DOWNLOAD_INTERRUPT_REASON_NONE);

  DVLOG(20) << __func__ << "() so_far=" << bytes_so_far
            << " per_sec=" << bytes_per_sec
            << " download=" << DebugString(true);

  UpdateProgress(bytes_so_far, bytes_per_sec);
  received_slices_ = received_slices;
  TRACE_EVENT_INSTANT1("download", "DownloadItemUpdated",
                       TRACE_EVENT_SCOPE_THREAD, "bytes_so_far",
                       GetReceivedBytes());

  UpdateObservers();
}
