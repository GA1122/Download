  static Resource::CachedMetadataHandlerImpl* Create(
      const Resource* resource,
      const SecurityOrigin* security_origin) {
    return new ServiceWorkerResponseCachedMetadataHandler(resource,
                                                          security_origin);
  }
