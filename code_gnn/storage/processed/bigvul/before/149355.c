void BinaryUploadService::OnTimeout(Request* request) {
  if (IsActive(request))
    FinishRequest(request, Result::TIMEOUT, DeepScanningClientResponse());
}
