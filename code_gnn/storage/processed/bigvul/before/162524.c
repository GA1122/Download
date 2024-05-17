bool Resource::CanReuse(const FetchParameters& params) const {
  const ResourceRequest& new_request = params.GetResourceRequest();
  const ResourceLoaderOptions& new_options = params.Options();

  if (GetResponse().WasFetchedViaServiceWorker() &&
      GetResponse().ResponseTypeViaServiceWorker() ==
          network::mojom::FetchResponseType::kOpaque &&
      new_request.GetFetchRequestMode() !=
          network::mojom::FetchRequestMode::kNoCORS) {
    return false;
  }

  if (GetResourceRequest().AllowStoredCredentials() !=
      new_request.AllowStoredCredentials())
    return false;

  if (new_request.IsConditional() || response_.HttpStatusCode() == 304)
    return false;


  if (new_options.synchronous_policy == kRequestSynchronously ||
      options_.synchronous_policy == kRequestSynchronously)
    return false;

  if (resource_request_.GetKeepalive() || new_request.GetKeepalive()) {
    return false;
  }


  if (new_request.GetFetchCredentialsMode() !=
      resource_request_.GetFetchCredentialsMode())
    return false;

  const auto new_mode = new_request.GetFetchRequestMode();
  const auto existing_mode = resource_request_.GetFetchRequestMode();

  if (new_mode != existing_mode)
    return false;

  switch (new_mode) {
    case network::mojom::FetchRequestMode::kNoCORS:
    case network::mojom::FetchRequestMode::kNavigate:
      break;

    case network::mojom::FetchRequestMode::kCORS:
    case network::mojom::FetchRequestMode::kSameOrigin:
    case network::mojom::FetchRequestMode::kCORSWithForcedPreflight:
      if (options_.cors_handling_by_resource_fetcher !=
          new_options.cors_handling_by_resource_fetcher) {
        if (!options_.cors_handling_by_resource_fetcher)
          return false;

      }
      break;
  }

  return true;
}
