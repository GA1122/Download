void BinaryUploadService::FinishRequest(Request* request,
                                        Result result,
                                        DeepScanningClientResponse response) {
  RecordRequestMetrics(request, result, response);

  request->FinishRequest(result, response);
  active_requests_.erase(request);
  active_timers_.erase(request);
  active_uploads_.erase(request);
  received_malware_verdicts_.erase(request);
  received_dlp_verdicts_.erase(request);

  auto token_it = active_tokens_.find(request);
  if (token_it != active_tokens_.end()) {
    binary_fcm_service_->ClearCallbackForToken(token_it->second);
    active_tokens_.erase(token_it);
  }
}
