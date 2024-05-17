net::URLRequestContextGetter* StoragePartitionImpl::GetURLRequestContext() {
#if defined(OS_WIN) || defined(OS_MACOSX) || defined(OS_LINUX)
  if (base::FeatureList::IsEnabled(network::features::kNetworkService))
    NOTREACHED();
#endif
  return url_request_context_.get();
}
