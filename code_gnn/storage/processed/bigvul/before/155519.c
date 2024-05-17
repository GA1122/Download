void DataReductionProxySettings::SetCustomProxyConfigClient(
    network::mojom::CustomProxyConfigClientPtrInfo proxy_config_client) {
  DCHECK(!data_reduction_proxy_service_);
  proxy_config_client_ = std::move(proxy_config_client);
}
