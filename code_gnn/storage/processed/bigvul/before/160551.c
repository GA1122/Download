void RenderFrameImpl::CommitFailedNavigation(
    const CommonNavigationParams& common_params,
    const RequestNavigationParams& request_params,
    bool has_stale_copy_in_cache,
    int error_code,
    const base::Optional<std::string>& error_page_content,
    std::unique_ptr<URLLoaderFactoryBundleInfo> subresource_loader_factories) {
  bool is_reload =
      FrameMsg_Navigate_Type::IsReload(common_params.navigation_type);
  RenderFrameImpl::PrepareRenderViewForNavigation(common_params.url,
                                                  request_params);

  GetContentClient()->SetActiveURL(
      common_params.url, frame_->Top()->GetSecurityOrigin().ToString().Utf8());

  SetupLoaderFactoryBundle(std::move(subresource_loader_factories));

  pending_navigation_params_.reset(
      new NavigationParams(common_params, request_params));

  WebURLError error(
      error_code,
      has_stale_copy_in_cache ? WebURLError::HasCopyInCache::kTrue
                              : WebURLError::HasCopyInCache::kFalse,
      WebURLError::IsWebSecurityViolation::kFalse, common_params.url);
  WebURLRequest failed_request =
      CreateURLRequestForNavigation(common_params, request_params,
                                    std::unique_ptr<StreamOverrideParameters>(),
                                    frame_->IsViewSourceModeEnabled(),
                                    false);   

  if (!ShouldDisplayErrorPageForFailedLoad(error_code, common_params.url)) {
    Send(new FrameHostMsg_DidStopLoading(routing_id_));
    browser_side_navigation_pending_ = false;
    browser_side_navigation_pending_url_ = GURL();
    return;
  }

  WebLocalFrame::FallbackContentResult fallback_result =
      frame_->MaybeRenderFallbackContent(error);
  if (fallback_result != WebLocalFrame::NoFallbackContent) {
    if (fallback_result == WebLocalFrame::NoLoadInProgress) {
      Send(new FrameHostMsg_DidStopLoading(routing_id_));
    }
    browser_side_navigation_pending_ = false;
    browser_side_navigation_pending_url_ = GURL();
    return;
  }

  frame_->EnableViewSourceMode(false);

  bool replace = is_reload || common_params.url == GetLoadingUrl() ||
                 common_params.should_replace_current_entry;
  std::unique_ptr<HistoryEntry> history_entry;
  if (request_params.page_state.IsValid())
    history_entry = PageStateToHistoryEntry(request_params.page_state);

  base::WeakPtr<RenderFrameImpl> weak_this = weak_factory_.GetWeakPtr();

  bool had_provisional_document_loader = frame_->GetProvisionalDocumentLoader();
  if (request_params.nav_entry_id == 0) {
    blink::WebHistoryCommitType commit_type =
        replace ? blink::kWebHistoryInertCommit : blink::kWebStandardCommit;
    if (error_page_content.has_value()) {
      DidFailProvisionalLoadInternal(error, commit_type, error_page_content);
    } else {
      DidFailProvisionalLoad(error, commit_type);
    }
    if (!weak_this)
      return;
  }

  if (request_params.nav_entry_id != 0 || !had_provisional_document_loader) {
    LoadNavigationErrorPage(failed_request, error, replace, history_entry.get(),
                            error_page_content);
    if (!weak_this)
      return;
  }

  browser_side_navigation_pending_ = false;
  browser_side_navigation_pending_url_ = GURL();
}
