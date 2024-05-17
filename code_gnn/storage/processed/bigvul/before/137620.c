void DownloadController::OnDangerousDownload(DownloadItem* item) {
  WebContents* web_contents = item->GetWebContents();
  if (!web_contents)
    return;
  ChromeDownloadDelegate::FromWebContents(web_contents)->OnDangerousDownload(
      item->GetTargetFilePath().BaseName().value(), item->GetGuid());
}
