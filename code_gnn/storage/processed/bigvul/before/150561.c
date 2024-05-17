void DataReductionProxyConfig::HandleWarmupFetcherResponse(
    const net::ProxyServer& proxy_server,
    WarmupURLFetcher::FetchResult success_response) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(IsFetchInFlight());

  base::Optional<DataReductionProxyTypeInfo> proxy_type_info =
      FindConfiguredDataReductionProxy(proxy_server);

  if (!proxy_type_info && proxy_server.is_valid() &&
      !proxy_server.is_direct()) {
    return;
  }

  bool is_secure_proxy = false;
  bool is_core_proxy = false;

  if (proxy_type_info) {
    DCHECK(proxy_server.is_valid());
    DCHECK(!proxy_server.is_direct());
    is_secure_proxy = proxy_server.is_https() || proxy_server.is_quic();
    is_core_proxy = proxy_type_info->proxy_servers[proxy_type_info->proxy_index]
                        .IsCoreProxy();

    DCHECK(GetInFlightWarmupProxyDetails());
    DCHECK_EQ(is_secure_proxy, GetInFlightWarmupProxyDetails()->first);
    DCHECK_EQ(is_core_proxy, GetInFlightWarmupProxyDetails()->second);
  } else {
    DCHECK(!proxy_server.is_valid() || proxy_server.is_direct());
    is_secure_proxy = warmup_url_fetch_in_flight_secure_proxy_;
    is_core_proxy = warmup_url_fetch_in_flight_core_proxy_;
  }

  if (is_secure_proxy && is_core_proxy) {
    UMA_HISTOGRAM_BOOLEAN(
        "DataReductionProxy.WarmupURLFetcherCallback.SuccessfulFetch."
        "SecureProxy.Core",
        success_response == WarmupURLFetcher::FetchResult::kSuccessful);
  } else if (is_secure_proxy && !is_core_proxy) {
    UMA_HISTOGRAM_BOOLEAN(
        "DataReductionProxy.WarmupURLFetcherCallback.SuccessfulFetch."
        "SecureProxy.NonCore",
        success_response == WarmupURLFetcher::FetchResult::kSuccessful);
  } else if (!is_secure_proxy && is_core_proxy) {
    UMA_HISTOGRAM_BOOLEAN(
        "DataReductionProxy.WarmupURLFetcherCallback.SuccessfulFetch."
        "InsecureProxy.Core",
        success_response == WarmupURLFetcher::FetchResult::kSuccessful);
  } else {
    UMA_HISTOGRAM_BOOLEAN(
        "DataReductionProxy.WarmupURLFetcherCallback.SuccessfulFetch."
        "InsecureProxy.NonCore",
        success_response == WarmupURLFetcher::FetchResult::kSuccessful);
  }

  bool warmup_url_failed_past =
      network_properties_manager_->HasWarmupURLProbeFailed(is_secure_proxy,
                                                           is_core_proxy);

  network_properties_manager_->SetHasWarmupURLProbeFailed(
      is_secure_proxy, is_core_proxy,
      success_response !=
          WarmupURLFetcher::FetchResult::kSuccessful  );

  if (warmup_url_failed_past !=
      network_properties_manager_->HasWarmupURLProbeFailed(is_secure_proxy,
                                                           is_core_proxy)) {
    ReloadConfig();
  }

  io_task_runner_->PostTask(
      FROM_HERE, base::BindOnce(&DataReductionProxyConfig::FetchWarmupProbeURL,
                                weak_factory_.GetWeakPtr()));
}
