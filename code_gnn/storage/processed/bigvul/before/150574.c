void DataReductionProxyConfigServiceClient::ApplySerializedConfig(
    const std::string& config_value) {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (RemoteConfigApplied())
    return;

  if (!client_config_override_.empty())
    return;

  std::string decoded_config;
  if (base::Base64Decode(config_value, &decoded_config)) {
    ClientConfig config;
    if (config.ParseFromString(decoded_config))
      ParseAndApplyProxyConfig(config);
  }
}
