void RenderFrameImpl::CommitFailedNavigationInternal(
    const CommonNavigationParams& common_params,
    const CommitNavigationParams& commit_params,
    bool has_stale_copy_in_cache,
    int error_code,
    const base::Optional<std::string>& error_page_content,
    std::unique_ptr<blink::URLLoaderFactoryBundleInfo>
        subresource_loader_factories,
    mojom::FrameNavigationControl::CommitFailedNavigationCallback callback,
    mojom::NavigationClient::CommitFailedNavigationCallback
        per_navigation_mojo_interface_callback) {
  TRACE_EVENT1("navigation,benchmark,rail",
               "RenderFrameImpl::CommitFailedNavigation", "id", routing_id_);
  DCHECK(
      !FrameMsg_Navigate_Type::IsSameDocument(common_params.navigation_type));
  RenderFrameImpl::PrepareRenderViewForNavigation(common_params.url,
                                                  commit_params);
  sync_navigation_callback_.Cancel();
  mhtml_body_loader_client_.reset();

  GetContentClient()->SetActiveURL(
      common_params.url, frame_->Top()->GetSecurityOrigin().ToString().Utf8());

  SetupLoaderFactoryBundle(std::move(subresource_loader_factories),
                           base::nullopt  ,
                           nullptr  );

  WebURLError error(
      error_code, 0,
      has_stale_copy_in_cache ? WebURLError::HasCopyInCache::kTrue
                              : WebURLError::HasCopyInCache::kFalse,
      WebURLError::IsWebSecurityViolation::kFalse, common_params.url);

  auto navigation_params = std::make_unique<WebNavigationParams>();
  FillNavigationParamsRequest(common_params, commit_params,
                              navigation_params.get());
  navigation_params->url = GURL(kUnreachableWebDataURL);
  navigation_params->error_code = error_code;

  if (!ShouldDisplayErrorPageForFailedLoad(error_code, common_params.url)) {
    AbortCommitNavigation(std::move(callback),
                          blink::mojom::CommitResult::Aborted);
    Send(new FrameHostMsg_DidStopLoading(routing_id_));
    browser_side_navigation_pending_ = false;
    browser_side_navigation_pending_url_ = GURL();
    return;
  }

  blink::WebNavigationControl::FallbackContentResult fallback_result =
      frame_->MaybeRenderFallbackContent(error);
  if (fallback_result != blink::WebNavigationControl::NoFallbackContent) {
    if (fallback_result == blink::WebNavigationControl::NoLoadInProgress) {
      AbortCommitNavigation(std::move(callback),
                            blink::mojom::CommitResult::Aborted);
      Send(new FrameHostMsg_DidStopLoading(routing_id_));
    } else {
      AbortCommitNavigation(std::move(callback),
                            blink::mojom::CommitResult::Ok);
    }
    browser_side_navigation_pending_ = false;
    browser_side_navigation_pending_url_ = GURL();
    return;
  }

  bool is_reload_or_history =
      FrameMsg_Navigate_Type::IsReload(common_params.navigation_type) ||
      FrameMsg_Navigate_Type::IsHistory(common_params.navigation_type);
  bool replace = is_reload_or_history || common_params.url == GetLoadingUrl() ||
                 common_params.should_replace_current_entry;
  std::unique_ptr<HistoryEntry> history_entry;
  if (commit_params.page_state.IsValid())
    history_entry = PageStateToHistoryEntry(commit_params.page_state);

  if (commit_params.nav_entry_id == 0) {
    NotifyObserversOfFailedProvisionalLoad(error);

    if (frame_->GetProvisionalDocumentLoader()) {
      SendFailedProvisionalLoad(navigation_params->http_method.Ascii(), error,
                                frame_);
    }
  }

  std::string error_html;
  if (error_page_content.has_value()) {
    error_html = error_page_content.value();
    GetContentClient()->renderer()->PrepareErrorPage(
        this, error, navigation_params->http_method.Ascii(),
        false  , nullptr);
  } else {
    GetContentClient()->renderer()->PrepareErrorPage(
        this, error, navigation_params->http_method.Ascii(),
        false  , &error_html);
  }

  frame_->EnableViewSourceMode(false);

  if (history_entry) {
    navigation_params->frame_load_type = WebFrameLoadType::kBackForward;
    navigation_params->history_item = history_entry->root();
  } else if (replace) {
    navigation_params->frame_load_type = WebFrameLoadType::kReplaceCurrentItem;
  }
  navigation_params->service_worker_network_provider =
      ServiceWorkerNetworkProviderForFrame::CreateInvalidInstance();
  FillMiscNavigationParams(common_params, commit_params,
                           navigation_params.get());
  WebNavigationParams::FillStaticResponse(navigation_params.get(), "text/html",
                                          "UTF-8", error_html);
  navigation_params->unreachable_url = error.url();

  std::unique_ptr<DocumentState> document_state = BuildDocumentStateFromParams(
      common_params, commit_params, base::TimeTicks(), std::move(callback),
      std::move(per_navigation_mojo_interface_callback), nullptr,
      std::move(navigation_client_impl_), ResourceDispatcher::MakeRequestID(),
      false  );

  base::WeakPtr<RenderFrameImpl> weak_this = weak_factory_.GetWeakPtr();
  frame_->CommitNavigation(std::move(navigation_params),
                           std::move(document_state));
  if (!weak_this)
    return;

  browser_side_navigation_pending_ = false;
  browser_side_navigation_pending_url_ = GURL();
}
