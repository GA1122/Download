void DownloadItemImpl::Resume() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DVLOG(20) << __func__ << "() download = " << DebugString(true);
  switch (state_) {
    case CANCELLED_INTERNAL:   
    case COMPLETE_INTERNAL:
    case COMPLETING_INTERNAL:
    case INITIAL_INTERNAL:
    case INTERRUPTED_TARGET_PENDING_INTERNAL:
    case RESUMING_INTERNAL:    
      return;

    case TARGET_PENDING_INTERNAL:
    case IN_PROGRESS_INTERNAL:
      if (!IsPaused())
        return;
      if (job_)
        job_->Resume(true);
      UpdateObservers();
      return;

    case INTERRUPTED_INTERNAL:
      auto_resume_count_ = 0;   
      ResumeInterruptedDownload(ResumptionRequestSource::USER);
      UpdateObservers();
      return;

    case MAX_DOWNLOAD_INTERNAL_STATE:
    case TARGET_RESOLVED_INTERNAL:
      NOTREACHED();
  }
}
