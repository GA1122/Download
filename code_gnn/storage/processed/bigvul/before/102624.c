bool CloudPolicyCacheBase::SetPolicyInternal(
    const em::PolicyFetchResponse& policy,
    base::Time* timestamp,
    bool check_for_timestamp_validity) {
  DCHECK(CalledOnValidThread());
  bool initialization_was_not_complete = !initialization_complete_;
  is_unmanaged_ = false;
  PolicyMap mandatory_policy;
  PolicyMap recommended_policy;
  base::Time temp_timestamp;
  PublicKeyVersion temp_public_key_version;
  bool ok = DecodePolicyResponse(policy, &mandatory_policy, &recommended_policy,
                                 &temp_timestamp, &temp_public_key_version);
  if (!ok) {
    LOG(WARNING) << "Decoding policy data failed.";
    UMA_HISTOGRAM_ENUMERATION(kMetricPolicy, kMetricPolicyFetchInvalidPolicy,
                              kMetricPolicySize);
    return false;
  }
  if (timestamp) {
    *timestamp = temp_timestamp;
  }
  if (check_for_timestamp_validity &&
      temp_timestamp > base::Time::NowFromSystemTime()) {
    LOG(WARNING) << "Rejected policy data, file is from the future.";
    UMA_HISTOGRAM_ENUMERATION(kMetricPolicy,
                              kMetricPolicyFetchTimestampInFuture,
                              kMetricPolicySize);
    return false;
  }
  public_key_version_.version = temp_public_key_version.version;
  public_key_version_.valid = temp_public_key_version.valid;

  const bool new_policy_differs =
      !mandatory_policy_.Equals(mandatory_policy) ||
      !recommended_policy_.Equals(recommended_policy);
  mandatory_policy_.Swap(&mandatory_policy);
  recommended_policy_.Swap(&recommended_policy);
  initialization_complete_ = true;

  if (!new_policy_differs) {
    UMA_HISTOGRAM_ENUMERATION(kMetricPolicy, kMetricPolicyFetchNotModified,
                              kMetricPolicySize);
  }

  if (new_policy_differs || initialization_was_not_complete) {
    FOR_EACH_OBSERVER(Observer, observer_list_, OnCacheUpdate(this));
  }
  InformNotifier(CloudPolicySubsystem::SUCCESS,
                 CloudPolicySubsystem::NO_DETAILS);
  return true;
}
