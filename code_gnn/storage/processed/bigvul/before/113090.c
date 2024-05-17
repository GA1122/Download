void DownloadItemImpl::OnTargetPathSelected(const FilePath& target_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK_EQ(TARGET_DISPOSITION_PROMPT, target_disposition_);
  target_path_ = target_path;
}
