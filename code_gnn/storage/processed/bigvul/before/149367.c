void BinaryUploadService::Request::set_request_dlp_scan(
    DlpDeepScanningClientRequest dlp_request) {
  *deep_scanning_request_.mutable_dlp_scan_request() = std::move(dlp_request);
}
