DataReductionProxyIOData::CreateCustomProxyConfig(
    bool is_warmup_url,
    const std::vector<DataReductionProxyServer>& proxies_for_http) const {
  auto config = network::mojom::CustomProxyConfig::New();
  if (params::IsIncludedInHoldbackFieldTrial()) {
    config->rules =
        configurator_
            ->CreateProxyConfig(is_warmup_url,
                                config_->GetNetworkPropertiesManager(),
                                std::vector<DataReductionProxyServer>())
            .proxy_rules();
  } else {
    config->rules =
        configurator_
            ->CreateProxyConfig(is_warmup_url,
                                config_->GetNetworkPropertiesManager(),
                                proxies_for_http)
            .proxy_rules();
  }

  net::EffectiveConnectionType type = GetEffectiveConnectionType();
  if (type > net::EFFECTIVE_CONNECTION_TYPE_OFFLINE) {
    DCHECK_NE(net::EFFECTIVE_CONNECTION_TYPE_LAST, type);
    config->pre_cache_headers.SetHeader(
        chrome_proxy_ect_header(),
        net::GetNameForEffectiveConnectionType(type));
  }

  request_options_->AddRequestHeader(&config->post_cache_headers,
                                     base::nullopt);

  config->assume_https_proxies_support_quic = true;
  config->can_use_proxy_on_http_url_redirect_cycles = false;

  return config;
}
