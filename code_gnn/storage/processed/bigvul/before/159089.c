void DownloadItemImpl::AutoResumeIfValid() {
  DVLOG(20) << __func__ << "() " << DebugString(true);
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  ResumeMode mode = GetResumeMode();

  if (mode != RESUME_MODE_IMMEDIATE_RESTART &&
      mode != RESUME_MODE_IMMEDIATE_CONTINUE) {
    return;
  }

  auto_resume_count_++;

  ResumeInterruptedDownload(ResumptionRequestSource::AUTOMATIC);
}
