void RenderFrameImpl::NavigateInternal(
    const CommonNavigationParams& common_params,
    const StartNavigationParams& start_params,
    const RequestNavigationParams& request_params,
    scoped_ptr<StreamOverrideParameters> stream_params) {
  bool browser_side_navigation =
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableBrowserSideNavigation);
  bool is_reload = IsReload(common_params.navigation_type);
  bool is_history_navigation = request_params.page_state.IsValid();
  WebURLRequest::CachePolicy cache_policy =
      WebURLRequest::UseProtocolCachePolicy;
  RenderFrameImpl::PrepareRenderViewForNavigation(
      common_params.url, request_params, &is_reload, &cache_policy);

  GetContentClient()->SetActiveURL(common_params.url);

  if (request_params.has_committed_real_load && frame_->parent())
    frame_->setCommittedFirstRealLoad();

  if (is_reload && !render_view_->history_controller()->GetCurrentEntry()) {
    is_reload = false;
    cache_policy = WebURLRequest::ReloadIgnoringCacheData;
  }

  pending_navigation_params_.reset(
      new NavigationParams(common_params, start_params, request_params));

  blink::WebFrameLoadType load_type = blink::WebFrameLoadType::Standard;
  bool should_load_request = false;
  WebHistoryItem item_for_history_navigation;
  WebURLRequest request = CreateURLRequestForNavigation(
      common_params, stream_params.Pass(), frame_->isViewSourceModeEnabled());
#if defined(OS_ANDROID)
  request.setHasUserGesture(start_params.has_user_gesture);
#endif

  if (browser_side_navigation)
    request.setCheckForBrowserSideNavigation(false);

  if (is_reload) {
    bool ignore_cache = (common_params.navigation_type ==
                         FrameMsg_Navigate_Type::RELOAD_IGNORING_CACHE);
    load_type = ignore_cache ? blink::WebFrameLoadType::ReloadFromOrigin
                             : blink::WebFrameLoadType::Reload;

    if (!browser_side_navigation) {
      const GURL override_url =
          (common_params.navigation_type ==
           FrameMsg_Navigate_Type::RELOAD_ORIGINAL_REQUEST_URL)
              ? common_params.url
              : GURL();
      request = frame_->requestForReload(load_type, override_url);
    }
    should_load_request = true;
  } else if (is_history_navigation) {
    DCHECK_NE(request_params.page_id, -1);
    DCHECK_NE(0, request_params.nav_entry_id);
    scoped_ptr<HistoryEntry> entry =
        PageStateToHistoryEntry(request_params.page_state);
    if (entry) {
      CHECK(entry->root().urlString() != WebString::fromUTF8(kSwappedOutURL));

      if (!browser_side_navigation) {
        scoped_ptr<NavigationParams> navigation_params(
            new NavigationParams(*pending_navigation_params_.get()));
        if (!SiteIsolationPolicy::UseSubframeNavigationEntries()) {
          DCHECK(!frame_->parent());
          render_view_->history_controller()->GoToEntry(
              frame_, entry.Pass(), navigation_params.Pass(), cache_policy);
        } else {
          SetPendingNavigationParams(navigation_params.Pass());
          blink::WebHistoryItem history_item = entry->root();
          blink::WebHistoryLoadType load_type =
              request_params.is_same_document_history_load
                  ? blink::WebHistorySameDocumentLoad
                  : blink::WebHistoryDifferentDocumentLoad;

          render_view_->history_controller()->set_provisional_entry(
              entry.Pass());
          WebURLRequest request =
              frame_->requestFromHistoryItem(history_item, cache_policy);
          frame_->load(request, blink::WebFrameLoadType::BackForward,
                       history_item, load_type);
        }
      } else {
        item_for_history_navigation =
            entry->GetHistoryNodeForFrame(this)->item();
        load_type = blink::WebFrameLoadType::BackForward;
        should_load_request = true;
      }
    }
  } else {
    if (!start_params.extra_headers.empty() && !browser_side_navigation) {
      for (net::HttpUtil::HeadersIterator i(start_params.extra_headers.begin(),
                                            start_params.extra_headers.end(),
                                            "\n");
           i.GetNext();) {
        request.addHTTPHeaderField(WebString::fromUTF8(i.name()),
                                   WebString::fromUTF8(i.values()));
      }
    }

    if (start_params.is_post && !browser_side_navigation) {
      request.setHTTPMethod(WebString::fromUTF8("POST"));

      WebHTTPBody http_body;
      http_body.initialize();
      const char* data = nullptr;
      if (start_params.browser_initiated_post_data.size()) {
        data = reinterpret_cast<const char*>(
            &start_params.browser_initiated_post_data.front());
      }
      http_body.appendData(
          WebData(data, start_params.browser_initiated_post_data.size()));
      request.setHTTPBody(http_body);
    }

    CHECK_EQ(request_params.page_id, -1);

    should_load_request = true;
  }

  if (should_load_request) {
    base::TimeTicks renderer_navigation_start = base::TimeTicks::Now();

    if (!common_params.base_url_for_data_url.is_empty() ||
        (browser_side_navigation &&
         common_params.url.SchemeIs(url::kDataScheme))) {
      LoadDataURL(common_params, frame_);
    } else {
      frame_->toWebLocalFrame()->load(request, load_type,
                                      item_for_history_navigation);
    }

    if (load_type == blink::WebFrameLoadType::Standard) {
      UpdateFrameNavigationTiming(frame_,
                                  request_params.browser_navigation_start,
                                  renderer_navigation_start);
    }
  }

  pending_navigation_params_.reset();
}
