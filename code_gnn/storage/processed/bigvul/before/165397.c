StoragePartitionImpl::GetMediaURLRequestContext() {
  if (base::FeatureList::IsEnabled(network::features::kNetworkService))
    NOTREACHED();
  return media_url_request_context_.get();
}
