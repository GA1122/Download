void DataReductionProxyConfigServiceClient::RetrieveConfig() {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (!enabled_)
    return;

  if (!client_config_override_.empty()) {
    if (client_config_override_used_) {
      return;
    }
    client_config_override_used_ = true;

    std::string override_config;
    bool b64_decode_ok =
        base::Base64Decode(client_config_override_, &override_config);
    LOG_IF(DFATAL, !b64_decode_ok)
        << "The given ClientConfig is not valid base64";

    ClientConfig config;
    bool was_valid_config = config.ParseFromString(override_config);
    LOG_IF(DFATAL, !was_valid_config) << "The given ClientConfig was invalid.";
    if (was_valid_config)
      ParseAndApplyProxyConfig(config);
    return;
  }

  config_fetch_start_time_ = base::TimeTicks::Now();

  RetrieveRemoteConfig();
}
