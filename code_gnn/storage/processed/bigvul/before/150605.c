void DataReductionProxyIOData::SetCustomProxyConfigClient(
    network::mojom::CustomProxyConfigClientPtrInfo config_client_info) {
  proxy_config_client_.Bind(std::move(config_client_info));
  UpdateCustomProxyConfig();
}
