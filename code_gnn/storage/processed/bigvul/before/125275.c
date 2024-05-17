void RenderViewImpl::OnNavigate(const ViewMsg_Navigate_Params& params) {
  MaybeHandleDebugURL(params.url);
  if (!webview())
    return;

  FOR_EACH_OBSERVER(RenderViewObserver, observers_, Navigate(params.url));

  bool is_reload = IsReload(params);

  if (IsBackForwardToStaleEntry(params, is_reload))
    return;

  if (is_swapped_out_) {
    webview()->setVisibilityState(visibilityState(), false);

    is_reload = false;

    SetSwappedOut(false);
  }

  history_list_offset_ = params.current_history_list_offset;
  history_list_length_ = params.current_history_list_length;
  if (history_list_length_ >= 0)
    history_page_ids_.resize(history_list_length_, -1);
  if (params.pending_history_list_offset >= 0 &&
      params.pending_history_list_offset < history_list_length_)
    history_page_ids_[params.pending_history_list_offset] = params.page_id;

  GetContentClient()->SetActiveURL(params.url);

  WebFrame* main_frame = webview()->mainFrame();
  if (is_reload && main_frame->currentHistoryItem().isNull()) {
    is_reload = false;
  }

  pending_navigation_params_.reset(new ViewMsg_Navigate_Params(params));

  if (is_reload) {
    bool reload_original_url =
        (params.navigation_type ==
            ViewMsg_Navigate_Type::RELOAD_ORIGINAL_REQUEST_URL);
    bool ignore_cache = (params.navigation_type ==
                             ViewMsg_Navigate_Type::RELOAD_IGNORING_CACHE);

    if (reload_original_url)
      main_frame->reloadWithOverrideURL(params.url, true);
    else
      main_frame->reload(ignore_cache);
  } else if (!params.state.empty()) {
    DCHECK_NE(params.page_id, -1);
    WebHistoryItem item = webkit_glue::HistoryItemFromString(params.state);
    if (!item.isNull()) {
      CHECK(item.urlString() != WebString::fromUTF8(kSwappedOutURL));
      main_frame->loadHistoryItem(item);
    }
  } else if (!params.base_url_for_data_url.is_empty()) {
    std::string mime_type, charset, data;
    if (net::DataURL::Parse(params.url, &mime_type, &charset, &data)) {
      main_frame->loadData(
          WebData(data.c_str(), data.length()),
          WebString::fromUTF8(mime_type),
          WebString::fromUTF8(charset),
          params.base_url_for_data_url,
          params.history_url_for_data_url,
          false);
    } else {
      CHECK(false) <<
          "Invalid URL passed: " << params.url.possibly_invalid_spec();
    }
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

    if (params.is_post) {
      request.setHTTPMethod(WebString::fromUTF8("POST"));

      WebHTTPBody http_body;
      http_body.initialize();
      http_body.appendData(WebData(
          reinterpret_cast<const char*>(
              &params.browser_initiated_post_data.front()),
          params.browser_initiated_post_data.size()));
      request.setHTTPBody(http_body);
    }

    main_frame->loadRequest(request);
  }

  pending_navigation_params_.reset();
}
