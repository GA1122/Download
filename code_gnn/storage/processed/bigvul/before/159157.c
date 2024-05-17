bool IsCancellation(DownloadInterruptReason reason) {
  return reason == DOWNLOAD_INTERRUPT_REASON_USER_SHUTDOWN ||
         reason == DOWNLOAD_INTERRUPT_REASON_USER_CANCELED;
}
