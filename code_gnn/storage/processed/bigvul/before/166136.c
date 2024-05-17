void RenderFrameHostImpl::MarkInitiatorsAsRequiringSeparateURLLoaderFactory(
    base::flat_set<url::Origin> request_initiators,
    bool push_to_renderer_now) {
  size_t old_size = initiators_requiring_separate_url_loader_factory_.size();
  initiators_requiring_separate_url_loader_factory_.insert(
      request_initiators.begin(), request_initiators.end());
  size_t new_size = initiators_requiring_separate_url_loader_factory_.size();
  bool insertion_took_place = (old_size != new_size);

  if (push_to_renderer_now && insertion_took_place &&
      has_committed_any_navigation_) {
    std::unique_ptr<URLLoaderFactoryBundleInfo> subresource_loader_factories =
        std::make_unique<URLLoaderFactoryBundleInfo>();
    subresource_loader_factories->initiator_specific_factory_infos() =
        CreateInitiatorSpecificURLLoaderFactories(request_initiators);
    GetNavigationControl()->UpdateSubresourceLoaderFactories(
        std::move(subresource_loader_factories));
  }
}
