void RenderFrameImpl::WillSendRequestInternal(
    blink::WebURLRequest& request,
    ResourceType resource_type,
    DocumentState* document_state,
    ui::PageTransition transition_type) {
  if (render_view_->renderer_preferences_.enable_do_not_track)
    request.SetHttpHeaderField(blink::WebString::FromUTF8(kDoNotTrackHeader),
                               "1");

  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentState(document_state);
  NavigationState* navigation_state = internal_data->navigation_state();

  ApplyFilePathAlias(&request);
  GURL new_url;
  bool attach_same_site_cookies = false;
  base::Optional<url::Origin> initiator_origin =
      request.RequestorOrigin().IsNull()
          ? base::Optional<url::Origin>()
          : base::Optional<url::Origin>(request.RequestorOrigin());
  GetContentClient()->renderer()->WillSendRequest(
      frame_, transition_type, request.Url(),
      base::OptionalOrNullptr(initiator_origin), &new_url,
      &attach_same_site_cookies);
  if (!new_url.is_empty())
    request.SetUrl(WebURL(new_url));

  if (internal_data->is_cache_policy_override_set())
    request.SetCacheMode(internal_data->cache_policy_override());

  WebString custom_user_agent;
  std::unique_ptr<NavigationResponseOverrideParameters> response_override;
  if (request.GetExtraData()) {
    RequestExtraData* old_extra_data =
        static_cast<RequestExtraData*>(request.GetExtraData());

    custom_user_agent = old_extra_data->custom_user_agent();
    if (!custom_user_agent.IsNull()) {
      if (custom_user_agent.IsEmpty())
        request.ClearHttpHeaderField("User-Agent");
      else
        request.SetHttpHeaderField("User-Agent", custom_user_agent);
    }
    response_override =
        old_extra_data->TakeNavigationResponseOverrideOwnership();
  }

  request.SetHttpOriginIfNeeded(WebSecurityOrigin::CreateUnique());

  WebDocument frame_document = frame_->GetDocument();
  if (!request.GetExtraData())
    request.SetExtraData(std::make_unique<RequestExtraData>());
  auto* extra_data = static_cast<RequestExtraData*>(request.GetExtraData());
  extra_data->set_is_preprerendering(
      GetContentClient()->renderer()->IsPrerenderingFrame(this));
  extra_data->set_custom_user_agent(custom_user_agent);
  extra_data->set_render_frame_id(routing_id_);
  extra_data->set_is_main_frame(IsMainFrame());
  extra_data->set_allow_download(
      navigation_state->common_params().download_policy.IsDownloadAllowed());
  extra_data->set_transition_type(transition_type);
  extra_data->set_navigation_response_override(std::move(response_override));
  bool is_for_no_state_prefetch =
      GetContentClient()->renderer()->IsPrefetchOnly(this, request);
  extra_data->set_is_for_no_state_prefetch(is_for_no_state_prefetch);
  extra_data->set_initiated_in_secure_context(frame_document.IsSecureContext());
  extra_data->set_attach_same_site_cookies(attach_same_site_cookies);
  extra_data->set_frame_request_blocker(frame_request_blocker_);

  request.SetDownloadToNetworkCacheOnly(
      is_for_no_state_prefetch && resource_type != ResourceType::kMainFrame);

  RenderThreadImpl* render_thread = RenderThreadImpl::current();
  if (render_thread && render_thread->url_loader_throttle_provider()) {
    extra_data->set_url_loader_throttles(
        render_thread->url_loader_throttle_provider()->CreateThrottles(
            routing_id_, request, resource_type));
  }

  if (request.GetPreviewsState() == WebURLRequest::kPreviewsUnspecified) {
    if (is_main_frame_ && !navigation_state->request_committed()) {
      request.SetPreviewsState(static_cast<WebURLRequest::PreviewsState>(
          navigation_state->common_params().previews_state));
    } else {
      WebURLRequest::PreviewsState request_previews_state =
          static_cast<WebURLRequest::PreviewsState>(previews_state_);

      request_previews_state &= ~(WebURLRequest::kClientLoFiOn);
      request_previews_state &= ~(WebURLRequest::kLazyImageLoadDeferred);
      if (request_previews_state == WebURLRequest::kPreviewsUnspecified)
        request_previews_state = WebURLRequest::kPreviewsOff;

      request.SetPreviewsState(request_previews_state);
    }
  }

  request.SetRequestorID(render_view_->GetRoutingID());
  request.SetHasUserGesture(
      WebUserGestureIndicator::IsProcessingUserGesture(frame_));

  if (!render_view_->renderer_preferences_.enable_referrers)
    request.SetHttpReferrer(WebString(),
                            network::mojom::ReferrerPolicy::kDefault);
}