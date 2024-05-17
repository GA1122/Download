bool DownloadItemImpl::IsDownloadReadyForCompletion(
    const base::Closure& state_change_notification) {
  if (state_ != IN_PROGRESS_INTERNAL)
    return false;

  if (!AllDataSaved())
    return false;

  if (IsDangerous())
    return false;

  DCHECK(!GetTargetFilePath().empty());
  DCHECK(!GetFullPath().empty());
  DCHECK(GetTargetFilePath().DirName() == GetFullPath().DirName());

  if (!delegate_->ShouldCompleteDownload(this, state_change_notification))
    return false;

  return true;
}
