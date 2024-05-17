bool IsSameOriginWindowProviderHost(const GURL& origin,
                                    ServiceWorkerProviderHost* host) {
  return host->provider_type() ==
             blink::mojom::ServiceWorkerProviderType::kForWindow &&
         host->url().GetOrigin() == origin && host->is_execution_ready();
}
