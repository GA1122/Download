void RenderFrameImpl::WillSendRequest(blink::WebURLRequest& request) {
  WebDocumentLoader* provisional_document_loader =
      frame_->GetProvisionalDocumentLoader();
  WebDocumentLoader* document_loader = provisional_document_loader
                                           ? provisional_document_loader
                                           : frame_->GetDocumentLoader();

  DocumentState* document_state =
      DocumentState::FromDocumentLoader(document_loader);
  DCHECK(document_state);
  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentState(document_state);
  NavigationStateImpl* navigation_state =
      static_cast<NavigationStateImpl*>(document_state->navigation_state());
  ui::PageTransition transition_type = navigation_state->GetTransitionType();
  if (provisional_document_loader &&
      provisional_document_loader->IsClientRedirect()) {
    transition_type = ui::PageTransitionFromInt(
        transition_type | ui::PAGE_TRANSITION_CLIENT_REDIRECT);
  }

  GURL new_url;
  if (GetContentClient()->renderer()->WillSendRequest(
          frame_, transition_type, request.Url(), &new_url)) {
    request.SetURL(WebURL(new_url));
  }

  if (internal_data->is_cache_policy_override_set())
    request.SetCacheMode(internal_data->cache_policy_override());

  WebString custom_user_agent;
  WebString requested_with;
  std::unique_ptr<StreamOverrideParameters> stream_override;
  if (request.GetExtraData()) {
    RequestExtraData* old_extra_data =
        static_cast<RequestExtraData*>(request.GetExtraData());

    custom_user_agent = old_extra_data->custom_user_agent();
    if (!custom_user_agent.IsNull()) {
      if (custom_user_agent.IsEmpty())
        request.ClearHTTPHeaderField("User-Agent");
      else
        request.SetHTTPHeaderField("User-Agent", custom_user_agent);
    }

    requested_with = old_extra_data->requested_with();
    if (!requested_with.IsNull()) {
      if (requested_with.IsEmpty())
        request.ClearHTTPHeaderField("X-Requested-With");
      else
        request.SetHTTPHeaderField("X-Requested-With", requested_with);
    }
    stream_override = old_extra_data->TakeStreamOverrideOwnership();
  }

  request.AddHTTPOriginIfNeeded(WebSecurityOrigin::CreateUnique());

  bool should_replace_current_entry =
      document_loader->ReplacesCurrentHistoryItem();

  WebFrame* parent = frame_->Parent();

  ResourceType resource_type = WebURLRequestToResourceType(request);
  WebDocument frame_document = frame_->GetDocument();
  RequestExtraData* extra_data =
      static_cast<RequestExtraData*>(request.GetExtraData());
  if (!extra_data)
    extra_data = new RequestExtraData();
  extra_data->set_visibility_state(VisibilityState());
  extra_data->set_custom_user_agent(custom_user_agent);
  extra_data->set_requested_with(requested_with);
  extra_data->set_render_frame_id(routing_id_);
  extra_data->set_is_main_frame(!parent);
  extra_data->set_frame_origin(url::Origin(frame_document.GetSecurityOrigin()));
  extra_data->set_allow_download(
      navigation_state->common_params().allow_download);
  extra_data->set_transition_type(transition_type);
  extra_data->set_should_replace_current_entry(should_replace_current_entry);
  extra_data->set_stream_override(std::move(stream_override));
  bool is_prefetch =
      GetContentClient()->renderer()->IsPrefetchOnly(this, request);
  extra_data->set_is_prefetch(is_prefetch);
  extra_data->set_download_to_network_cache_only(
      is_prefetch && resource_type != RESOURCE_TYPE_MAIN_FRAME);
  extra_data->set_initiated_in_secure_context(frame_document.IsSecureContext());

  bool is_navigational_request =
      request.GetFrameType() != network::mojom::RequestContextFrameType::kNone;
  if (is_navigational_request) {
    RequestExtraData* current_request_data =
        static_cast<RequestExtraData*>(request.GetExtraData());
    if (current_request_data) {
      extra_data->set_navigation_initiated_by_renderer(
          current_request_data->navigation_initiated_by_renderer());
    }
  }

  RenderThreadImpl* render_thread = RenderThreadImpl::current();
  if (render_thread && render_thread->url_loader_throttle_provider()) {
    extra_data->set_url_loader_throttles(
        render_thread->url_loader_throttle_provider()->CreateThrottles(
            routing_id_, request.Url(), resource_type));
  }

  request.SetExtraData(extra_data);

  if (request.GetPreviewsState() == WebURLRequest::kPreviewsUnspecified) {
    if (is_main_frame_ && !navigation_state->request_committed()) {
      request.SetPreviewsState(static_cast<WebURLRequest::PreviewsState>(
          navigation_state->common_params().previews_state));
    } else {
      WebURLRequest::PreviewsState request_previews_state =
          static_cast<WebURLRequest::PreviewsState>(previews_state_);

      request_previews_state &= ~(WebURLRequest::kClientLoFiOn);
      if (request_previews_state == WebURLRequest::kPreviewsUnspecified)
        request_previews_state = WebURLRequest::kPreviewsOff;

      request.SetPreviewsState(request_previews_state);
    }
  }

  request.SetRequestorID(render_view_->GetRoutingID());
  request.SetHasUserGesture(
      WebUserGestureIndicator::IsProcessingUserGesture(frame_));

  if (!render_view_->renderer_preferences_.enable_referrers)
    request.SetHTTPReferrer(WebString(), blink::kWebReferrerPolicyDefault);
}
