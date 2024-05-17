void DataReductionProxyIOData::MarkProxiesAsBad(
    base::TimeDelta bypass_duration,
    const net::ProxyList& bad_proxies,
    mojom::DataReductionProxy::MarkProxiesAsBadCallback callback) {

  if (bypass_duration < base::TimeDelta()) {
    LOG(ERROR) << "Received bad MarkProxiesAsBad() -- invalid bypass_duration: "
               << bypass_duration;
    std::move(callback).Run();
    return;
  }

  if (bypass_duration > base::TimeDelta::FromDays(1))
    bypass_duration = base::TimeDelta::FromDays(1);

  for (const auto& proxy : bad_proxies.GetAll()) {
    if (!config_->FindConfiguredDataReductionProxy(proxy)) {
      LOG(ERROR) << "Received bad MarkProxiesAsBad() -- not a DRP server: "
                 << proxy.ToURI();
      std::move(callback).Run();
      return;
    }
  }

  proxy_config_client_->MarkProxiesAsBad(bypass_duration, bad_proxies,
                                         std::move(callback));
}
