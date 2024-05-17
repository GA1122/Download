void SecureProxyChecker::OnURLLoadCompleteOrRedirect(
    const std::string& response,
    int net_error,
    int response_code) {
  url_loader_.reset();

  base::TimeDelta secure_proxy_check_latency =
      base::Time::Now() - secure_proxy_check_start_time_;
  if (secure_proxy_check_latency >= base::TimeDelta()) {
    UMA_HISTOGRAM_MEDIUM_TIMES(kUMAProxySecureProxyCheckLatency,
                               secure_proxy_check_latency);
  }

  fetcher_callback_.Run(response, net_error, response_code);
}
