 void DownloadItemImpl::OnDownloadRenamedToIntermediateName(
    const FilePath& full_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  if (!full_path.empty()) {
    SetFullPath(full_path);
    UpdateObservers();
  }
  delegate_->DownloadRenamedToIntermediateName(this);
}
