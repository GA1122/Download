void DownloadController::DangerousDownloadValidated(
    WebContents* web_contents,
    const std::string& download_guid,
    bool accept) {
  if (!web_contents)
    return;
  DownloadManager* dlm =
      BrowserContext::GetDownloadManager(web_contents->GetBrowserContext());
  DownloadItem* item = dlm->GetDownloadByGuid(download_guid);
  if (!item)
    return;
  if (accept) {
    item->ValidateDangerousDownload();
  } else {
    DownloadController::RecordDownloadCancelReason(
        DownloadController::CANCEL_REASON_DANGEROUS_DOWNLOAD_INFOBAR_DISMISSED);
    item->Remove();
  }
}
