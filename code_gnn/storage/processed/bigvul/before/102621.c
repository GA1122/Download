bool CloudPolicyCacheBase::DecodePolicyResponse(
    const em::PolicyFetchResponse& policy_response,
    PolicyMap* mandatory,
    PolicyMap* recommended,
    base::Time* timestamp,
    PublicKeyVersion* public_key_version) {
  std::string data = policy_response.policy_data();
  em::PolicyData policy_data;
  if (!policy_data.ParseFromString(data)) {
    LOG(WARNING) << "Failed to parse PolicyData protobuf.";
    return false;
  }
  if (timestamp) {
    *timestamp = base::Time::UnixEpoch() +
                 base::TimeDelta::FromMilliseconds(policy_data.timestamp());
  }
  if (public_key_version) {
    public_key_version->valid = policy_data.has_public_key_version();
    if (public_key_version->valid)
      public_key_version->version = policy_data.public_key_version();
  }

  return DecodePolicyData(policy_data, mandatory, recommended);
}
