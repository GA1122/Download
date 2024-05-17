void DataReductionProxyConfigServiceClient::SetEnabled(bool enabled) {
  DCHECK(thread_checker_.CalledOnValidThread());
  enabled_ = enabled;
}
