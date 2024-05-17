RenderFrameImpl::MakeDidCommitProvisionalLoadParams(
    blink::WebHistoryCommitType commit_type,
    ui::PageTransition transition) {
  WebDocumentLoader* document_loader = frame_->GetDocumentLoader();
  const WebURLResponse& response = document_loader->GetResponse();

  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentLoader(
          frame_->GetDocumentLoader());
  NavigationState* navigation_state = internal_data->navigation_state();

  std::unique_ptr<FrameHostMsg_DidCommitProvisionalLoad_Params> params =
      std::make_unique<FrameHostMsg_DidCommitProvisionalLoad_Params>();
  params->http_status_code = response.HttpStatusCode();
  params->url_is_unreachable = document_loader->HasUnreachableURL();
  params->method = "GET";
  params->intended_as_new_entry =
      navigation_state->commit_params().intended_as_new_entry;
  params->should_replace_current_entry =
      document_loader->ReplacesCurrentHistoryItem();
  params->post_id = -1;
  params->nav_entry_id = navigation_state->commit_params().nav_entry_id;

  params->navigation_token = navigation_state->commit_params().navigation_token;
  if (params->navigation_token.is_empty())
    params->navigation_token = base::UnguessableToken::Create();

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

  params->gesture = document_loader->HadUserGesture() ? NavigationGestureUser
                                                      : NavigationGestureAuto;

  params->page_state = SingleHistoryItemToPageState(current_history_item_);

  params->content_source_id = GetLocalRootRenderWidget()->GetContentSourceId();

  params->method = document_loader->HttpMethod().Latin1();
  if (params->method == "POST")
    params->post_id = ExtractPostId(current_history_item_);

  params->item_sequence_number = current_history_item_.ItemSequenceNumber();
  params->document_sequence_number =
      current_history_item_.DocumentSequenceNumber();

  if (document_loader->IsClientRedirect()) {
    params->referrer =
        Referrer(params->redirects[0], document_loader->GetReferrerPolicy());
  } else {
    params->referrer =
        Referrer(blink::WebStringToGURL(document_loader->Referrer()),
                 document_loader->GetReferrerPolicy());
  }

  if (!frame_->Parent()) {

    params->contents_mime_type =
        document_loader->GetResponse().MimeType().Utf8();

    params->transition = transition;
    DCHECK(ui::PageTransitionIsMainFrame(params->transition));

    if (document_loader->IsClientRedirect()) {
      params->transition = ui::PageTransitionFromInt(
          params->transition | ui::PAGE_TRANSITION_CLIENT_REDIRECT);
    }

    params->is_overriding_user_agent =
        internal_data->is_overriding_user_agent();

    params->original_request_url = GetOriginalRequestURL(document_loader);

    params->history_list_was_cleared =
        navigation_state->commit_params().should_clear_history_list;
  } else {
    if (commit_type == blink::kWebStandardCommit)
      params->transition = ui::PAGE_TRANSITION_MANUAL_SUBFRAME;
    else
      params->transition = ui::PAGE_TRANSITION_AUTO_SUBFRAME;

    DCHECK(!navigation_state->commit_params().should_clear_history_list);
    params->history_list_was_cleared = false;
  }

  if (!params->origin.opaque() && params->url.IsStandard() &&
      render_view_->GetWebkitPreferences().web_security_enabled) {
    if (params->origin.scheme() != url::kFileScheme ||
        !render_view_->GetWebkitPreferences()
             .allow_universal_access_from_file_urls) {
      CHECK(params->origin.IsSameOriginWith(url::Origin::Create(params->url)))
          << " url:" << params->url << " origin:" << params->origin;
    }
  }
  params->request_id = internal_data->request_id();

  return params;
}
