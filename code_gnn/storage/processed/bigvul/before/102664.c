 void DeviceTokenFetcher::HandleRegisterResponse(
    const em::DeviceRegisterResponse& response) {
  if (response.has_device_management_token()) {
    UMA_HISTOGRAM_ENUMERATION(kMetricToken, kMetricTokenFetchOK,
                              kMetricTokenSize);
    data_store_->SetDeviceToken(response.device_management_token(), false);
    SetState(STATE_TOKEN_AVAILABLE);
  } else {
    NOTREACHED();
    UMA_HISTOGRAM_ENUMERATION(kMetricToken, kMetricTokenFetchBadResponse,
                              kMetricTokenSize);
    SetState(STATE_ERROR);
  }
}
