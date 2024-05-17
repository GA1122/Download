void RenderFrameImpl::CommitNavigation(
    const network::ResourceResponseHead& head,
    const CommonNavigationParams& common_params,
    const RequestNavigationParams& request_params,
    network::mojom::URLLoaderClientEndpointsPtr url_loader_client_endpoints,
    std::unique_ptr<URLLoaderFactoryBundleInfo> subresource_loader_factories,
    base::Optional<std::vector<mojom::TransferrableURLLoaderPtr>>
        subresource_overrides,
    blink::mojom::ControllerServiceWorkerInfoPtr controller_service_worker_info,
    network::mojom::URLLoaderFactoryPtr prefetch_loader_factory,
    const base::UnguessableToken& devtools_navigation_token,
    CommitNavigationCallback callback) {
  DCHECK(!IsRendererDebugURL(common_params.url));
  DCHECK(
      !FrameMsg_Navigate_Type::IsSameDocument(common_params.navigation_type));
  if (!browser_side_navigation_pending_ &&
      !browser_side_navigation_pending_url_.is_empty() &&
      browser_side_navigation_pending_url_ == request_params.original_url &&
      request_params.nav_entry_id == 0) {
    browser_side_navigation_pending_url_ = GURL();
    std::move(callback).Run(blink::mojom::CommitResult::Aborted);
    return;
  }

  DCHECK(common_params.url.SchemeIs(url::kJavaScriptScheme) ||
         !base::FeatureList::IsEnabled(network::features::kNetworkService) ||
         subresource_loader_factories);

  SetupLoaderFactoryBundle(std::move(subresource_loader_factories),
                           std::move(subresource_overrides),
                           std::move(prefetch_loader_factory));

  if (request_params.is_view_source)
    frame_->EnableViewSourceMode(true);

  PrepareFrameForCommit(common_params.url, request_params);

  const network::ResourceResponseHead* response_head = nullptr;
  if (!frame_->Parent() && !frame_->IsViewSourceModeEnabled())
    response_head = &head;
  std::unique_ptr<DocumentState> document_state(BuildDocumentStateFromParams(
      common_params, request_params, base::TimeTicks::Now(),
      std::move(callback), response_head));

  blink::WebFrameLoadType load_type = NavigationTypeToLoadType(
      common_params.navigation_type, common_params.should_replace_current_entry,
      request_params.page_state.IsValid());

  WebHistoryItem item_for_history_navigation;
  blink::mojom::CommitResult commit_status = blink::mojom::CommitResult::Ok;

  if (load_type == WebFrameLoadType::kBackForward) {
    DCHECK_NE(0, request_params.nav_entry_id);

    commit_status = PrepareForHistoryNavigationCommit(
        common_params.navigation_type, request_params,
        &item_for_history_navigation, &load_type);
  }

  if (commit_status != blink::mojom::CommitResult::Ok) {
    if (frame_ && !frame_->IsLoading())
      Send(new FrameHostMsg_DidStopLoading(routing_id_));
    return;
  }

  base::WeakPtr<RenderFrameImpl> weak_this = weak_factory_.GetWeakPtr();
  bool is_client_redirect =
      !!(common_params.transition & ui::PAGE_TRANSITION_CLIENT_REDIRECT);

  auto navigation_params =
      std::make_unique<WebNavigationParams>(devtools_navigation_token);
  navigation_params->frame_load_type = load_type;
  navigation_params->history_item = item_for_history_navigation;
  navigation_params->is_client_redirect = is_client_redirect;
  navigation_params->service_worker_network_provider =
      BuildServiceWorkerNetworkProviderForNavigation(
          &request_params, std::move(controller_service_worker_info));
  FillNavigationParams(common_params, request_params, navigation_params.get());

  bool should_load_data_url = !common_params.base_url_for_data_url.is_empty();
#if defined(OS_ANDROID)
  should_load_data_url |= !request_params.data_url_as_string.empty();
#endif
  if (is_main_frame_ && should_load_data_url) {
    std::string mime_type, charset, data;
    GURL base_url;
    DecodeDataURL(common_params, request_params, &mime_type, &charset, &data,
                  &base_url);
    navigation_params->request = WebURLRequest(base_url);
    navigation_params->data = WebData(data.c_str(), data.length());
    navigation_params->mime_type = WebString::FromUTF8(mime_type);
    navigation_params->text_encoding = WebString::FromUTF8(charset);
    navigation_params->unreachable_url = common_params.history_url_for_data_url;
  } else {
    navigation_params->request =
        CreateURLRequestForCommit(common_params, request_params,
                                  std::move(url_loader_client_endpoints), head);
  }

  committing_main_request_ = true;
  frame_->CommitNavigation(std::move(navigation_params),
                           std::move(document_state));
  if (!weak_this)
    return;
  committing_main_request_ = false;
}
