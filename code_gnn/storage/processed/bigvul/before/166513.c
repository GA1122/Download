void ChromeContentBrowserClient::OnNetworkServiceCreated(
    network::mojom::NetworkService* network_service) {
  if (!base::FeatureList::IsEnabled(network::features::kNetworkService))
    return;
  if (!SystemNetworkContextManager::GetInstance()) {
    DCHECK(!g_browser_process);
    DCHECK(chrome_feature_list_creator_->local_state());
    SystemNetworkContextManager::CreateInstance(
        chrome_feature_list_creator_->local_state());
  }
  SystemNetworkContextManager::GetInstance()->OnNetworkServiceCreated(
      network_service);
}
