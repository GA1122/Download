void DownloadItemImpl::DetermineDownloadTarget() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DVLOG(20) << __func__ << "() " << DebugString(true);

  RecordDownloadCount(DETERMINE_DOWNLOAD_TARGET_COUNT);
  delegate_->DetermineDownloadTarget(
      this, base::Bind(&DownloadItemImpl::OnDownloadTargetDetermined,
                       weak_ptr_factory_.GetWeakPtr()));
}
