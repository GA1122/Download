void DownloadItemImpl::OnContentCheckCompleted(
    content::DownloadDangerType danger_type) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(AllDataSaved());
  SetDangerType(danger_type);
}
