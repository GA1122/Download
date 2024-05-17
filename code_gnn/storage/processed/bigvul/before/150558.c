size_t DataReductionProxyConfig::GetWarmupURLFetchAttemptCounts() const {
  DCHECK(thread_checker_.CalledOnValidThread());
  return network_properties_manager_->GetWarmupURLFetchAttemptCounts(
      warmup_url_fetch_in_flight_secure_proxy_,
      warmup_url_fetch_in_flight_core_proxy_);
}
