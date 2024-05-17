    ServiceWorkerResponseCachedMetadataHandler(
        const Resource* resource,
        const SecurityOrigin* security_origin)
    : CachedMetadataHandlerImpl(resource), security_origin_(security_origin) {}
