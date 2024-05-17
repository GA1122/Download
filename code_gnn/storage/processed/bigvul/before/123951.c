void RenderViewImpl::UpdateURL(WebFrame* frame) {
  WebDataSource* ds = frame->dataSource();
  DCHECK(ds);

  const WebURLRequest& request = ds->request();
  const WebURLRequest& original_request = ds->originalRequest();
  const WebURLResponse& response = ds->response();

  DocumentState* document_state = DocumentState::FromDataSource(ds);
  NavigationState* navigation_state = document_state->navigation_state();

  ViewHostMsg_FrameNavigate_Params params;
  params.http_status_code = response.httpStatusCode();
  params.is_post = false;
  params.post_id = -1;
  params.page_id = page_id_;
  params.frame_id = frame->identifier();
  params.socket_address.set_host(response.remoteIPAddress().utf8());
  params.socket_address.set_port(response.remotePort());
  WebURLResponseExtraDataImpl* extra_data = GetExtraDataFromResponse(response);
  if (extra_data) {
    params.was_fetched_via_proxy = extra_data->was_fetched_via_proxy();
  }
  params.was_within_same_page = navigation_state->was_within_same_page();
  if (!document_state->security_info().empty()) {
    DCHECK(response.securityInfo().isEmpty());
    params.security_info = document_state->security_info();
  } else {
    params.security_info = response.securityInfo();
  }

  params.url = GetLoadingUrl(frame);

  if (frame->document().baseURL() != params.url)
    params.base_url = frame->document().baseURL();

  GetRedirectChain(ds, &params.redirects);
  params.should_update_history = !ds->hasUnreachableURL() &&
      !response.isMultipartPayload() && (response.httpStatusCode() != 404);

  params.searchable_form_url = document_state->searchable_form_url();
  params.searchable_form_encoding =
      document_state->searchable_form_encoding();

  const PasswordForm* password_form_data =
      document_state->password_form_data();
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

    webview()->zoomLimitsChanged(
        WebView::zoomFactorToZoomLevel(kMinimumZoomFactor),
        WebView::zoomFactorToZoomLevel(kMaximumZoomFactor));

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

    params.contents_mime_type = ds->response().mimeType().utf8();

    params.transition = navigation_state->transition_type();
    if (!PageTransitionIsMainFrame(params.transition)) {
      params.transition = PAGE_TRANSITION_LINK;
    }

    if (completed_client_redirect_src_.url.is_valid()) {
      DCHECK(completed_client_redirect_src_.url == params.redirects[0]);
      params.referrer = completed_client_redirect_src_;
      params.transition = static_cast<PageTransition>(
          params.transition | PAGE_TRANSITION_CLIENT_REDIRECT);
    } else {
      params.referrer = Referrer(GURL(
          original_request.httpHeaderField(WebString::fromUTF8("Referer"))),
          GetReferrerPolicyFromRequest(frame, original_request));
    }

    string16 method = request.httpMethod();
    if (EqualsASCII(method, "POST")) {
      params.is_post = true;
      params.post_id = ExtractPostId(item);
    }

    params.is_overriding_user_agent =
        document_state->is_overriding_user_agent();

    params.original_request_url = original_request.url();

    UMA_HISTOGRAM_COUNTS_10000("Memory.GlyphPagesPerLoad",
                               webkit_glue::GetGlyphPageCount());

    Send(new ViewHostMsg_FrameNavigate(routing_id_, params));
  } else {
    if (page_id_ > last_page_id_sent_to_browser_)
      params.transition = PAGE_TRANSITION_MANUAL_SUBFRAME;
    else
      params.transition = PAGE_TRANSITION_AUTO_SUBFRAME;

    Send(new ViewHostMsg_FrameNavigate(routing_id_, params));
  }

  last_page_id_sent_to_browser_ =
      std::max(last_page_id_sent_to_browser_, page_id_);

  navigation_state->set_transition_type(PAGE_TRANSITION_LINK);
}
