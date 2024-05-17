void Resource::SetCachePolicyBypassingCache() {
  resource_request_.SetCacheMode(mojom::FetchCacheMode::kBypassCache);
}
