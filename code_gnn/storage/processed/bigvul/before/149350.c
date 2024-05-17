void BinaryUploadService::MaybeUploadForDeepScanning(
    std::unique_ptr<BinaryUploadService::Request> request) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  can_upload_data_ = true;
  if (!can_upload_data_.has_value()) {
    IsAuthorized(
        base::BindOnce(&BinaryUploadService::MaybeUploadForDeepScanningCallback,
                       weakptr_factory_.GetWeakPtr(), std::move(request)));
    return;
  }

  MaybeUploadForDeepScanningCallback(std::move(request),
                                     can_upload_data_.value());
}
