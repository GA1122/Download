void ResourceDispatcherHostImpl::OnDidLoadResourceFromMemoryCache(
    const GURL& url,
    const std::string& security_info,
    const std::string& http_method,
    const std::string& mime_type,
    ResourceType::Type resource_type) {
  if (!url.is_valid() || !(url.SchemeIs("http") || url.SchemeIs("https")))
    return;

  filter_->GetURLRequestContext(resource_type)->http_transaction_factory()->
      GetCache()->OnExternalCacheHit(url, http_method);
}
