bool IsSameOriginClientProviderHost(const GURL& origin,
                                    bool allow_reserved_client,
                                    ServiceWorkerProviderHost* host) {
  return host->IsProviderForClient() && host->url().GetOrigin() == origin &&
         (allow_reserved_client || host->is_execution_ready());
}
