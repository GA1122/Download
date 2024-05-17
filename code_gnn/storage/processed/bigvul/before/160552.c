void RenderFrameImpl::CommitNavigation(
    const network::ResourceResponseHead& head,
    const GURL& body_url,
    const CommonNavigationParams& common_params,
    const RequestNavigationParams& request_params,
    network::mojom::URLLoaderClientEndpointsPtr url_loader_client_endpoints,
    std::unique_ptr<URLLoaderFactoryBundleInfo> subresource_loader_factories,
    mojom::ControllerServiceWorkerInfoPtr controller_service_worker_info,
    const base::UnguessableToken& devtools_navigation_token) {
  DCHECK(!IsRendererDebugURL(common_params.url));
  if (!browser_side_navigation_pending_ &&
      !browser_side_navigation_pending_url_.is_empty() &&
      browser_side_navigation_pending_url_ == request_params.original_url &&
      request_params.nav_entry_id == 0) {
    browser_side_navigation_pending_url_ = GURL();
    return;
  }

  controller_service_worker_info_ = std::move(controller_service_worker_info);

  std::unique_ptr<blink::WebScopedUserGesture> gesture(
      common_params.has_user_gesture ? new blink::WebScopedUserGesture(frame_)
                                     : nullptr);

  browser_side_navigation_pending_ = false;
  browser_side_navigation_pending_url_ = GURL();

  pending_navigation_info_.reset(nullptr);

  base::TimeTicks renderer_navigation_start = base::TimeTicks::Now();

  bool is_reload =
      FrameMsg_Navigate_Type::IsReload(common_params.navigation_type);
  bool is_history_navigation = request_params.page_state.IsValid();
  auto cache_mode = blink::mojom::FetchCacheMode::kDefault;
  RenderFrameImpl::PrepareRenderViewForNavigation(common_params.url,
                                                  request_params);

  GetContentClient()->SetActiveURL(
      common_params.url, frame_->Top()->GetSecurityOrigin().ToString().Utf8());

  if (is_reload && current_history_item_.IsNull()) {
    is_reload = false;
    cache_mode = blink::mojom::FetchCacheMode::kValidateCache;
  }

  if (request_params.is_view_source)
    frame_->EnableViewSourceMode(true);

  pending_navigation_params_.reset(
      new NavigationParams(common_params, request_params));

  pending_navigation_params_->common_params.navigation_start =
      SanitizeNavigationTiming(common_params.navigation_start,
                               renderer_navigation_start);

  blink::WebFrameLoadType load_type =
      common_params.should_replace_current_entry
          ? blink::WebFrameLoadType::kReplaceCurrentItem
          : blink::WebFrameLoadType::kStandard;
  blink::WebHistoryLoadType history_load_type =
      blink::kWebHistoryDifferentDocumentLoad;
  bool should_load_request = false;
  WebHistoryItem item_for_history_navigation;
  bool is_same_document =
      FrameMsg_Navigate_Type::IsSameDocument(common_params.navigation_type);

  DCHECK(is_same_document ||
         common_params.url.SchemeIs(url::kJavaScriptScheme) ||
         !base::FeatureList::IsEnabled(network::features::kNetworkService) ||
         subresource_loader_factories);

  SetupLoaderFactoryBundle(std::move(subresource_loader_factories));

  bool has_history_navigation_in_frame = false;

  if (is_reload) {
    load_type = ReloadFrameLoadTypeFor(common_params.navigation_type);
    should_load_request = true;
  } else if (is_history_navigation) {
    DCHECK_NE(0, request_params.nav_entry_id);
    std::unique_ptr<HistoryEntry> entry =
        PageStateToHistoryEntry(request_params.page_state);
    if (entry) {
      item_for_history_navigation = entry->root();
      switch (common_params.navigation_type) {
        case FrameMsg_Navigate_Type::RELOAD:
        case FrameMsg_Navigate_Type::RELOAD_BYPASSING_CACHE:
        case FrameMsg_Navigate_Type::RELOAD_ORIGINAL_REQUEST_URL:
        case FrameMsg_Navigate_Type::RESTORE:
        case FrameMsg_Navigate_Type::RESTORE_WITH_POST:
        case FrameMsg_Navigate_Type::HISTORY_DIFFERENT_DOCUMENT:
          history_load_type = blink::kWebHistoryDifferentDocumentLoad;
          break;
        case FrameMsg_Navigate_Type::HISTORY_SAME_DOCUMENT:
          history_load_type = blink::kWebHistorySameDocumentLoad;
          break;
        default:
          NOTREACHED();
          history_load_type = blink::kWebHistoryDifferentDocumentLoad;
      }
      load_type = request_params.is_history_navigation_in_new_child
                      ? blink::WebFrameLoadType::kInitialHistoryLoad
                      : blink::WebFrameLoadType::kBackForward;
      should_load_request = true;

      history_subframe_unique_names_ = request_params.subframe_unique_names;

      if (history_load_type == blink::kWebHistorySameDocumentLoad) {
        if (current_history_item_.IsNull()) {
          history_load_type = blink::kWebHistoryDifferentDocumentLoad;
          NOTREACHED();
        } else {
          if (current_history_item_.DocumentSequenceNumber() !=
              item_for_history_navigation.DocumentSequenceNumber()) {
            history_load_type = blink::kWebHistoryDifferentDocumentLoad;
          }
        }
      }

      bool interrupted_by_client_redirect =
          frame_->IsNavigationScheduledWithin(0) ||
          frame_->GetProvisionalDocumentLoader() ||
          !current_history_item_.IsNull();
      if (request_params.is_history_navigation_in_new_child &&
          interrupted_by_client_redirect) {
        should_load_request = false;
        has_history_navigation_in_frame = false;
      }
    }
  } else {
    should_load_request = true;
  }

  if (should_load_request) {
    base::WeakPtr<RenderFrameImpl> weak_this = weak_factory_.GetWeakPtr();
    bool is_client_redirect =
        !!(common_params.transition & ui::PAGE_TRANSITION_CLIENT_REDIRECT);

    bool should_load_data_url = !common_params.base_url_for_data_url.is_empty();
#if defined(OS_ANDROID)
    should_load_data_url |= !request_params.data_url_as_string.empty();
#endif
    if (is_main_frame_ && should_load_data_url) {
      LoadDataURL(common_params, request_params, frame_, load_type,
                  item_for_history_navigation, history_load_type,
                  is_client_redirect);
    } else {
      WebURLRequest request = CreateURLRequestForCommit(
          common_params, request_params, std::move(url_loader_client_endpoints),
          head, body_url, is_same_document);

      frame_->Load(request, load_type, item_for_history_navigation,
                   history_load_type, is_client_redirect,
                   devtools_navigation_token);

      if (!weak_this)
        return;
    }
  } else {
    if (frame_ && !frame_->IsLoading() && !has_history_navigation_in_frame)
      Send(new FrameHostMsg_DidStopLoading(routing_id_));
  }

  pending_navigation_params_.reset();

  frame_->GetDocumentLoader()->ResetSourceLocation();
  if (frame_->GetProvisionalDocumentLoader())
    frame_->GetProvisionalDocumentLoader()->ResetSourceLocation();
}
