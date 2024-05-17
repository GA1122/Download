void DownloadItemImpl::OnContentCheckCompleted(DownloadDangerType danger_type,
                                               DownloadInterruptReason reason) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(AllDataSaved());

  DCHECK_EQ(state_, IN_PROGRESS_INTERNAL);
  DVLOG(20) << __func__ << "() danger_type=" << danger_type
            << " download=" << DebugString(true);
  SetDangerType(danger_type);
  if (reason != DOWNLOAD_INTERRUPT_REASON_NONE) {
    InterruptAndDiscardPartialState(reason);
    DCHECK_EQ(RESUME_MODE_INVALID, GetResumeMode());
  }
  UpdateObservers();
}
