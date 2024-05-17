base::Optional<base::TimeDelta> DataReductionProxyConfig::GetHttpRttEstimate()
    const {
  DCHECK(thread_checker_.CalledOnValidThread());
  return http_rtt_;
}
