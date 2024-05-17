bool IsInterruptedDownloadAutoResumable(content::DownloadItem* download_item) {
  int interrupt_reason = download_item->GetLastReason();
  DCHECK_NE(interrupt_reason, content::DOWNLOAD_INTERRUPT_REASON_NONE);
  return
      interrupt_reason == content::DOWNLOAD_INTERRUPT_REASON_NETWORK_TIMEOUT ||
      interrupt_reason == content::DOWNLOAD_INTERRUPT_REASON_NETWORK_FAILED ||
      interrupt_reason ==
          content::DOWNLOAD_INTERRUPT_REASON_NETWORK_DISCONNECTED;
}
