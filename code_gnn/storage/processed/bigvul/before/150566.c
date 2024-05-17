net::ProxyConfig DataReductionProxyConfig::ProxyConfigIgnoringHoldback() const {
  if (!enabled_by_user_ || config_values_->proxies_for_http().empty())
    return net::ProxyConfig::CreateDirect();
  return configurator_->CreateProxyConfig(false  ,
                                          *network_properties_manager_,
                                          config_values_->proxies_for_http());
}
