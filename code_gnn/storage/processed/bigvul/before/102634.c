 void CloudPolicyController::HandlePolicyResponse(
    const em::DevicePolicyResponse& response) {
  if (response.response_size() > 0) {
    if (response.response_size() > 1) {
      LOG(WARNING) << "More than one policy in the response of the device "
                   << "management server, discarding.";
    }
    if (response.response(0).error_code() !=
        DeviceManagementBackend::kErrorServicePolicyNotFound) {
      cache_->SetPolicy(response.response(0));
      SetState(STATE_POLICY_VALID);
    } else {
      UMA_HISTOGRAM_ENUMERATION(kMetricPolicy, kMetricPolicyFetchBadResponse,
                                kMetricPolicySize);
      SetState(STATE_POLICY_UNAVAILABLE);
    }
  } else {
    UMA_HISTOGRAM_ENUMERATION(kMetricPolicy, kMetricPolicyFetchBadResponse,
                              kMetricPolicySize);
  }
}
