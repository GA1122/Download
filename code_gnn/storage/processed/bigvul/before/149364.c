void BinaryUploadService::ValidateDataUploadRequestCallback(
    BinaryUploadService::Result result,
    DeepScanningClientResponse response) {
  pending_validate_data_upload_request_ = false;
  can_upload_data_ = result == BinaryUploadService::Result::SUCCESS;
  RunAuthorizationCallbacks();
}
