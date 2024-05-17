void BinaryUploadService::Request::set_request_malware_scan(
    MalwareDeepScanningClientRequest malware_request) {
  *deep_scanning_request_.mutable_malware_scan_request() =
      std::move(malware_request);
}
