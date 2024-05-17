void BindNetworkContextOnUI(network::mojom::CustomProxyConfigPtr config,
                            network::mojom::NetworkContextRequest request,
                            const std::string& user_agent) {
  auto params = network::mojom::NetworkContextParams::New();
  params->user_agent = user_agent;
  params->initial_custom_proxy_config = std::move(config);
  content::GetNetworkService()->CreateNetworkContext(std::move(request),
                                                     std::move(params));
}
