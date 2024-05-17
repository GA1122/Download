void DownloadController::RecordDownloadCancelReason(
    DownloadCancelReason reason) {
  UMA_HISTOGRAM_ENUMERATION(
      "MobileDownload.CancelReason", reason, CANCEL_REASON_MAX);
}
