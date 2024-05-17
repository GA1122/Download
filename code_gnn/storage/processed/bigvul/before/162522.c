Resource::CachedMetadataHandlerImpl::CachedMetadataHandlerImpl(
    const Resource* resource)
    : response_url_(resource->GetResponse().Url()),
      response_time_(resource->GetResponse().ResponseTime()),
      cache_storage_cache_name_(
          resource->GetResponse().CacheStorageCacheName()),
      encoding_(resource->Encoding()) {}
