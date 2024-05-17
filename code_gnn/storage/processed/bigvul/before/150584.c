bool DataReductionProxyConfigServiceClient::RemoteConfigApplied() const {
  DCHECK(thread_checker_.CalledOnValidThread());
  return remote_config_applied_;
}
