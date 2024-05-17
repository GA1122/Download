void BinaryUploadService::MaybeFinishRequest(Request* request) {
  bool requested_dlp_scan_response =
      request->deep_scanning_request().has_dlp_scan_request();
  auto received_dlp_response = received_dlp_verdicts_.find(request);
  if (requested_dlp_scan_response &&
      received_dlp_response == received_dlp_verdicts_.end()) {
    return;
  }

  bool requested_malware_scan_response =
      request->deep_scanning_request().has_malware_scan_request();
  auto received_malware_response = received_malware_verdicts_.find(request);
  if (requested_malware_scan_response &&
      received_malware_response == received_malware_verdicts_.end()) {
    return;
  }

  DeepScanningClientResponse response;
  if (requested_dlp_scan_response) {
    response.set_allocated_dlp_scan_verdict(
        received_dlp_response->second.release());
  }

  if (requested_malware_scan_response) {
    response.set_allocated_malware_scan_verdict(
        received_malware_response->second.release());
  }

  FinishRequest(request, Result::SUCCESS, std::move(response));
}
