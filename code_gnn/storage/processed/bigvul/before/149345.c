void BinaryUploadService::Request::FinishRequest(
    Result result,
    DeepScanningClientResponse response) {
  std::move(callback_).Run(result, response);
}
