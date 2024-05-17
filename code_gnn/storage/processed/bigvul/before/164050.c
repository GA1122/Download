void OnDownloadStarted(
    download::DownloadItemImpl* download,
    const download::DownloadUrlParameters::OnStartedCallback& on_started) {
  if (on_started.is_null())
    return;

  if (!download || download->GetState() == download::DownloadItem::CANCELLED)
    on_started.Run(nullptr, download::DOWNLOAD_INTERRUPT_REASON_USER_CANCELED);
  else
    on_started.Run(download, download::DOWNLOAD_INTERRUPT_REASON_NONE);
}
