void RenderView::UpdateURL(WebFrame* frame) {
  WebDataSource* ds = frame->dataSource();
  DCHECK(ds);

  const WebURLRequest& request = ds->request();
  const WebURLRequest& original_request = ds->originalRequest();
  const WebURLResponse& response = ds->response();

  NavigationState* navigation_state = NavigationState::FromDataSource(ds);
  DCHECK(navigation_state);

  ViewHostMsg_FrameNavigate_Params params;
  params.http_status_code = response.httpStatusCode();
  params.is_post = false;
  params.page_id = page_id_;
  params.frame_id = frame->identifier();
  params.socket_address.set_host(response.remoteIPAddress().utf8());
  params.socket_address.set_port(response.remotePort());
  params.was_fetched_via_proxy = response.wasFetchedViaProxy();
  params.was_within_same_page = navigation_state->was_within_same_page();
  if (!navigation_state->security_info().empty()) {
    DCHECK(response.securityInfo().isEmpty());
    params.security_info = navigation_state->security_info();
  } else {
    params.security_info = response.securityInfo();
  }

  if (ds->hasUnreachableURL()) {
    params.url = ds->unreachableURL();
  } else {
    params.url = request.url();
  }

  GetRedirectChain(ds, &params.redirects);
  params.should_update_history = !ds->hasUnreachableURL() &&
      !response.isMultipartPayload() && (response.httpStatusCode() != 404);

  params.searchable_form_url = navigation_state->searchable_form_url();
  params.searchable_form_encoding =
      navigation_state->searchable_form_encoding();

  const PasswordForm* password_form_data =
      navigation_state->password_form_data();
  if (password_form_data)
    params.password_form = *password_form_data;

  params.gesture = navigation_gesture_;
  navigation_gesture_ = NavigationGestureUnknown;

  const WebHistoryItem& item = frame->currentHistoryItem();
  if (!item.isNull()) {
    params.content_state = webkit_glue::HistoryItemToString(item);
  } else {
    params.content_state =
        webkit_glue::CreateHistoryStateForURL(GURL(request.url()));
  }

  if (!frame->parent()) {

    HostZoomLevels::iterator host_zoom =
        host_zoom_levels_.find(GURL(request.url()));
    if (webview()->mainFrame()->document().isPluginDocument()) {
      webview()->setZoomLevel(false, 0);
    } else {
      if (host_zoom != host_zoom_levels_.end())
        webview()->setZoomLevel(false, host_zoom->second);
    }

    if (host_zoom != host_zoom_levels_.end()) {
      host_zoom_levels_.erase(host_zoom);
    }

    webview()->zoomLimitsChanged(
        WebView::zoomFactorToZoomLevel(WebView::minTextSizeMultiplier),
        WebView::zoomFactorToZoomLevel(WebView::maxTextSizeMultiplier));

    params.contents_mime_type = ds->response().mimeType().utf8();

    params.transition = navigation_state->transition_type();
    if (!PageTransition::IsMainFrame(params.transition)) {
      params.transition = PageTransition::LINK;
    }

    if (completed_client_redirect_src_.is_valid()) {
      DCHECK(completed_client_redirect_src_ == params.redirects[0]);
      params.referrer = completed_client_redirect_src_;
      params.transition = static_cast<PageTransition::Type>(
          params.transition | PageTransition::CLIENT_REDIRECT);
    } else {
      params.referrer = GURL(
          original_request.httpHeaderField(WebString::fromUTF8("Referer")));
    }

    string16 method = request.httpMethod();
    if (EqualsASCII(method, "POST"))
      params.is_post = true;

    UMA_HISTOGRAM_COUNTS_10000("Memory.GlyphPagesPerLoad",
                               webkit_glue::GetGlyphPageCount());

    Send(new ViewHostMsg_FrameNavigate(routing_id_, params));
  } else {
    if (page_id_ > last_page_id_sent_to_browser_)
      params.transition = PageTransition::MANUAL_SUBFRAME;
    else
      params.transition = PageTransition::AUTO_SUBFRAME;

    Send(new ViewHostMsg_FrameNavigate(routing_id_, params));
  }

  last_page_id_sent_to_browser_ =
      std::max(last_page_id_sent_to_browser_, page_id_);

  navigation_state->set_transition_type(PageTransition::LINK);

  if (accessibility_.get() && !navigation_state->was_within_same_page()) {
    accessibility_.reset();
    pending_accessibility_notifications_.clear();
  }
}
