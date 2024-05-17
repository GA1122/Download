void RenderFrameImpl::SendDidCommitProvisionalLoad(
    blink::WebFrame* frame,
    blink::WebHistoryCommitType commit_type,
    const blink::WebHistoryItem& item) {
  DCHECK(!frame_ || frame_ == frame);
  WebDataSource* ds = frame->dataSource();
  DCHECK(ds);

  const WebURLRequest& request = ds->request();
  const WebURLResponse& response = ds->response();

  DocumentState* document_state = DocumentState::FromDataSource(ds);
  NavigationStateImpl* navigation_state =
      static_cast<NavigationStateImpl*>(document_state->navigation_state());
  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentState(document_state);

  FrameHostMsg_DidCommitProvisionalLoad_Params params;
  params.http_status_code = response.httpStatusCode();
  params.url_is_unreachable = ds->hasUnreachableURL();
  params.is_post = false;
  params.intended_as_new_entry =
      navigation_state->request_params().intended_as_new_entry;
  params.did_create_new_entry = commit_type == blink::WebStandardCommit;
  params.post_id = -1;
  params.page_id = render_view_->page_id_;
  params.nav_entry_id = navigation_state->request_params().nav_entry_id;
  params.render_view_routing_id = render_view_->routing_id();
  params.socket_address.set_host(response.remoteIPAddress().utf8());
  params.socket_address.set_port(response.remotePort());
  WebURLResponseExtraDataImpl* extra_data = GetExtraDataFromResponse(response);
  if (extra_data)
    params.was_fetched_via_proxy = extra_data->was_fetched_via_proxy();
  params.was_within_same_page = navigation_state->WasWithinSamePage();
  params.security_info = response.securityInfo();

  params.url = GetLoadingUrl();
  DCHECK(!is_swapped_out_ || params.url == GURL(kSwappedOutURL));

  if (!is_swapped_out_) {
    std::string scheme = frame->document().securityOrigin().protocol().utf8();
    if (url::IsStandard(scheme.c_str(),
                        url::Component(0, static_cast<int>(scheme.length())))) {
      params.origin = frame->document().securityOrigin();
    }
  }

  if (frame->document().baseURL() != params.url)
    params.base_url = frame->document().baseURL();

  GetRedirectChain(ds, &params.redirects);
  params.should_update_history = !ds->hasUnreachableURL() &&
      !response.isMultipartPayload() && (response.httpStatusCode() != 404);

  params.searchable_form_url = internal_data->searchable_form_url();
  params.searchable_form_encoding = internal_data->searchable_form_encoding();

  params.gesture = render_view_->navigation_gesture_;
  render_view_->navigation_gesture_ = NavigationGestureUnknown;

  HistoryEntry* entry = render_view_->history_controller()->GetCurrentEntry();
  if (!SiteIsolationPolicy::UseSubframeNavigationEntries()) {
    if (entry)
      params.page_state = HistoryEntryToPageState(entry);
    else
      params.page_state = PageState::CreateFromURL(request.url());
  } else {
    params.page_state = SingleHistoryItemToPageState(item);
  }
  params.item_sequence_number = item.itemSequenceNumber();
  params.document_sequence_number = item.documentSequenceNumber();

  if (!frame->parent()) {

    render_view_->webview()->zoomLimitsChanged(
        ZoomFactorToZoomLevel(kMinimumZoomFactor),
        ZoomFactorToZoomLevel(kMaximumZoomFactor));

    HostZoomLevels::iterator host_zoom =
        render_view_->host_zoom_levels_.find(GURL(request.url()));
    if (render_view_->webview()->mainFrame()->document().isPluginDocument()) {
      render_view_->webview()->setZoomLevel(0);
    } else {
      if (host_zoom != render_view_->host_zoom_levels_.end())
        render_view_->webview()->setZoomLevel(host_zoom->second);
    }

    if (host_zoom != render_view_->host_zoom_levels_.end()) {
      render_view_->host_zoom_levels_.erase(host_zoom);
    }

    params.contents_mime_type = ds->response().mimeType().utf8();

    params.transition = navigation_state->GetTransitionType();
    if (!ui::PageTransitionIsMainFrame(params.transition)) {
      params.transition = ui::PAGE_TRANSITION_LINK;
    }

    if (ds->isClientRedirect()) {
      params.referrer =
          Referrer(params.redirects[0], ds->request().referrerPolicy());
      params.transition = ui::PageTransitionFromInt(
          params.transition | ui::PAGE_TRANSITION_CLIENT_REDIRECT);
    } else {
      params.referrer = RenderViewImpl::GetReferrerFromRequest(
          frame, ds->request());
    }

    base::string16 method = request.httpMethod();
    if (base::EqualsASCII(method, "POST")) {
      params.is_post = true;
      params.post_id = ExtractPostId(entry);
    }

    params.is_overriding_user_agent = internal_data->is_overriding_user_agent();

    params.original_request_url = GetOriginalRequestURL(ds);

    params.history_list_was_cleared =
        navigation_state->request_params().should_clear_history_list;

    params.report_type = static_cast<FrameMsg_UILoadMetricsReportType::Value>(
        frame->dataSource()->request().inputPerfMetricReportPolicy());
    params.ui_timestamp = base::TimeTicks() + base::TimeDelta::FromSecondsD(
        frame->dataSource()->request().uiStartTime());

    UMA_HISTOGRAM_COUNTS_10000("Memory.GlyphPagesPerLoad",
                               blink::WebGlyphCache::pageCount());

    Send(new FrameHostMsg_DidCommitProvisionalLoad(routing_id_, params));
  } else {
    if (commit_type == blink::WebStandardCommit)
      params.transition = ui::PAGE_TRANSITION_MANUAL_SUBFRAME;
    else
      params.transition = ui::PAGE_TRANSITION_AUTO_SUBFRAME;

    DCHECK(!navigation_state->request_params().should_clear_history_list);
    params.history_list_was_cleared = false;
    params.report_type = FrameMsg_UILoadMetricsReportType::NO_REPORT;

    if (!is_swapped_out())
      Send(new FrameHostMsg_DidCommitProvisionalLoad(routing_id_, params));
  }

  navigation_state->set_transition_type(ui::PAGE_TRANSITION_LINK);
}
