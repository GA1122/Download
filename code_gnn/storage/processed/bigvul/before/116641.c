void RenderViewImpl::OnNavigate(const ViewMsg_Navigate_Params& params) {
#if defined(OS_CHROMEOS)
  LOG(ERROR) << "OnNavigate: url=" << params.url
             << ", webview=" << webview()
             << ", reload=" << IsReload(params)
             << ", paerams.state.empty=" << params.state.empty();
#endif
  MaybeHandleDebugURL(params.url);
  if (!webview())
    return;

  FOR_EACH_OBSERVER(RenderViewObserver, observers_, Navigate(params.url));

  bool is_reload = IsReload(params);

  if (IsBackForwardToStaleEntry(params, is_reload))
    return;

  if (is_swapped_out_)
    SetSwappedOut(false);

  history_list_offset_ = params.current_history_list_offset;
  history_list_length_ = params.current_history_list_length;
  if (history_list_length_ >= 0)
    history_page_ids_.resize(history_list_length_, -1);
  if (params.pending_history_list_offset >= 0 &&
      params.pending_history_list_offset < history_list_length_)
    history_page_ids_[params.pending_history_list_offset] = params.page_id;

  content::GetContentClient()->SetActiveURL(params.url);

  WebFrame* main_frame = webview()->mainFrame();
  if (is_reload && main_frame->currentHistoryItem().isNull()) {
    is_reload = false;
  }

  pending_navigation_params_.reset(new ViewMsg_Navigate_Params);
  *pending_navigation_params_.get() = params;

  if (is_reload) {
    bool ignore_cache = (params.navigation_type ==
                             ViewMsg_Navigate_Type::RELOAD_IGNORING_CACHE);
    main_frame->reload(ignore_cache);
  } else if (!params.state.empty()) {
    DCHECK_NE(params.page_id, -1);
    main_frame->loadHistoryItem(
        webkit_glue::HistoryItemFromString(params.state));
  } else {
    WebURLRequest request(params.url);

    CHECK_EQ(params.page_id, -1);

    if (main_frame->isViewSourceModeEnabled())
      request.setCachePolicy(WebURLRequest::ReturnCacheDataElseLoad);

    if (params.referrer.url.is_valid()) {
      WebString referrer = WebSecurityPolicy::generateReferrerHeader(
          params.referrer.policy,
          params.url,
          WebString::fromUTF8(params.referrer.url.spec()));
      if (!referrer.isEmpty())
        request.setHTTPHeaderField(WebString::fromUTF8("Referer"), referrer);
    }

    if (!params.extra_headers.empty()) {
      for (net::HttpUtil::HeadersIterator i(params.extra_headers.begin(),
                                            params.extra_headers.end(), "\n");
           i.GetNext(); ) {
        request.addHTTPHeaderField(WebString::fromUTF8(i.name()),
                                   WebString::fromUTF8(i.values()));
      }
    }
#if defined(OS_CHROMEOS)
    LOG(ERROR) << " FrameLoader::loadRequest()";
#endif
    main_frame->loadRequest(request);
  }

  pending_navigation_params_.reset();
}
