void RenderView::OnNavigate(const ViewMsg_Navigate_Params& params) {
  if (!webview())
    return;

  bool is_reload =
      params.navigation_type == ViewMsg_Navigate_Type::RELOAD ||
      params.navigation_type == ViewMsg_Navigate_Type::RELOAD_IGNORING_CACHE;

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

  if (!params.url.SchemeIs(chrome::kJavaScriptScheme)) {
    NavigationState* state = NavigationState::CreateBrowserInitiated(
        params.page_id,
        params.pending_history_list_offset,
        params.transition,
        params.request_time);
    if (params.navigation_type == ViewMsg_Navigate_Type::RESTORE) {
      state->set_cache_policy_override(WebURLRequest::UseProtocolCachePolicy);
    }
    pending_navigation_state_.reset(state);
  }

  NavigationState* navigation_state = pending_navigation_state_.get();

  if (navigation_state) {
    navigation_state->set_alt_error_page_fetcher(NULL);
  }

  if (is_reload) {
    if (navigation_state)
      navigation_state->set_load_type(NavigationState::RELOAD);
    bool ignore_cache = (params.navigation_type ==
                             ViewMsg_Navigate_Type::RELOAD_IGNORING_CACHE);
    main_frame->reload(ignore_cache);
  } else if (!params.state.empty()) {
    DCHECK_NE(params.page_id, -1);
    if (navigation_state)
      navigation_state->set_load_type(NavigationState::HISTORY_LOAD);
    main_frame->loadHistoryItem(
        webkit_glue::HistoryItemFromString(params.state));
  } else {
    WebURLRequest request(params.url);

    DCHECK_EQ(params.page_id, -1);

    if (main_frame->isViewSourceModeEnabled())
      request.setCachePolicy(WebURLRequest::ReturnCacheDataElseLoad);

    if (params.referrer.is_valid()) {
      if (!WebSecurityPolicy::shouldHideReferrer(
              params.url,
              WebString::fromUTF8(params.referrer.spec()))) {
        request.setHTTPHeaderField(WebString::fromUTF8("Referer"),
                                   WebString::fromUTF8(params.referrer.spec()));
      }
    }

    if (!params.extra_headers.empty()) {
      for (net::HttpUtil::HeadersIterator i(params.extra_headers.begin(),
                                            params.extra_headers.end(), "\n");
           i.GetNext(); ) {
        request.addHTTPHeaderField(WebString::fromUTF8(i.name()),
                                   WebString::fromUTF8(i.values()));
      }
    }

    if (navigation_state)
      navigation_state->set_load_type(NavigationState::NORMAL_LOAD);
    main_frame->loadRequest(request);
  }

  pending_navigation_state_.reset();
}
