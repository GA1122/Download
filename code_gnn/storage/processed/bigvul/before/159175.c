void DownloadItemImpl::OnDownloadRenamedToFinalName(
    DownloadInterruptReason reason,
    const base::FilePath& full_path) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!IsSavePackageDownload());

  if (state_ != IN_PROGRESS_INTERNAL)
    return;

  DVLOG(20) << __func__ << "() full_path = \"" << full_path.value() << "\" "
            << DebugString(false);

  if (DOWNLOAD_INTERRUPT_REASON_NONE != reason) {
    InterruptAndDiscardPartialState(reason);
    UpdateObservers();
    return;
  }

  DCHECK(GetTargetFilePath() == full_path);

  if (full_path != GetFullPath()) {
    DCHECK(!full_path.empty());
    SetFullPath(full_path);
  }

  DCHECK(download_file_);
  ReleaseDownloadFile(false);

  TransitionTo(COMPLETING_INTERNAL);

  if (delegate_->ShouldOpenDownload(
          this, base::Bind(&DownloadItemImpl::DelayedDownloadOpened,
                           weak_ptr_factory_.GetWeakPtr()))) {
    Completed();
  } else {
    delegate_delayed_complete_ = true;
    UpdateObservers();
  }
}
