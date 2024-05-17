void DownloadItemImpl::TransitionTo(DownloadInternalState new_state) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (state_ == new_state)
    return;

  DownloadInternalState old_state = state_;
  state_ = new_state;

  DCHECK(IsSavePackageDownload()
             ? IsValidSavePackageStateTransition(old_state, new_state)
             : IsValidStateTransition(old_state, new_state))
      << "Invalid state transition from:" << DebugDownloadStateString(old_state)
      << " to:" << DebugDownloadStateString(new_state);

  switch (state_) {
    case INITIAL_INTERNAL:
      NOTREACHED();
      break;

    case TARGET_PENDING_INTERNAL:
    case TARGET_RESOLVED_INTERNAL:
      break;

    case INTERRUPTED_TARGET_PENDING_INTERNAL:
      DCHECK_NE(DOWNLOAD_INTERRUPT_REASON_NONE, deferred_interrupt_reason_)
          << "Interrupt reason must be set prior to transitioning into "
             "TARGET_PENDING";
      break;

    case IN_PROGRESS_INTERNAL:
      DCHECK(!GetFullPath().empty()) << "Current output path must be known.";
      DCHECK(!GetTargetFilePath().empty()) << "Target path must be known.";
      DCHECK(GetFullPath().DirName() == GetTargetFilePath().DirName())
          << "Current output directory must match target directory.";
      DCHECK(download_file_) << "Output file must be owned by download item.";
      DCHECK(job_) << "Must have active download job.";
      DCHECK(!job_->is_paused())
          << "At the time a download enters IN_PROGRESS state, "
             "it must not be paused.";
      break;

    case COMPLETING_INTERNAL:
      DCHECK(AllDataSaved()) << "All data must be saved prior to completion.";
      DCHECK(!download_file_)
          << "Download file must be released prior to completion.";
      DCHECK(!GetTargetFilePath().empty()) << "Target path must be known.";
      DCHECK(GetFullPath() == GetTargetFilePath())
          << "Current output path must match target path.";

      TRACE_EVENT_INSTANT2("download", "DownloadItemCompleting",
                           TRACE_EVENT_SCOPE_THREAD, "bytes_so_far",
                           GetReceivedBytes(), "final_hash",
                           destination_info_.hash);
      break;

    case COMPLETE_INTERNAL:
      TRACE_EVENT_INSTANT1("download", "DownloadItemFinished",
                           TRACE_EVENT_SCOPE_THREAD, "auto_opened",
                           auto_opened_ ? "yes" : "no");
      break;

    case INTERRUPTED_INTERNAL:
      DCHECK(!download_file_)
          << "Download file must be released prior to interruption.";
      DCHECK_NE(last_reason_, DOWNLOAD_INTERRUPT_REASON_NONE);
      TRACE_EVENT_INSTANT2("download", "DownloadItemInterrupted",
                           TRACE_EVENT_SCOPE_THREAD, "interrupt_reason",
                           DownloadInterruptReasonToString(last_reason_),
                           "bytes_so_far", GetReceivedBytes());
      break;

    case RESUMING_INTERNAL:
      TRACE_EVENT_INSTANT2("download", "DownloadItemResumed",
                           TRACE_EVENT_SCOPE_THREAD, "interrupt_reason",
                           DownloadInterruptReasonToString(last_reason_),
                           "bytes_so_far", GetReceivedBytes());
      break;

    case CANCELLED_INTERNAL:
      TRACE_EVENT_INSTANT1("download", "DownloadItemCancelled",
                           TRACE_EVENT_SCOPE_THREAD, "bytes_so_far",
                           GetReceivedBytes());
      break;

    case MAX_DOWNLOAD_INTERNAL_STATE:
      NOTREACHED();
      break;
  }

  DVLOG(20) << __func__ << "() from:" << DebugDownloadStateString(old_state)
            << " to:" << DebugDownloadStateString(state_)
            << " this = " << DebugString(true);
  bool is_done =
      (state_ == COMPLETE_INTERNAL || state_ == INTERRUPTED_INTERNAL ||
       state_ == RESUMING_INTERNAL || state_ == CANCELLED_INTERNAL);
  bool was_done =
      (old_state == COMPLETE_INTERNAL || old_state == INTERRUPTED_INTERNAL ||
       old_state == RESUMING_INTERNAL || old_state == CANCELLED_INTERNAL);

  if (is_done && !was_done)
    TRACE_EVENT_ASYNC_END0("download", "DownloadItemActive", download_id_);

  if (was_done && !is_done) {
    std::string file_name(GetTargetFilePath().BaseName().AsUTF8Unsafe());
    TRACE_EVENT_NESTABLE_ASYNC_BEGIN1(
        "download", "DownloadItemActive", download_id_, "download_item",
        std::make_unique<DownloadItemActivatedData>(
            TYPE_ACTIVE_DOWNLOAD, GetId(), GetOriginalUrl().spec(),
            GetURL().spec(), file_name, GetDangerType(), GetReceivedBytes(),
            HasUserGesture()));
  }
}
