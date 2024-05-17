DataReductionProxyConfig::GetInFlightWarmupProxyDetails() const {
  DCHECK(thread_checker_.CalledOnValidThread());

  if (!IsFetchInFlight())
    return base::nullopt;

  return std::make_pair(warmup_url_fetch_in_flight_secure_proxy_,
                        warmup_url_fetch_in_flight_core_proxy_);
}
