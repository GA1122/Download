  static Resource::CachedMetadataHandlerImpl* Create(const Resource* resource) {
    return new CachedMetadataHandlerImpl(resource);
  }
