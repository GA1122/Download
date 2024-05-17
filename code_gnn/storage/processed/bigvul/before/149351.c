void BinaryUploadService::MaybeUploadForDeepScanningCallback(
    std::unique_ptr<BinaryUploadService::Request> request,
    bool authorized) {
  if (!authorized)
    return;
  UploadForDeepScanning(std::move(request));
}
