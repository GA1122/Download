void BinaryUploadService::UploadForDeepScanning(
    std::unique_ptr<BinaryUploadService::Request> request) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  Request* raw_request = request.get();
  active_requests_[raw_request] = std::move(request);
  start_times_[raw_request] = base::TimeTicks::Now();

  if (!binary_fcm_service_) {
    base::PostTask(FROM_HERE, {content::BrowserThread::UI},
                   base::BindOnce(&BinaryUploadService::FinishRequest,
                                  weakptr_factory_.GetWeakPtr(), raw_request,
                                  Result::FAILED_TO_GET_TOKEN,
                                  DeepScanningClientResponse()));
    return;
  }

  std::string token = base::RandBytesAsString(128);
  token = base::HexEncode(token.data(), token.size());
  active_tokens_[raw_request] = token;
  binary_fcm_service_->SetCallbackForToken(
      token, base::BindRepeating(&BinaryUploadService::OnGetResponse,
                                 weakptr_factory_.GetWeakPtr(), raw_request));
  raw_request->set_request_token(std::move(token));

  binary_fcm_service_->GetInstanceID(
      base::BindOnce(&BinaryUploadService::OnGetInstanceID,
                     weakptr_factory_.GetWeakPtr(), raw_request));
  active_timers_[raw_request] = std::make_unique<base::OneShotTimer>();
  active_timers_[raw_request]->Start(
      FROM_HERE, base::TimeDelta::FromSeconds(kScanningTimeoutSeconds),
      base::BindOnce(&BinaryUploadService::OnTimeout,
                     weakptr_factory_.GetWeakPtr(), raw_request));
}
