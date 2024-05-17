RenderFrameHostImpl::PendingNavigation::PendingNavigation(
    const CommonNavigationParams& common_params,
    mojom::BeginNavigationParamsPtr begin_params,
    scoped_refptr<network::SharedURLLoaderFactory> blob_url_loader_factory)
    : common_params(common_params),
      begin_params(std::move(begin_params)),
      blob_url_loader_factory(std::move(blob_url_loader_factory)) {}
