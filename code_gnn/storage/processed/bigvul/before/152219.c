void RenderFrameImpl::CommitNavigationWithParams(
    const CommonNavigationParams& common_params,
    const CommitNavigationParams& commit_params,
    std::unique_ptr<blink::URLLoaderFactoryBundleInfo>
        subresource_loader_factories,
    base::Optional<std::vector<mojom::TransferrableURLLoaderPtr>>
        subresource_overrides,
    blink::mojom::ControllerServiceWorkerInfoPtr controller_service_worker_info,
    blink::mojom::ServiceWorkerProviderInfoForWindowPtr provider_info,
    network::mojom::URLLoaderFactoryPtr prefetch_loader_factory,
    std::unique_ptr<DocumentState> document_state,
    std::unique_ptr<WebNavigationParams> navigation_params) {
  if (ShouldIgnoreCommitNavigation(commit_params)) {
    browser_side_navigation_pending_url_ = GURL();
    return;
  }

  if (common_params.url.SchemeIs(content::kChromeUIScheme)) {
    blink::WebRuntimeFeatures::EnableShadowDOMV0(true);
    blink::WebRuntimeFeatures::EnableCustomElementsV0(true);
    blink::WebRuntimeFeatures::EnableHTMLImports(true);
  }

  SetupLoaderFactoryBundle(std::move(subresource_loader_factories),
                           std::move(subresource_overrides),
                           std::move(prefetch_loader_factory));

  if (commit_params.is_view_source)
    frame_->EnableViewSourceMode(true);

  PrepareFrameForCommit(common_params.url, commit_params);

  blink::WebFrameLoadType load_type = NavigationTypeToLoadType(
      common_params.navigation_type, common_params.should_replace_current_entry,
      commit_params.page_state.IsValid());

  WebHistoryItem item_for_history_navigation;
  blink::mojom::CommitResult commit_status = blink::mojom::CommitResult::Ok;

  if (load_type == WebFrameLoadType::kBackForward) {
    DCHECK_NE(0, commit_params.nav_entry_id);

    commit_status = PrepareForHistoryNavigationCommit(
        common_params.navigation_type, commit_params,
        &item_for_history_navigation, &load_type);
  }

  if (commit_status != blink::mojom::CommitResult::Ok) {
    if (frame_ && !frame_->IsLoading())
      Send(new FrameHostMsg_DidStopLoading(routing_id_));
    return;
  }

  navigation_params->frame_load_type = load_type;
  navigation_params->history_item = item_for_history_navigation;
  navigation_params->service_worker_network_provider =
      BuildServiceWorkerNetworkProviderForNavigation(
          std::move(controller_service_worker_info), std::move(provider_info));

  frame_->CommitNavigation(std::move(navigation_params),
                           std::move(document_state));
}
