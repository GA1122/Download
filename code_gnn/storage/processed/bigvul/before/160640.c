RenderFrameImpl::MakeDidCommitProvisionalLoadParams(
    blink::WebHistoryCommitType commit_type) {
  WebDocumentLoader* document_loader = frame_->GetDocumentLoader();
  const WebURLRequest& request = document_loader->GetRequest();
  const WebURLResponse& response = document_loader->GetResponse();

  DocumentState* document_state =
      DocumentState::FromDocumentLoader(frame_->GetDocumentLoader());
  NavigationStateImpl* navigation_state =
      static_cast<NavigationStateImpl*>(document_state->navigation_state());
  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentState(document_state);

  std::unique_ptr<FrameHostMsg_DidCommitProvisionalLoad_Params> params =
      std::make_unique<FrameHostMsg_DidCommitProvisionalLoad_Params>();
  params->http_status_code = response.HttpStatusCode();
  params->url_is_unreachable = document_loader->HasUnreachableURL();
  params->method = "GET";
  params->intended_as_new_entry =
      navigation_state->request_params().intended_as_new_entry;
  params->should_replace_current_entry =
      document_loader->ReplacesCurrentHistoryItem();
  params->post_id = -1;
  params->nav_entry_id = navigation_state->request_params().nav_entry_id;
  params->render_view_routing_id = render_view_->routing_id();
  params->was_within_same_document = navigation_state->WasWithinSameDocument();

  params->did_create_new_entry =
      (commit_type == blink::kWebStandardCommit) ||
      (commit_type == blink::kWebHistoryInertCommit && !frame_->Parent() &&
       params->should_replace_current_entry &&
       !navigation_state->WasWithinSameDocument());

  WebDocument frame_document = frame_->GetDocument();
  WebSecurityOrigin frame_origin = frame_document.GetSecurityOrigin();
  params->origin = frame_origin;

  params->insecure_request_policy = frame_->GetInsecureRequestPolicy();
  params->insecure_navigations_set = frame_->GetInsecureRequestToUpgrade();

  params->has_potentially_trustworthy_unique_origin =
      frame_origin.IsUnique() && frame_origin.IsPotentiallyTrustworthy();

  params->url = GetLoadingUrl();
  if (GURL(frame_document.BaseURL()) != params->url)
    params->base_url = frame_document.BaseURL();

  GetRedirectChain(document_loader, &params->redirects);
  params->should_update_history =
      !document_loader->HasUnreachableURL() && response.HttpStatusCode() != 404;

  params->searchable_form_url = internal_data->searchable_form_url();
  params->searchable_form_encoding = internal_data->searchable_form_encoding();

  params->gesture = render_view_->navigation_gesture_;
  render_view_->navigation_gesture_ = NavigationGestureUnknown;

  params->page_state = SingleHistoryItemToPageState(current_history_item_);

  params->content_source_id = GetRenderWidget()->GetContentSourceId();

  params->method = request.HttpMethod().Latin1();
  if (params->method == "POST")
    params->post_id = ExtractPostId(current_history_item_);

  params->item_sequence_number = current_history_item_.ItemSequenceNumber();
  params->document_sequence_number =
      current_history_item_.DocumentSequenceNumber();

  if (document_loader->IsClientRedirect()) {
    params->referrer =
        Referrer(params->redirects[0],
                 document_loader->GetRequest().GetReferrerPolicy());
  } else {
    params->referrer = RenderViewImpl::GetReferrerFromRequest(
        frame_, document_loader->GetRequest());
  }

  if (!frame_->Parent()) {

    params->contents_mime_type =
        document_loader->GetResponse().MimeType().Utf8();

    params->transition = navigation_state->GetTransitionType();
    DCHECK(ui::PageTransitionIsMainFrame(params->transition));

    if (document_loader->IsClientRedirect()) {
      params->transition = ui::PageTransitionFromInt(
          params->transition | ui::PAGE_TRANSITION_CLIENT_REDIRECT);
    }

    params->is_overriding_user_agent =
        internal_data->is_overriding_user_agent();

    params->original_request_url = GetOriginalRequestURL(document_loader);

    params->history_list_was_cleared =
        navigation_state->request_params().should_clear_history_list;

    params->report_type = static_cast<FrameMsg_UILoadMetricsReportType::Value>(
        frame_->GetDocumentLoader()
            ->GetRequest()
            .InputPerfMetricReportPolicy());
    params->ui_timestamp =
        base::TimeTicks() +
        base::TimeDelta::FromSecondsD(
            frame_->GetDocumentLoader()->GetRequest().UiStartTime());
  } else {
    if (commit_type == blink::kWebStandardCommit)
      params->transition = ui::PAGE_TRANSITION_MANUAL_SUBFRAME;
    else
      params->transition = ui::PAGE_TRANSITION_AUTO_SUBFRAME;

    DCHECK(!navigation_state->request_params().should_clear_history_list);
    params->history_list_was_cleared = false;
    params->report_type = FrameMsg_UILoadMetricsReportType::NO_REPORT;
  }

  if (!params->origin.unique() && params->url.IsStandard() &&
      render_view_->GetWebkitPreferences().web_security_enabled) {
    if (params->origin.scheme() != url::kFileScheme ||
        !render_view_->GetWebkitPreferences()
             .allow_universal_access_from_file_urls) {
      CHECK(params->origin.IsSamePhysicalOriginWith(
          url::Origin::Create(params->url)))
          << " url:" << params->url << " origin:" << params->origin;
    }
  }

  return params;
}
