void ResourceMessageFilter::OnSetCacheMode(bool enabled) {
  if (!CheckBenchmarkingEnabled())
    return;

  net::HttpCache::Mode mode = enabled ?
      net::HttpCache::NORMAL : net::HttpCache::DISABLE;
  request_context_->GetURLRequestContext()->
      http_transaction_factory()->GetCache()->set_mode(mode);
}
