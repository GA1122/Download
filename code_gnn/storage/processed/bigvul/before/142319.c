void ChromePasswordManagerClient::PostHSTSQueryForHost(
    const GURL& origin,
    password_manager::HSTSCallback callback) const {
  password_manager::PostHSTSQueryForHostAndNetworkContext(
      origin,
      content::BrowserContext::GetDefaultStoragePartition(profile_)
          ->GetNetworkContext(),
      std::move(callback));
}
