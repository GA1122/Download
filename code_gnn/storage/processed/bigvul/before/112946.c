void ChromeDownloadManagerDelegate::CheckDownloadUrlDone(
    int32 download_id,
    DownloadProtectionService::DownloadCheckResult result) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DownloadItem* download =
      download_manager_->GetActiveDownloadItem(download_id);
  if (!download)
    return;

  VLOG(2) << __FUNCTION__ << "() download = " << download->DebugString(false)
          << " verdict = " << result;
  content::DownloadDangerType danger_type = download->GetDangerType();
  if (result != DownloadProtectionService::SAFE)
    danger_type = content::DOWNLOAD_DANGER_TYPE_DANGEROUS_URL;

  download_history_->CheckVisitedReferrerBefore(
      download_id, download->GetReferrerUrl(),
      base::Bind(&ChromeDownloadManagerDelegate::CheckVisitedReferrerBeforeDone,
                 base::Unretained(this), download_id, danger_type));
}
