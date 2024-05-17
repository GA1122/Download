void DownloadItemImpl::OnDownloadTargetDetermined(
    const base::FilePath& target_path,
    TargetDisposition disposition,
    DownloadDangerType danger_type,
    const base::FilePath& intermediate_path,
    DownloadInterruptReason interrupt_reason) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (state_ == CANCELLED_INTERNAL)
    return;

  DCHECK(state_ == TARGET_PENDING_INTERNAL ||
         state_ == INTERRUPTED_TARGET_PENDING_INTERNAL);
  DVLOG(20) << __func__ << "() target_path:" << target_path.value()
            << " intermediate_path:" << intermediate_path.value()
            << " disposition:" << disposition << " danger_type:" << danger_type
            << " interrupt_reason:"
            << DownloadInterruptReasonToString(interrupt_reason)
            << " this:" << DebugString(true);

  RecordDownloadCount(DOWNLOAD_TARGET_DETERMINED_COUNT);

  if (IsCancellation(interrupt_reason) || target_path.empty()) {
    Cancel(true);
    return;
  }

  if (state_ == TARGET_PENDING_INTERNAL &&
      interrupt_reason != DOWNLOAD_INTERRUPT_REASON_NONE) {
    deferred_interrupt_reason_ = interrupt_reason;
    TransitionTo(INTERRUPTED_TARGET_PENDING_INTERNAL);
    OnTargetResolved();
    return;
  }

  destination_info_.target_path = target_path;
  destination_info_.target_disposition = disposition;
  SetDangerType(danger_type);

  if (state_ == INTERRUPTED_TARGET_PENDING_INTERNAL && !download_file_) {
    OnTargetResolved();
    return;
  }

  DCHECK(intermediate_path.DirName() == target_path.DirName());

  if (intermediate_path == GetFullPath()) {
    OnDownloadRenamedToIntermediateName(DOWNLOAD_INTERRUPT_REASON_NONE,
                                        intermediate_path);
    return;
  }

  DCHECK(!IsSavePackageDownload());
  DownloadFile::RenameCompletionCallback callback =
      base::Bind(&DownloadItemImpl::OnDownloadRenamedToIntermediateName,
                 weak_ptr_factory_.GetWeakPtr());
  GetDownloadTaskRunner()->PostTask(
      FROM_HERE,
      base::BindOnce(&DownloadFile::RenameAndUniquify,
                     base::Unretained(download_file_.get()), intermediate_path,
                     callback));
}
