void DownloadItemImpl::InterruptWithPartialState(
    int64_t bytes_so_far,
    std::unique_ptr<crypto::SecureHash> hash_state,
    DownloadInterruptReason reason) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK_NE(DOWNLOAD_INTERRUPT_REASON_NONE, reason);
  DVLOG(20) << __func__
            << "() reason:" << DownloadInterruptReasonToString(reason)
            << " bytes_so_far:" << bytes_so_far
            << " hash_state:" << (hash_state ? "Valid" : "Invalid")
            << " this=" << DebugString(true);

  switch (state_) {
    case CANCELLED_INTERNAL:
    case COMPLETING_INTERNAL:
    case COMPLETE_INTERNAL:
      return;

    case INITIAL_INTERNAL:
    case MAX_DOWNLOAD_INTERNAL_STATE:
      NOTREACHED();
      return;

    case TARGET_PENDING_INTERNAL:
    case INTERRUPTED_TARGET_PENDING_INTERNAL:
      if (!IsCancellation(reason)) {
        UpdateProgress(bytes_so_far, 0);
        SetHashState(std::move(hash_state));
        deferred_interrupt_reason_ = reason;
        TransitionTo(INTERRUPTED_TARGET_PENDING_INTERNAL);
        return;
      }

    case IN_PROGRESS_INTERNAL:
    case TARGET_RESOLVED_INTERNAL:
      last_reason_ = reason;

      if (download_file_) {
        ResumeMode resume_mode = GetResumeMode();
        ReleaseDownloadFile(resume_mode != RESUME_MODE_IMMEDIATE_CONTINUE &&
                            resume_mode != RESUME_MODE_USER_CONTINUE);
      }
      break;

    case RESUMING_INTERNAL:
    case INTERRUPTED_INTERNAL:
      DCHECK(!download_file_);
      if (!IsCancellation(reason))
        return;

      last_reason_ = reason;
      if (!GetFullPath().empty()) {
        GetDownloadTaskRunner()->PostTask(
            FROM_HERE, base::BindOnce(base::IgnoreResult(&DeleteDownloadedFile),
                                      GetFullPath()));
        destination_info_.current_path.clear();
      }
      break;
  }

  destination_info_.all_data_saved = false;

  if (GetFullPath().empty()) {
    hash_state_.reset();
    destination_info_.hash.clear();
    destination_info_.received_bytes = 0;
    received_slices_.clear();
  } else {
    UpdateProgress(bytes_so_far, 0);
    SetHashState(std::move(hash_state));
  }

  if (job_)
    job_->Cancel(false);

  if (IsCancellation(reason)) {
    if (IsDangerous()) {
      RecordDangerousDownloadDiscard(
          reason == DOWNLOAD_INTERRUPT_REASON_USER_CANCELED
              ? DOWNLOAD_DISCARD_DUE_TO_USER_ACTION
              : DOWNLOAD_DISCARD_DUE_TO_SHUTDOWN,
          GetDangerType(), GetTargetFilePath());
    }

    RecordDownloadCount(CANCELLED_COUNT);
    if (job_ && job_->IsParallelizable()) {
      RecordParallelizableDownloadCount(CANCELLED_COUNT,
                                        IsParallelDownloadEnabled());
    }
    DCHECK_EQ(last_reason_, reason);
    TransitionTo(CANCELLED_INTERNAL);
    return;
  }

  RecordDownloadInterrupted(reason, GetReceivedBytes(), total_bytes_,
                            job_ && job_->IsParallelizable(),
                            IsParallelDownloadEnabled());
  if (reason == DOWNLOAD_INTERRUPT_REASON_SERVER_CONTENT_LENGTH_MISMATCH)
    received_bytes_at_length_mismatch_ = GetReceivedBytes();

  if (!GetWebContents())
    RecordDownloadCount(INTERRUPTED_WITHOUT_WEBCONTENTS);

  DCHECK_EQ(last_reason_, reason);
  TransitionTo(INTERRUPTED_INTERNAL);
  AutoResumeIfValid();
}
