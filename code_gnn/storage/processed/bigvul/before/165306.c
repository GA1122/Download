void ChromeContentBrowserClient::OnNetworkServiceCreated(
    network::mojom::NetworkService* network_service) {
  if (!base::FeatureList::IsEnabled(network::features::kNetworkService))
    return;

  PrefService* local_state;
  if (g_browser_process) {
    DCHECK(g_browser_process->local_state());
    local_state = g_browser_process->local_state();
  } else {
    DCHECK(chrome_feature_list_creator_->local_state());
    local_state = chrome_feature_list_creator_->local_state();
  }

  if (!data_use_measurement::ChromeDataUseMeasurement::GetInstance())
    data_use_measurement::ChromeDataUseMeasurement::CreateInstance(local_state);

  if (!SystemNetworkContextManager::GetInstance()) {
    DCHECK(!g_browser_process);
    SystemNetworkContextManager::CreateInstance(local_state);
  }
  SystemNetworkContextManager::GetInstance()->OnNetworkServiceCreated(
      network_service);
}
