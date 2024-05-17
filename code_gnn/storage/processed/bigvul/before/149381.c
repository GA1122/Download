bool CheckClientDownloadRequest::MaybeReturnAsynchronousVerdict(
    DownloadCheckResultReason reason) {
  if (ShouldUploadBinary(reason)) {
    callback_.Run(DownloadCheckResult::ASYNC_SCANNING);
    return true;
  }

  return false;
}
