void RenderFrameImpl::willSendRequest(
    blink::WebLocalFrame* frame,
    unsigned identifier,
    blink::WebURLRequest& request,
    const blink::WebURLResponse& redirect_response) {
  DCHECK(!frame_ || frame_ == frame);
  if (request.url().isEmpty())
    return;

  if (request.firstPartyForCookies().isEmpty()) {
    if (request.frameType() == blink::WebURLRequest::FrameTypeTopLevel) {
      request.setFirstPartyForCookies(request.url());
    } else {
      WebFrame* top = frame->top();
      if (top->isWebLocalFrame()) {
        request.setFirstPartyForCookies(
            frame->top()->document().firstPartyForCookies());
      }
    }
  }

  WebDataSource* provisional_data_source = frame->provisionalDataSource();
  WebDataSource* data_source =
      provisional_data_source ? provisional_data_source : frame->dataSource();

  DocumentState* document_state = DocumentState::FromDataSource(data_source);
  DCHECK(document_state);
  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentState(document_state);
  NavigationStateImpl* navigation_state =
      static_cast<NavigationStateImpl*>(document_state->navigation_state());
  ui::PageTransition transition_type = navigation_state->GetTransitionType();
  if (provisional_data_source && provisional_data_source->isClientRedirect()) {
    transition_type = ui::PageTransitionFromInt(
        transition_type | ui::PAGE_TRANSITION_CLIENT_REDIRECT);
  }

  GURL request_url(request.url());
  GURL new_url;
  if (GetContentClient()->renderer()->WillSendRequest(
          frame,
          transition_type,
          request_url,
          request.firstPartyForCookies(),
          &new_url)) {
    request.setURL(WebURL(new_url));
  }

  if (internal_data->is_cache_policy_override_set())
    request.setCachePolicy(internal_data->cache_policy_override());

  WebString custom_user_agent;
  WebString requested_with;
  scoped_ptr<StreamOverrideParameters> stream_override;
  if (request.extraData()) {
    RequestExtraData* old_extra_data =
        static_cast<RequestExtraData*>(request.extraData());

    custom_user_agent = old_extra_data->custom_user_agent();
    if (!custom_user_agent.isNull()) {
      if (custom_user_agent.isEmpty())
        request.clearHTTPHeaderField("User-Agent");
      else
        request.setHTTPHeaderField("User-Agent", custom_user_agent);
    }

    requested_with = old_extra_data->requested_with();
    if (!requested_with.isNull()) {
      if (requested_with.isEmpty())
        request.clearHTTPHeaderField("X-Requested-With");
      else
        request.setHTTPHeaderField("X-Requested-With", requested_with);
    }
    stream_override = old_extra_data->TakeStreamOverrideOwnership();
  }

  if ((request.frameType() == blink::WebURLRequest::FrameTypeTopLevel ||
       request.frameType() == blink::WebURLRequest::FrameTypeNested) &&
      request.httpHeaderField(WebString::fromUTF8(kAcceptHeader)).isEmpty()) {
    request.setHTTPHeaderField(WebString::fromUTF8(kAcceptHeader),
                               WebString::fromUTF8(kDefaultAcceptHeader));
  }

  request.addHTTPOriginIfNeeded(WebString());

  bool should_replace_current_entry = false;
  if (navigation_state->IsContentInitiated()) {
    should_replace_current_entry = data_source->replacesCurrentHistoryItem();
  } else {
    should_replace_current_entry =
        navigation_state->common_params().should_replace_current_entry;
  }

  int provider_id = kInvalidServiceWorkerProviderId;
  if (request.frameType() == blink::WebURLRequest::FrameTypeTopLevel ||
      request.frameType() == blink::WebURLRequest::FrameTypeNested) {
    if (frame->provisionalDataSource()) {
      ServiceWorkerNetworkProvider* provider =
          ServiceWorkerNetworkProvider::FromDocumentState(
              DocumentState::FromDataSource(frame->provisionalDataSource()));
      provider_id = provider->provider_id();
    }
  } else if (frame->dataSource()) {
    ServiceWorkerNetworkProvider* provider =
        ServiceWorkerNetworkProvider::FromDocumentState(
            DocumentState::FromDataSource(frame->dataSource()));
    provider_id = provider->provider_id();
  }

  WebFrame* parent = frame->parent();
  int parent_routing_id = MSG_ROUTING_NONE;
  if (!parent) {
    parent_routing_id = -1;
  } else if (parent->isWebLocalFrame()) {
    parent_routing_id = FromWebFrame(parent)->GetRoutingID();
  } else {
    parent_routing_id = RenderFrameProxy::FromWebFrame(parent)->routing_id();
  }

  RequestExtraData* extra_data = new RequestExtraData();
  extra_data->set_visibility_state(render_view_->visibilityState());
  extra_data->set_custom_user_agent(custom_user_agent);
  extra_data->set_requested_with(requested_with);
  extra_data->set_render_frame_id(routing_id_);
  extra_data->set_is_main_frame(!parent);
  extra_data->set_frame_origin(
      GURL(frame->document().securityOrigin().toString()));
  extra_data->set_parent_is_main_frame(parent && !parent->parent());
  extra_data->set_parent_render_frame_id(parent_routing_id);
  extra_data->set_allow_download(
      navigation_state->common_params().allow_download);
  extra_data->set_transition_type(transition_type);
  extra_data->set_should_replace_current_entry(should_replace_current_entry);
  extra_data->set_transferred_request_child_id(
      navigation_state->start_params().transferred_request_child_id);
  extra_data->set_transferred_request_request_id(
      navigation_state->start_params().transferred_request_request_id);
  extra_data->set_service_worker_provider_id(provider_id);
  extra_data->set_stream_override(stream_override.Pass());
  request.setExtraData(extra_data);

  WebFrame* top_frame = frame->top();
  if (top_frame && top_frame->isWebLocalFrame()) {
    DocumentState* top_document_state =
        DocumentState::FromDataSource(top_frame->dataSource());
    if (top_document_state) {
      if (request.requestContext() == WebURLRequest::RequestContextPrefetch)
        top_document_state->set_was_prefetcher(true);
    }
  }

  request.setRequestorID(render_view_->GetRoutingID());
  request.setHasUserGesture(WebUserGestureIndicator::isProcessingUserGesture());

  if (!navigation_state->start_params().extra_headers.empty()) {
    for (net::HttpUtil::HeadersIterator i(
             navigation_state->start_params().extra_headers.begin(),
             navigation_state->start_params().extra_headers.end(), "\n");
         i.GetNext();) {
      if (base::LowerCaseEqualsASCII(i.name(), "referer")) {
        WebString referrer = WebSecurityPolicy::generateReferrerHeader(
            blink::WebReferrerPolicyDefault,
            request.url(),
            WebString::fromUTF8(i.values()));
        request.setHTTPReferrer(referrer, blink::WebReferrerPolicyDefault);
      } else {
        request.setHTTPHeaderField(WebString::fromUTF8(i.name()),
                                   WebString::fromUTF8(i.values()));
      }
    }
  }

  if (!render_view_->renderer_preferences_.enable_referrers)
    request.setHTTPReferrer(WebString(), blink::WebReferrerPolicyDefault);
}
