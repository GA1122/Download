void NotifyCacheOnIO(
    scoped_refptr<net::URLRequestContextGetter> request_context,
    const GURL& url,
    const std::string& http_method) {
  net::HttpCache* cache = request_context->GetURLRequestContext()->
      http_transaction_factory()->GetCache();
  if (cache)
    cache->OnExternalCacheHit(url, http_method);
}
