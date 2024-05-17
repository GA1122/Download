void NetworkHandler::WillSendNavigationRequest(net::HttpRequestHeaders* headers,
                                               bool* skip_service_worker,
                                               bool* disable_cache) {
  headers->SetHeader(kDevToolsEmulateNetworkConditionsClientId, host_id_);
  if (!user_agent_.empty())
    headers->SetHeader(net::HttpRequestHeaders::kUserAgent, user_agent_);
  for (auto& entry : extra_headers_)
    headers->SetHeader(entry.first, entry.second);
  *skip_service_worker |= bypass_service_worker_;
  *disable_cache |= cache_disabled_;
}
