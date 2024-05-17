void DownloadItemImpl::OnTargetPathDetermined(
    const FilePath& target_path,
    TargetDisposition disposition,
    content::DownloadDangerType danger_type) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  target_path_ = target_path;
  target_disposition_ = disposition;
  SetDangerType(danger_type);
}
