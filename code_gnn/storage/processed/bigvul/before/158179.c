  void FallbackToNonInterceptedRequest(bool reset_subresource_loader_params) {
    if (reset_subresource_loader_params)
      subresource_loader_params_.reset();

    if (!base::FeatureList::IsEnabled(network::features::kNetworkService)) {
      DCHECK(ResourceDispatcherHostImpl::Get());
      ResourceDispatcherHostImpl::Get()->CancelRequest(
          global_request_id_.child_id, global_request_id_.request_id);
    }

    DCHECK(url_loader_);
    uint32_t options = network::mojom::kURLLoadOptionNone;
    scoped_refptr<network::SharedURLLoaderFactory> factory =
        PrepareForNonInterceptedRequest(&options);
    url_loader_->RestartWithFactory(std::move(factory), options);
  }
