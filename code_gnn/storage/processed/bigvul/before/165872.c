void RenderFrameImpl::CommitFailedNavigation(
    const CommonNavigationParams& common_params,
    const RequestNavigationParams& request_params,
    bool has_stale_copy_in_cache,
    int error_code,
    const base::Optional<std::string>& error_page_content,
    std::unique_ptr<URLLoaderFactoryBundleInfo> subresource_loader_factories,
    CommitFailedNavigationCallback callback) {
  TRACE_EVENT1("navigation,benchmark,rail",
               "RenderFrameImpl::CommitFailedNavigation", "id", routing_id_);
  DCHECK(
      !FrameMsg_Navigate_Type::IsSameDocument(common_params.navigation_type));
  RenderFrameImpl::PrepareRenderViewForNavigation(common_params.url,
                                                  request_params);
  sync_navigation_callback_.Cancel();

  if (frame_->Parent() && error_code == net::ERR_CERT_SYMANTEC_LEGACY) {
    ReportLegacySymantecCert(common_params.url, true  );
  }

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
  WebURLRequest failed_request = CreateURLRequestForNavigation(
      common_params, request_params,
       nullptr, frame_->IsViewSourceModeEnabled());

  if (!ShouldDisplayErrorPageForFailedLoad(error_code, common_params.url)) {
    std::move(callback).Run(blink::mojom::CommitResult::Aborted);
    Send(new FrameHostMsg_DidStopLoading(routing_id_));
    browser_side_navigation_pending_ = false;
    browser_side_navigation_pending_url_ = GURL();
    return;
  }

  blink::WebNavigationControl::FallbackContentResult fallback_result =
      frame_->MaybeRenderFallbackContent(error);
  if (fallback_result != blink::WebNavigationControl::NoFallbackContent) {
    if (fallback_result == blink::WebNavigationControl::NoLoadInProgress) {
      std::move(callback).Run(blink::mojom::CommitResult::Aborted);
      Send(new FrameHostMsg_DidStopLoading(routing_id_));
    } else {
      std::move(callback).Run(blink::mojom::CommitResult::Ok);
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
  if (request_params.page_state.IsValid())
    history_entry = PageStateToHistoryEntry(request_params.page_state);

  if (request_params.nav_entry_id == 0) {
    NotifyObserversOfFailedProvisionalLoad(error);

    if (frame_->GetProvisionalDocumentLoader()) {
      SendFailedProvisionalLoad(failed_request, error, frame_);
    }
  }

  std::string error_html;
  if (error_page_content.has_value()) {
    error_html = error_page_content.value();
    GetContentClient()->renderer()->PrepareErrorPage(this, failed_request,
                                                     error, nullptr);
  } else {
    GetContentClient()->renderer()->PrepareErrorPage(this, failed_request,
                                                     error, &error_html);
  }

  frame_->EnableViewSourceMode(false);

  auto navigation_params = std::make_unique<WebNavigationParams>();
  if (history_entry) {
    navigation_params->frame_load_type = WebFrameLoadType::kBackForward;
    navigation_params->history_item = history_entry->root();
  } else if (replace) {
    navigation_params->frame_load_type = WebFrameLoadType::kReplaceCurrentItem;
  }
  navigation_params->service_worker_network_provider =
      BuildServiceWorkerNetworkProviderForNavigation(&request_params, nullptr);
  FillNavigationParams(common_params, request_params, navigation_params.get());

  failed_request.SetURL(GURL(kUnreachableWebDataURL));
  failed_request.SetCacheMode(blink::mojom::FetchCacheMode::kNoStore);
  navigation_params->request = failed_request;

  navigation_params->data = WebData(error_html.data(), error_html.length());
  navigation_params->mime_type = "text/html";
  navigation_params->text_encoding = "UTF-8";
  navigation_params->unreachable_url = error.url();

  std::unique_ptr<DocumentState> document_state = BuildDocumentStateFromParams(
      common_params, request_params, base::TimeTicks(), std::move(callback),
      nullptr);

  base::WeakPtr<RenderFrameImpl> weak_this = weak_factory_.GetWeakPtr();
  frame_->CommitNavigation(std::move(navigation_params),
                           std::move(document_state));
  if (!weak_this)
    return;

  browser_side_navigation_pending_ = false;
  browser_side_navigation_pending_url_ = GURL();
}
