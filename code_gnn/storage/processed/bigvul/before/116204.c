void SSLManager::DidLoadFromMemoryCache(LoadFromMemoryCacheDetails* details) {
  scoped_refptr<SSLRequestInfo> info(new SSLRequestInfo(
      details->url(),
      ResourceType::SUB_RESOURCE,
      details->pid(),
      details->ssl_cert_id(),
      details->ssl_cert_status()));

  policy()->OnRequestStarted(info.get());
}
