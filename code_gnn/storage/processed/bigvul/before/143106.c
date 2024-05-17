ServiceWorkerNetworkProvider::ServiceWorkerNetworkProvider(
    int route_id,
    ServiceWorkerProviderType provider_type,
    bool is_parent_frame_secure)
    : ServiceWorkerNetworkProvider(route_id,
                                   provider_type,
                                   GetNextProviderId(),
                                   is_parent_frame_secure) {}
